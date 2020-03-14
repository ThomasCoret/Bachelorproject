#include "world.h"

#define fitnessgainfood 10

world::world(bool _generational){
	generational = _generational;
	frames = 0;
	width = 20;
	height = 20;
	if(generational)
		nrobots = 10;
	else
		nrobots = 1;
	maxfood = 10;
	nfood = maxfood;
	//random number generating
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,width*10); // distribution in range [0, width]
	std::uniform_int_distribution<std::mt19937::result_type> heightdist (0,height*10); // distribution in range [0, height]
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT); // distribution in range [0, ]

	for(int i = 0; i < nrobots; i++){
		float newx = ((float)widthdist (rng))/10;
		float newy = ((float)heightdist(rng))/10;
		int newrot = widthdist(rng)%8;
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
	std::uniform_int_distribution<std::mt19937::result_type> rotdist (0,ROT); // distribution in range [0, ]
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
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		checkfoodcollision(i);
		bool foodaheadbool = foodahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation);
		bool foodtotheright = foodahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation + 1);
		bool foodtotheleft = foodahead(robots[i].x, robots[i].y, robots[i].radius, robots[i].rotation - 1);
		robots[i].simulate(foodaheadbool, foodtotheright, foodtotheleft);
		//reinforcement learning
		if(!generational){
			robots[i].qlearn(foodaheadbool, foodtotheright, foodtotheleft);
		}
		moverobot(i);
	}
	frames++;
}

bool world::foodahead(float x, float y, float radius, int rotation){
	bool foodaheadbool = false;
	rotation = robots[0].fixrotation(rotation);
	//check if any food is ahead of the robot
	for(auto j : foods){
		//float diagonaldist = sqrt(i.radius/2);
		switch (rotation){
			//robot is looking 'up'
			case 0:
				foodaheadbool = foodaheadbool || ((j.x > x-radius/2 && j.x < x+radius/2) && (j.y > y && j.y < y+radius));
				break;
			//right and up
			case 1:
				foodaheadbool = foodaheadbool || ((j.x > x && j.x < x+radius) && (j.y>y && j.y < y+radius));
				break;
			//right
			case 2:
				foodaheadbool = foodaheadbool || ((j.y > y-radius/2 && j.y < y+radius/2) && (j.x > x && j.x < x+radius));
				break;
			//right and down
			case 3:
				foodaheadbool = foodaheadbool || ((j.x < x && j.x > x-radius) && (j.y > y && j.y < y+radius));
				break;
			//down
			case 4:
				foodaheadbool = foodaheadbool || ((j.x > x-radius/2 && j.x < x+radius/2) && (j.y < y && j.y > y-radius));
				break;
			//down and left
			case 5:
				foodaheadbool = foodaheadbool || ((j.x < x && j.x > x-radius) && (j.y < y && j.y > y-radius));
				break;
			//left
			case 6:
				foodaheadbool = foodaheadbool || ((j.y < y-radius/2 && j.y > y+radius/2) && (j.x < x && j.x > x-radius));
				break;
			//up and left
			case 7:
				foodaheadbool = foodaheadbool || ((j.x > x && j.x < x+radius) && (j.y < y && j.y > y-radius));
				break;

		}
	}
	return foodaheadbool;
}

void world::moverobot(std::vector<robot>::size_type i){
	float robotx = robots[i].x;
	float roboty = robots[i].y;
	switch(robots[i].rotation){
		//robot is looking 'up'
			case 0:
				robots[i].y += robots[i].speed;
				if(robots[i].y > height) robots[i].y = height;
				break;
			//right and up
			case 1:
				robots[i].y += robots[i].speed/2;
				robots[i].x += robots[i].speed/2;
				if(robots[i].y > height) robots[i].y = height;
				if(robots[i].x > width)  robots[i].x = width;
				break;
			//right
			case 2:
				robots[i].x += robots[i].speed;
				if(robots[i].x > width) robots[i].x = width;
				break;
			//right and down
			case 3:
				robots[i].y -= robots[i].speed/2;
				robots[i].x += robots[i].speed/2;
				if(robots[i].y < 0)     robots[i].y = 0;
				if(robots[i].x > width) robots[i].x = width;
				break;
			//down
			case 4:
				robots[i].y -= robots[i].speed;
				if(robots[i].y < 0) robots[i].y = 0;
				break;
			//down and left
			case 5:
				robots[i].y -= robots[i].speed/2;
				robots[i].x -= robots[i].speed/2;
				if(robots[i].y < 0) robots[i].y = 0;
				if(robots[i].x < 0) robots[i].x = 0;
				break;
			//left
			case 6:
				robots[i].x -= robots[i].speed;
				if(robots[i].x < 0) robots[i].x = 0;
				break;
			//up and left
			case 7:
				robots[i].y += robots[i].speed/2;
				robots[i].x -= robots[i].speed/2;
				if(robots[i].y > height) robots[i].y = height;
				if(robots[i].x < 0)      robots[i].x = 0;
				break;
	}
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
		//if already robot in this place increment the number.
		//if(worlddraw[(int)round(i.x)][(int)round(i.y)] >1){
			worlddraw[(int)round(i.x)][(int)round(i.y)] = 'f';
		/*}
		else{
		worlddraw[(int)round(i.x)][(int)round(i.y)] = 2;
		//std::cout<<"food x: "<<i.x<<", y= "<<i.y<<std::endl;
		}*/
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
			return 'I';
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