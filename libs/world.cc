#include "world.h"


world::world(){
	srand (time(NULL));
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
		i.simulate();
	}
}