#include "world.h"

#define fitnessgainfood 100
#define nrays 30
#define raylength 20
#define raystepsize 0.1
#define ROT 360.0
#define FOV 180.0
#define NFOVDISTR 3

world::world(){
	frames = 0;
	width = 50;
	height = 50;
	//root of width squared + height squared
	maxdistance = (float)sqrt(pow(width,2) + pow(height,2));
	nrobots = 1;
	maxfood = 10;
	nfood = maxfood;
	//random number generating
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,width*10); // distribution in range [0, width]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (0,height*10); // distribution in range [0, height]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT*10); // distribution in range [0, ]

	currentmaxfitness = 0;
	currentaveragefitness = 0;

	for(int i = 0; i < nrobots; i++){
		//spawn robot in a random location
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		//spawn robot in the middle of the map
		//float newx = width/2;
		//float newy = height/2;
		float newrot = ((float)rotdist (rng))/10;
		robot Newrobot(newx, newy, newrot, i);
		robots.push_back(Newrobot);
	}
	for(int i = 0; i < nfood; i++){
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		food Newfood(newx, newy, i);
		foods.push_back(Newfood);
	}
}

void world::randomizeworld(){
	frames = 0;
	//random number generating
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,width*10); // distribution in range [0, width*10]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (0,height*10); // distribution in range [0, height*10]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist    (0,ROT*10); // distribution in range [0, ROT*10]
	//remove the food
	foods.clear();
	//put new food back 
	for(int i = 0; i < maxfood; i++){
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;

		food Newfood(newx, newy, i);
		foods.push_back(Newfood);
	}
	//randomize the robots locations
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		//spawn robot in a random location
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		//spawn robot in the middle of the map
		//float newx = width/2;
		//float newy = height/2;
		float newrot = ((float)rotdist (rng))/10;
		robots[i].rotation = newrot;
		robots[i].x = newx;
		robots[i].y = newy;
		robots[i].fitness = 0;
	}
	nfood = maxfood;
}

bool world::done(){
	return (nfood<1);
}

void world::simulate(){
	for(std::vector<robot>::size_type i = 0; i < robots.size(); i++) {
		checkfoodcollision(i);
		float collisions[NFOVDISTR];
		for(int j =0 ; j<NFOVDISTR; j++){
			collisions[j] = foodahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation, j);
		}
		float distance[3];
		//reverse and normalize the distance to the wall so 0 is far away from the wall and 1 is close to the wall
		distance[0] = (maxdistance - distancetowall(robots[i].x, robots[i].y, robots[i].rotation) ) / maxdistance;
		distance[1] = (maxdistance - distancetowall(robots[i].x, robots[i].y, robots[i].rotation - 90.0) ) / maxdistance;
		distance[2] = (maxdistance - distancetowall(robots[i].x, robots[i].y, robots[i].rotation + 90.0) ) / maxdistance;
		robots[i].simulate(collisions[0], collisions[1], collisions[2], distance[0], distance[1], distance[2]);
		moverobot(i);
		//every step we substract one from the fitness so the robots that collect all food fastest get favoured
		robots[i].fitness -= 1;
	}
	frames++;
}

//works the same as a single ray. 
float world::distancetowall(float x, float y, float degree){
	float originalx = x;
	float originaly = y;
	//robots can be exactly on the edge so extend the edges 
	while(x > -0.1 && x < width+0.1 && y > -0.1 && y < height + 0.1){
		x += raystepsize * cos(degree*M_PI/180);
		y += raystepsize * sin(degree*M_PI/180);
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
		float ray = castray(x,y,rotation+i*(FOV/NFOVDISTR/nrays));
		//add the intensity of this ray divided by the total number of rays (if all rays hit intensity will be 1 (not taking in account the distance of the food))
		intensity += (float)ray/nrays;
	}
	return intensity;
}

float world::castray(float x, float y, float degree){
	
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
				//don return length just that it has seen food
				return 1.0;
				//return length to food+
				//return (float)sqrt(pow(abs(originalx - fx),2) + pow(abs(originaly - fy),2)) / raylength;
			}
		}
		x += raystepsize * cos(degree*M_PI/180);
		y += raystepsize * sin(degree*M_PI/180);
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
	robots[i].fitness += sqrt(pow(abs(robots[i].x - robotx),2) + pow(abs(robots[i].y - roboty),2));
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
			robots[i].fitness += fitnessgainfood;
			foods.erase(foods.begin() + j);
			nfood--;
			//subtract 1 from iterator since we erase the current food
			j--;
		}
	}
}

void world::updaterobots(float adapt){
	int maxfitness = -9000000;
	std::vector<robot>::size_type bestrobot = -1;
	float bestinputtohidden[MAX][MAX];
	float besthiddentooutput[MAX][MAX];
	float avaragefitness = 0;
	//find the most succesfull robot
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		std::cout<<robotchar(robots[i].nrobot)<<": "<<robots[i].fitness<<std::endl;
		avaragefitness += robots[i].fitness;
		if(robots[i].fitness > maxfitness){
			maxfitness = robots[i].fitness;
			bestrobot = i;
		}
	}
	std::cout<<"best robot: "<<robotchar(robots[bestrobot].nrobot)<<", fitness: "<<robots[bestrobot].fitness<<std::endl;
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
	}
	currentaveragefitness = avaragefitness / nrobots;
	currentmaxfitness = maxfitness;
}

void world::drawworld(){
	char worlddraw[width+1][height+1];

		for(int i = 0; i < width + 1; i++){
		for(int j = 0; j < height + 1; j++){
			worlddraw[i][j]= ' ';
			if(i == 0 || i == width || j == 0 || j == height )
				worlddraw[i][j]= '0';
		}
	}

	/* //raycasting draw
	for(int i = 0 ; i<60;i++){
		float x = robots[0].x;
		float y = robots[0].y;

		while(x > 0 && x< width && y > 0 && y <height){
			x += raystepsize * cos((robots[0].rotation-90+(3*i))*M_PI/180);
			y += raystepsize * sin((robots[0].rotation-90+(3*i))*M_PI/180);
			worlddraw[(int)round(x)][(int)round(y)] = 'r'; 
		}
	}
	*/

	for(auto i : robots){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = robotchar(i.nrobot);
		//std::cout<<"robot x: "<<i.x<<", y= "<<i.y<<", rot: "<<i.rotation<<std::endl;
	}
	for(auto i : foods){
			worlddraw[(int)round(i.x)][(int)round(i.y)] = '@';
	}
	
	for (int j = height; j >= 0 ; j--){
		for(int i =0; i < width + 1; i++){
			std::cout<<worlddraw[i][j]<<" ";
		}
		std::cout<<std::endl;
	} 
	
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
		if(robots[i].fitness > maxfitness){
			maxfitness = robots[i].fitness;
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
	}
	else std::cout<<"can't open file"<<std::endl;

	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		robots[i].copyith(inputith);
		robots[i].copyhto(inputhto);
	}
}

float world::getaveragefitness(){
	float avaragefitness = 0;
	for(auto x : robots){
		avaragefitness += x.fitness;
	}
	return avaragefitness / nrobots;
}

float world::getmaxfitness(){
	float max = -100000;
	for(auto x : robots){
		if(x.fitness > max)
			max = x.fitness;
	}
	return max;
}

//the functions below are used in a situation with only 1 robot so 0 is hardcoded
void world::getith(float input[MAX][MAX]){
	robots[0].returnith(input);
	
}

void world::gethto(float input[MAX][MAX]){
	robots[0].returnhto(input);
}

void world::newith(float input[MAX][MAX]){
	robots[0].newith(input);
}

void world::newhto(float input[MAX][MAX]){
	robots[0].newhto(input);
}