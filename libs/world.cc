#include "world.h"

#define fitnessgainfood 100
#define socialfitnessgainfood 50
#define nrays 20
#define raylength 20
#define raystepsize 0.2
#define ROT 360.0
#define FOV 180.0
#define NFOVDISTR 3

world::world(){
	//world size
	width = 100;
	height = 100;

	//whats in the world
	nrobots = 1;
	maxfood = 10;

	//widths for raycast collision 
	foodwidth = 2.0;
	robotwidth = 2.0;
	
	//true = robots gain fitness from other robots collecting food
	social = true;

	//random number generating
	std::random_device dev;
    std::mt19937 rng(dev());
    //food has to be completely inside the world
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (foodwidth * 10, width * 10  - foodwidth * 10); // distribution in range [foodwidth*10, width*10  - foodwidth*10]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (foodwidth * 10, height * 10 - foodwidth * 10); // distribution in range [foodwidth*10, height*10 - foodwidth*10]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT*10); // distribution in range [0, rot]
	
	//setup the start of the world
	frames = 0;
	worlddone = false;
	nfood = maxfood;
	currentmaxfitness = 0;
	currentaveragefitness = 0;

	//setup the robots
	for(int i = 0; i < nrobots; i++){
		//spawn robot in a random location
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		float newrot = ((float)rotdist (rng))/10;
		robot Newrobot(newx, newy, newrot, i, robotwidth);
		robots.push_back(Newrobot);
	}

	//setup the food
	for(int i = 0; i < nfood; i++){
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		food Newfood(newx, newy, i, foodwidth);
		foods.push_back(Newfood);
	}
}

void world::randomizeworld(int randfood){
	frames = 0;
	worlddone = false;
	//random number generating
	std::random_device dev;
    std::mt19937 rng(dev());
    //make food not be partly outside of the world
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (foodwidth * 10, width * 10  - foodwidth * 10); // distribution in range [foodwidth, width*10  - foodwidth]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (foodwidth * 10, height * 10 - foodwidth * 10); // distribution in range [foodwidth, height*10 - foodwidth]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist    (0,ROT*10); // distribution in range [0, ROT*10]
	//remove the food
	foods.clear();
	//for experimenting with random amounds of food
	if(randfood > 0)
		nfood = randfood;
	else
		nfood = maxfood;

	//put new food back 
	for(int i = 0; i < nfood; i++){
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		food Newfood(newx, newy, i, foodwidth);
		foods.push_back(Newfood);
	}
	//randomize the robots locations and reset necessary
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		//spawn robot in a random location
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		float newrot = ((float)rotdist (rng))/10;
		robots[i].rotation = newrot;
		robots[i].x = newx;
		robots[i].y = newy;
		//robots[i].fitness = 0;
		robots[i].foodcollected = 0;
		robots[i].socialfoodcollected = 0;
		robots[i].distancetraveled = 0;
		robots[i].allfoodcollected = false;
		robots[i].iterations = 0;
	}
}

void world::randomizerobots(){
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		//spawn robot in a random location
		robots[i].randomize();
	}
}

bool world::done(){
	return worlddone;
}

void world::simulate(){
	for(std::vector<robot>::size_type i = 0; i < robots.size(); i++) {
		checkfoodcollision(i);
		
		float foodint[NFOVDISTR];
		for(int j =0 ; j<NFOVDISTR; j++){
			foodint[j] = foodahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation, j);
		}
		float robotint[NFOVDISTR];
		for(int j =0 ; j<NFOVDISTR; j++){
			robotint[j] = robotahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation, j, i);
		}
		float distance[3];
		//normalize distance to wall to the ray length and reverse it (1 is close to wall 0 is raylength away)(so the robot can take action when he gets too close to a wall)
		distance[0] = (raylength - distancetowall(robots[i].x, robots[i].y, robots[i].rotation)) / raylength;
		distance[1] = (raylength - distancetowall(robots[i].x, robots[i].y, robots[i].rotation - 90.0)) / raylength;
		distance[2] = (raylength - distancetowall(robots[i].x, robots[i].y, robots[i].rotation + 90.0)) / raylength;
		robots[i].simulate(foodint[0], foodint[1], foodint[2], distance[0], distance[1], distance[2], robotint[0], robotint[1], robotint[2]);
		moverobot(i);
		//every step we substract one from the fitness so the robots that collect all food fastest get favoured
		//robots[i].fitness -= 1;
	}
	//check if world is done
	if(nfood < 1 && !worlddone){
		//give all robots a bonus for collecting all food (for simplicity equal to collecting an extra food)
		for(std::vector<robot>::size_type j = 0; j < robots.size(); j++) {
			robots[j].allfoodcollected = true;
		}
		worlddone = true;
	}
	frames++;
}

//works the same as a single ray. 
float world::distancetowall(float x, float y, float degree){
	float originalx = x;
	float originaly = y;
	for(float i = 0; i < raylength; i += raystepsize){
		//robots can be exactly on the edge so extend the edges 
		if(x > -0.1 && x < width+0.1 && y > -0.1 && y < height + 0.1){
			x += raystepsize * cos(degree*M_PI/180);
			y += raystepsize * sin(degree*M_PI/180);
		}
	}
	//make the calculation be exactly on the wall for precision.
	if(x > width)  x = width;
	if(y > height) y = height;
	if(x < 0)      x = 0;
	if(y < 0)	   y = 0;
	float distance = (float)sqrt(pow(abs(originalx - x),2) + pow(abs(originaly - y),2));
	return distance;
}

float world::foodahead(float x, float y, float radius, float rotation, int n){
	float intensity = 0;
	rotation -= (FOV/2) - (FOV/NFOVDISTR * n);
	for(int i = 0; i<nrays; i++){
		// FOV/NFOVDISTR = amount of degrees for the current section the robot is scanning divided by rays give the amount of degrees seperating the rays
		float ray = castrayfood(x,y,rotation+i*(FOV/NFOVDISTR/nrays));
		//add the intensity of this ray divided by the total number of rays (if all rays hit intensity will be 1 (not taking in account the distance of the food))
		intensity += (float)ray/nrays;
	}
	return intensity;
}

float world::robotahead(float x, float y, float radius, float rotation, int n, std::vector<robot>::size_type currobot){
	float intensity = 0;
	rotation -= (FOV/2) - (FOV/NFOVDISTR * n);
	for(int i = 0; i<nrays; i++){
		// FOV/NFOVDISTR = amount of degrees for the current section the robot is scanning divided by rays give the amount of degrees seperating the rays
		float ray = castrayrobot(x,y,rotation+i*(FOV/NFOVDISTR/nrays), currobot);
		//add the intensity of this ray divided by the total number of rays (if all rays hit intensity will be 1 (not taking in account the distance of the food))
		intensity += (float)ray/nrays;
	}
	return intensity;
}

float world::castrayfood(float x, float y, float degree){
	//use robot 0's fixrotation since he (should) always exists 
	degree = robots[0].fixrotation(degree);
	float originalx = x;
	float originaly = y;
	for(float i = 0; i < raylength; i += raystepsize){
		for(std::vector<food>::size_type j = 0; j < foods.size(); j++) {
			float fx = foods[j].x;
			float fy = foods[j].y;
			float fw = foods[j].width;
			//collision with food
			if((fx + fw > x ) && (fx - fw < x) && (fy + fw > y) && (fy - fw< y)){
				//don't return length just that it has seen food
				return 1.0;
				//return length to food+
				//return (float)sqrt(pow(abs(originalx - fx),2) + pow(abs(originaly - fy),2)) / raylength;
			}
		}
		x += raystepsize * cos(degree*M_PI/180);
		y += raystepsize * sin(degree*M_PI/180);
		//if x or y out of bounds end the loop
		if(x>width || x<0||y>height||y<0)
			i = raylength;
	}
	return 0;
}

float world::castrayrobot(float x, float y, float degree, std::vector<robot>::size_type currobot){
	degree = robots[0].fixrotation(degree);
	float originalx = x;
	float originaly = y;
	for(float i = 0; i < raylength; i += raystepsize){
		for(std::vector<robot>::size_type j = 0; j < robots.size(); j++) {
			//don't check collision with the robot sending the rays (obviously)
			if(j != currobot){
				float fx = robots[j].x;
				float fy = robots[j].y;
				float fw = robots[j].width;
				//collision with food
				if((fx + fw > x ) && (fx - fw < x) && (fy + fw > y) && (fy - fw< y)){
					//don't return length just that it has seen robot
					return 1.0;
					//return length to food+
					//return (float)sqrt(pow(abs(originalx - fx),2) + pow(abs(originaly - fy),2)) / raylength;
				}
			}
		}
		x += raystepsize * cos(degree*M_PI/180);
		y += raystepsize * sin(degree*M_PI/180);
		//if x or y out of bounds end the loop
		if(x>width || x<0||y>height||y<0)
			i = raylength;
	}
	return 0;
}

void world::moverobot(std::vector<robot>::size_type i){
	float robotx = robots[i].x;
	float roboty = robots[i].y;
	robots[i].x += robots[i].speed * cos(robots[i].rotation*M_PI/180);
	robots[i].y += robots[i].speed * sin(robots[i].rotation*M_PI/180);
	//edge
	if(robots[i].x > width)  robots[i].x = width;
	if(robots[i].y > height) robots[i].y = height;
	if(robots[i].x < 0)      robots[i].x = 0;
	if(robots[i].y < 0)		 robots[i].y = 0;
	//robots gain fitness for the amount they moved
	robots[i].distancetraveled += sqrt(pow(abs(robots[i].x - robotx),2) + pow(abs(robots[i].y - roboty),2));
	//std::cout<<"robot "<<i<<" moved: "<<sqrt(pow(abs(robots[i].x - robotx),2) + pow(abs(robots[i].y - roboty),2))<<std::endl;
}

void world::checkfoodcollision(std::vector<robot>::size_type i){
	float rx = robots[i].x;
	float ry = robots[i].y;
	float rr = robots[i].grabradius;

	for(std::vector<food>::size_type j = 0; j < foods.size(); j++) {
		float fx = foods[j].x;
		float fy = foods[j].y;
		//collision with food
		if((fx > rx - rr) && (fx < rx + rr) && (fy > ry - rr) && (fy < ry + rr)){
			//robots[i].fitness += fitnessgainfood / (1 + robots[i].foodcollected);
			robots[i].foodcollected += 1;
			//if social other robots may also gain fitness
			if(social){
				for(std::vector<robot>::size_type k = 0; k != robots.size(); k++) {
					//check that we don't give extra fitness to the robot that touched the food only other robots
					if(i!=k){
						robots[k].socialfoodcollected++;
					}
				}
			}
			foods.erase(foods.begin() + j);
			nfood--;
			//subtract 1 from iterator since we erase the current food
			j--;
		}
	}
}

void world::updaterobots(float adapt){
	float bestinputtohidden[MAX][MAX];
	float besthiddentooutput[MAX][MAX];

	int maxfitness = -9000000;
	std::vector<robot>::size_type bestrobot = -1;
	float avaragefitness = 0;

	//find the most succesfull robot
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		//std::cout<<robotchar(robots[i].nrobot)<<": "<<robots[i].fitness<<std::endl;
		avaragefitness += robots[i].returnfitness();		
		if(robots[i].returnfitness() > maxfitness){
			maxfitness = robots[i].returnfitness();
			bestrobot = i;
		}
	}
	//std::cout<<"best robot: "<<robotchar(robots[bestrobot].nrobot)<<", fitness: "<<robots[bestrobot].fitness<<std::endl;
	//take genes from best robot
	robots[bestrobot].returnith(bestinputtohidden);
	robots[bestrobot].returnhto(besthiddentooutput);

	//give best genes to other robots
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		//best robot keeps the same genes
		if(i != bestrobot){
			robots[i].newith(bestinputtohidden);
			robots[i].newhto(besthiddentooutput);
		}
		robots[i].adjustlearningrate(adapt);
		robots[i].generation++;
	}
	currentaveragefitness = avaragefitness / nrobots;
	currentmaxfitness = maxfitness;
}

char world::robotchar(int nrobot){
	//can't use 0
	if(nrobot<9) {
		return '1'+ nrobot;
	}
	else{
		char robot = 'a';
		robot+= nrobot - 9;
		return robot;	
	}
}

void world::savebestrobot(std::string filename){
	int maxfitness = -100000;
	std::vector<robot>::size_type bestrobot;
	//find the most succesfull robot
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		if(robots[i].returnfitness() > maxfitness){
			maxfitness = robots[i].returnfitness();
			bestrobot = i;
		}
	}
	robots[bestrobot].savenodes(filename);
}

void world::loadrobot(std::string filename){
	float inputith[MAX][MAX];
	float inputhto[MAX][MAX];
	int x = 0, y = 0;
	bool ithdone = false;
	std::string line, currentnumber = "";
	std::ifstream inputfile(filename);
	if(inputfile.is_open()){
		while(getline (inputfile, line)){
			if(line == "end"){
				x = 0;
				y = 0;
				ithdone = true;
			}
			else{
				for(int i = 0; i < line.length(); i++){
					//end of current number
					if(line[i] == ' '){
						if(!ithdone)
							inputith[x][y] = std::stof(currentnumber);
						else
							inputhto[x][y] = std::stof(currentnumber);
						y++;
						currentnumber = "";
					}	
					else{
						currentnumber += line[i];
					}
				}
				x++;
				y = 0;
			}

		}
		for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
			robots[i].copyith(inputith);
			robots[i].copyhto(inputhto);
		}
	}
	else 
		std::cout<<"can't open file"<<std::endl;
}

float world::getaveragefitness(){
	float avaragefitness = 0;
	for(auto x : robots){
		avaragefitness += x.returnfitness();
	}
	return avaragefitness / nrobots;
}

float world::getmaxfitness(){
	//fitness can be negative so the number needs to be big and negative
	float max = -100000;
	for(auto x : robots){
		if(x.returnfitness() > max)
			max = x.returnfitness();
	}
	return max;
}

//the functions below are used in a situation with only 1 robot or identical robots so 0 is hardcoded
void world::getith(float input[MAX][MAX]){
	robots[0].returnith(input);
}

//the functions below are used in a situation with only 1 robot or identical robots so 0 is hardcoded
void world::gethto(float input[MAX][MAX]){
	robots[0].returnhto(input);
}

void world::newith(float input[MAX][MAX]){
	robots[0].newith(input);
}

void world::newhto(float input[MAX][MAX]){
	robots[0].newhto(input);
}

void world::clonerobots(){
	float ith[MAX][MAX];
	float hto[MAX][MAX];
	//copy robot 0's nodes
	robots[0].returnith(ith);
	robots[0].returnhto(hto);
	//give his nodes to all other robots
	for(std::vector<robot>::size_type i = 1; i != robots.size(); i++) {
		robots[i].copyith(ith);
		robots[i].copyhto(hto);
	}
}

float world::getworldfitness(){
	float totalfitness = 0;
	for(auto x : robots){
		totalfitness += x.returnfitness();
	}
	return totalfitness;
}
