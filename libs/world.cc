#include "world.h"


world::world(){
	srand (time(NULL));
	frames = 0;
	width = 100;
	height = 100;
	nrobots = 10;
	nfood = 10;
	for(int i =0;i<nrobots;i++){
		robot Newrobot(rand() % width, rand() % height, rand() % 360);
		robots.push_back(Newrobot);
	}
	for(int i =0;i<nfood;i++){
		food Newfood(rand() % width, rand() % height);
		foods.push_back(Newfood);
	}
}

bool world::done(){
	return (nfood==0);
}

void world::simulate(){
	for(auto i : robots){
		bool foodahead = false;
		
		i.simulate(foodahead);
	}
	frames++;
}

bool world::foodahead(robot i){
	bool foodaheadbool = false;
	//check if any food is ahead of the robot
	for(auto j : foods){
		//float diagonaldist = sqrt(i.radius/2);
		switch (i.rotation){
			//TODO: implement diagonal looking
			//robot is looking 'up'
			case 0:
				foodaheadbool = foodaheadbool || (j.x>i.x-i.radius/2&&j.x<i.x+i.radius/2)&&(j.y>i.y&&j.y<i.y+i.radius);
				break;
			//right and up
			case 1:
				
				break;
			//right
			case 2:
				foodaheadbool = foodaheadbool || (j.y>i.y-i.radius/2&&j.y<i.y+i.radius/2)&&(j.x>i.x&&j.x<i.x+i.radius);
				break;
			//right and down
			case 3:

				break;
			//down
			case 4:
				foodaheadbool = foodaheadbool || (j.x>i.x-i.radius/2&&j.x<i.x+i.radius/2)&&(j.y<i.y&&j.y>i.y-i.radius);
				break;
			//down and left
			case 5:

				break;
			//left
			case 6:
				foodaheadbool = foodaheadbool || (j.y<i.y-i.radius/2&&j.y>i.y+i.radius/2)&&(j.x<i.x&&j.x>i.x-i.radius);
				break;
			//up and left
			case 7:

				break;

		}
	}
	return foodaheadbool;
}