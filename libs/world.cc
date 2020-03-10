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
	for(auto i : robots){
		bool foodaheadbool = foodahead(i);
		i.simulate(foodaheadbool);
		moverobot(i);
	}
	frames++;
}

bool world::foodahead(robot i){
	bool foodaheadbool = false;
	//check if any food is ahead of the robot
	for(auto j : foods){
		//float diagonaldist = sqrt(i.radius/2);
		switch (i.rotation){
			//robot is looking 'up'
			case 0:
				foodaheadbool = foodaheadbool || ((j.x>i.x-i.radius/2&&j.x<i.x+i.radius/2)&&(j.y>i.y&&j.y<i.y+i.radius));
				break;
			//right and up
			case 1:
				foodaheadbool = foodaheadbool || ((j.x >i.x&&j.x < i.x+i.radius)&&(j.y>i.y&&j.y<i.y+i.radius));
				break;
			//right
			case 2:
				foodaheadbool = foodaheadbool || ((j.y>i.y-i.radius/2&&j.y<i.y+i.radius/2)&&(j.x>i.x&&j.x<i.x+i.radius));
				break;
			//right and down
			case 3:
				foodaheadbool = foodaheadbool || ((j.x <i.x&&j.x > i.x-i.radius)&&(j.y>i.y&&j.y<i.y+i.radius));
				break;
			//down
			case 4:
				foodaheadbool = foodaheadbool || ((j.x>i.x-i.radius/2&&j.x<i.x+i.radius/2)&&(j.y<i.y&&j.y>i.y-i.radius));
				break;
			//down and left
			case 5:
				foodaheadbool = foodaheadbool || ((j.x <i.x&&j.x > i.x-i.radius)&&(j.y<i.y&&j.y>i.y-i.radius));
				break;
			//left
			case 6:
				foodaheadbool = foodaheadbool || ((j.y<i.y-i.radius/2&&j.y>i.y+i.radius/2)&&(j.x<i.x&&j.x>i.x-i.radius));
				break;
			//up and left
			case 7:
				foodaheadbool = foodaheadbool || ((j.x >i.x&&j.x < i.x+i.radius)&&(j.y<i.y&&j.y>i.y-i.radius));
				break;

		}
	}
	return foodaheadbool;
}

void world::moverobot(robot i){
	switch(i.rotation){
		//robot is looking 'up'
			case 0:
				i.y += i.speed;
				i.y > height ? i.y = height : i.y = i.y;
				break;
			//right and up
			case 1:
				i.y += i.speed/2;
				i.x += i.speed/2;
				i.y > height ? i.y = height : i.y = i.y;
				i.x > width ? i.x = width : i.x = i.x;
				break;
			//right
			case 2:
				i.x += i.speed;
				i.x > width ? i.x = width : i.x = i.x;
				break;
			//right and down
			case 3:
				i.y -= i.speed/2;
				i.x += i.speed/2;
				i.y < 0 ? i.y = 0 : i.y = i.y;
				i.x > width ? i.x = width : i.x = i.x;
				break;
			//down
			case 4:
				i.y -= i.speed;
				i.y < 0 ? i.y = 0 : i.y = i.y;
				break;
			//down and left
			case 5:
				i.y -= i.speed/2;
				i.x -= i.speed/2;
				i.y < 0 ? i.y = 0 : i.y = i.y;
				i.x < 0 ? i.x = 0 : i.x = i.x;
				break;
			//left
			case 6:
				i.x -= i.speed;
				i.x < 0 ? i.x = 0 : i.x = i.x;
				break;
			//up and left
			case 7:
				i.y += i.speed/2;
				i.x -= i.speed/2;
				i.y > height ? i.y = height : i.y = i.y;
				i.x < 0 ? i.x = 0 : i.x = i.x;
				break;
	}
}

void world::drawworld(){
	/*
	for(auto i:robots){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = 1;
		std::cout<<"robot x: "<<i.x<<", y= "<<i.y<<std::endl;
	}
	for(auto i:foods){
		worlddraw[(int)round(i.x)][(int)round(i.y)] = 2;
		std::cout<<"food x: "<<i.x<<", y= "<<i.y<<std::endl;
	}
	*/
	for(int i =0; i < width + 1; i++){
		for (int j = 0; j < height + 1; j++){
			std::cout<<worlddraw[i][j];
		}
		std::cout<<std::endl;
	} 

	//reset the worlddraw
	for(int i = 0; i < width +1; i++){
		for(int j = 0; j < height + 1; j++){
			worlddraw[i][j]= 0;
		}
	}
}