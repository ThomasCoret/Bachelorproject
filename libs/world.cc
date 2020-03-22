#include "world.h"

#define fitnessgainfood 30
#define nrays 30
#define raywidth 0.5
#define raylength 20
#define raystepsize 0.1
#define ROT 360.0
#define FOV 180
#define NFOVDISTR 3

world::world(bool _generational){
	generational = _generational;
	frames = 0;
	width = 100;
	height = 100;
	if(generational)
		nrobots = 10;
	else
		nrobots = 1;
	maxfood = 50;
	nfood = maxfood;
	//random number generating
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,width*10); // distribution in range [0, width]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (0,height*10); // distribution in range [0, height]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT*10); // distribution in range [0, ]

	for(int i = 0; i < nrobots; i++){
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
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
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,width*10); // distribution in range [0, width]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (0,height*10); // distribution in range [0, height]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT*10); // distribution in range [0, 7]
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
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
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
		float distance[4];
		distance[0] = height - robots[i].y;
		distance[1] = robots[i].x;
		distance[2] = width - robots[i].x;
		distance[3] = robots[i].y;
		robots[i].simulate(collisions[0], collisions[1], collisions[2], distance[0], distance[1], distance[2], distance[3]);
		//reinforcement learning
		if(!generational){
			robots[i].qlearn(collisions[0], collisions[1], collisions[2]);
		}
		moverobot(i);
	}
	frames++;
}

float world::foodahead(float x, float y, float radius, float rotation, int n){
	float intensity = 0;
	rotation -= FOV/2 + FOV/NFOVDISTR * n;
	for(int i = 0; i<nrays; i++){
		float ray = castray(x,y,rotation+i*(FOV/NFOVDISTR/nrays));
		intensity += (float)1.0/nrays * ray;
	}
	return intensity;
}

float world::castray(float x, float y, float degree){
	float originalx = x;
	float originaly = y;
	for(float i = 0; i < raylength; i += raystepsize){
		for(std::vector<food>::size_type j = 0; j < foods.size(); j++) {
			float fx = foods[j].x;
			float fy = foods[j].y;
			//collision with food
			if((fx > x - raywidth) && (fx < x + raywidth) && (fy > y - raywidth) && (fy < y + raywidth)){
				//return length to food
				return (float)sqrt(pow(abs(originalx - fx),2) + pow(abs(originaly - fy),2)) / raylength;
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

void world::updaterobots(){
	int maxfitness = -1;
	float bestinputtohidden[MAX][MAX];
	float besthiddentooutput[MAX][MAX];
	//find the most succesfull robot
	for(auto x : robots){
		std::cout<<robotchar(x.nrobot)<<": "<<x.fitness<<std::endl;
		if(x.fitness > maxfitness){
			maxfitness = x.fitness;
			//take best robots genes
			x.returnith(bestinputtohidden);
			x.returnhto(besthiddentooutput);
		}
	}
	//give best genes to other robots
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		robots[i].newith(bestinputtohidden);
		robots[i].newhto(besthiddentooutput);
	}
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

	for(auto i : robots){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = robotchar(i.nrobot);
		//std::cout<<"robot x: "<<i.x<<", y= "<<i.y<<", rot: "<<i.rotation<<std::endl;
	}
	for(auto i : foods){
			worlddraw[(int)round(i.x)][(int)round(i.y)] = '@';
	}
	
	for(int i =0; i < width + 1; i++){
		for (int j = 0; j < height + 1; j++){
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