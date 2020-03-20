#include "world.h"

#define fitnessgainfood 10
#define nrays 10
#define raywidth 0.3
#define raylength 10
#define raystepsize 0.1
#define ROT 360.0

world::world(bool _generational){
	generational = _generational;
	frames = 0;
	width = 20;
	height = 20;
	if(generational)
		nrobots = 1;
	else
		nrobots = 1;
	maxfood = 10;
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
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT); // distribution in range [0, 7]
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
		robots[i].x = newx;
		robots[i].y = newy;
		robots[i].fitness = 0;
	}
	nfood = 10;
}

bool world::done(){
	return (nfood<1);
}

void world::simulate(){
	for(std::vector<robot>::size_type i = 0; i < robots.size(); i++) {
		checkfoodcollision(i);
		float collisions[3];
		for(int j =0 ; j<3; j++){
			collisions[j] = foodahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation, j);
		}
		robots[i].simulate(collisions[0], collisions[1], collisions[2]);
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

	for(int i = 0; i<nrays; i++){
		intensity += (1/nrays) * castray(x,y,rotation+i*(60/nrays));
	}

	return intensity;
}

float world::castray(float x, float y, float degree){
	char worlddraw[width+1][height+1];

	for(int i = 0; i < width + 1; i++){
		for(int j = 0; j < height + 1; j++){
			worlddraw[i][j]= '0';
		}
	}
	float originalx = x;
	float originaly = y;
	float xslope = xcomputeslope(degree);
	float yslope = ycomputeslope(degree);
	for(float i = 0; i < raylength; i += raystepsize){
		worlddraw[(int)round(x)][(int)round(y)] = 'r';
		for(int i =0; i < width + 1; i++){
			for (int j = 0; j < height + 1; j++){
				std::cout<<worlddraw[i][j]<<" ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;
		for(std::vector<food>::size_type j = 0; j < foods.size(); j++) {
			float fx = foods[j].x;
			float fy = foods[j].y;
			//collision with food
			if((fx > x - raywidth) && (fx < x + raywidth) && (fy > y - raywidth) && (fy < y + raywidth)){
				//return length to food
				return abs(originalx - fx) + abs(originaly - fy);
			}
			x+= raystepsize * xslope;
			y+= raystepsize * yslope;
		}
	}
	return 0;
}

float world::xcomputeslope(float degree){
	if(degree < 0.1 || degree > 359.9){
		return 0;
	}
	if(degree > 179.9 && degree < 180.1)
		return 0;
	if(degree > 89.9 && degree <90.1)
		return 1;
	if(degree >269.9 && degree <270.1)
		return -1;
	return (cos(degree)/sin(degree));

}

float world::ycomputeslope(float degree){
	if(degree < 0.1 || degree > 359.9){
		return 1;
	}
	if(degree > 179.9 && degree < 180.1)
		return -1;
	if(degree > 89.9 && degree <90.1)
		return 0;
	if(degree >269.9 && degree <270.1)
		return 0;
	return (sin(degree)/cos(degree));
}

void world::moverobot(std::vector<robot>::size_type i){
	float robotx = robots[i].x;
	float roboty = robots[i].y;
	float xslope = xcomputeslope(robots[i].rotation);
	float yslope = ycomputeslope(robots[i].rotation);
	robots[i].x += robots[i].speed * xslope;
	robots[i].y += robots[i].speed * yslope;
	std::cout<<std::endl;
	//robots gain fitness for the amount they moved
	robots[i].fitness += abs(robots[i].x - robotx) + abs(robots[i].y - roboty);
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
		std::cout<<"x.nrobot: "<<x.fitness<<std::endl;
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
			worlddraw[i][j]= '0';
		}
	}

	for(auto i : robots){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = robotchar(i.rotation);
		//std::cout<<"robot x: "<<i.x<<", y= "<<i.y<<", rot: "<<i.rotation<<std::endl;
	}
	for(auto i : foods){
			worlddraw[(int)round(i.x)][(int)round(i.y)] = 'f';
	}
	
	for(int i =0; i < width + 1; i++){
		for (int j = 0; j < height + 1; j++){
			std::cout<<worlddraw[i][j]<<" ";
		}
		std::cout<<std::endl;
	} 
	
}

char world::robotchar(int rotation){
	switch(rotation){
		case 0:
		case 4:
			return '|';
		case 1:
		case 5:
			return '/';
		case 2:
		case 6:
			return '-';
		case 3:
		case 7:
			return '\\';
	}
}