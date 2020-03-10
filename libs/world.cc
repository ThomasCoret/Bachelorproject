#include "world.h"


world::world(){
	frames = 0;
	width = 100;
	height = 100;
	nrobots = 10;
	nfood = 10;
	for(int i = 0; i < nrobots; i++){
		//for some reason after 1 iteration rand stays the same unless you call it multiple times however in the next while loop rand is different so im confused but this works
		for(int j = 0; j < i; j++)
			rand();
		float newx = ((float)(rand() % (width*10)))/10;
		float newy = ((float)(rand() % (height*10)))/10;
		//std::cout<<"rand: "<<rand()<<", newx: "<<newx<<", newy: "<<newy<<std::endl;
		robot Newrobot(newx, newy, rand() % 8);
		robots.push_back(Newrobot);
	}
	for(int i =0;i<nfood;i++){
		float newx = ((float)(rand()% (width*10)))/10;
		float newy = ((float)(rand() % (height*10)))/10;
		food Newfood(newx, newy);
		foods.push_back(Newfood);
	}
	for(int i = 0; i < width+1; i++){
		for(int j = 0; j < height+1; j++){
			worlddraw[i][j]= 0;
		}
	}
}

bool world::done(){
	return (nfood==0);
}

void world::simulate(){
	for(std::vector<robot>::size_type i = 0; i != robots.size(); i++) {
		bool foodaheadbool = foodahead(i);
		robots[i].simulate(foodaheadbool);
		moverobot(i);
	}
	frames++;
}

bool world::foodahead(std::vector<robot>::size_type i){
	bool foodaheadbool = false;
	float x = robots[i].x;
	float y = robots[i].y;
	float radius = robots[i].radius;
	//check if any food is ahead of the robot
	for(auto j : foods){
		//float diagonaldist = sqrt(i.radius/2);
		switch (robots[i].rotation){
			//robot is looking 'up'
			case 0:
				foodaheadbool = foodaheadbool || ((j.x> x-radius/2 && j.x < x+radius/2) && (j.y > y && j.y < y+radius));
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
}

void world::drawworld(){
	
	for(auto i:robots){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = 1;
		std::cout<<"robot x: "<<i.x<<", y= "<<i.y<<", rot: "<<i.rotation<<std::endl;
	}
	for(auto i:foods){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = 2;
		std::cout<<"food x: "<<i.x<<", y= "<<i.y<<std::endl;
	}
	/*
	for(int i =0; i < width + 1; i++){
		for (int j = 0; j < height + 1; j++){
			std::cout<<worlddraw[i][j];
		}
		std::cout<<std::endl;
	} 
	*/
	//reset the worlddraw
	for(int i = 0; i < width +1; i++){
		for(int j = 0; j < height + 1; j++){
			worlddraw[i][j]= 0;
		}
	}
}