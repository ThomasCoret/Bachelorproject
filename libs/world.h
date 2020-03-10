#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <math.h>
#include <iostream>
#include <random>
#include "food.h"
#include "robot.h"

class world{
	
	//variables
	private:
		
	public:
		int frames;
		int width, height;
		int nrobots, nfood;
		std::vector<robot> robots;
		std::vector<food> foods;
		int worlddraw[101][101];

	//functionS
	private:
		bool foodahead(std::vector<robot>::size_type);
		
	public:
		world();
		bool done();
		void simulate();
		void moverobot(std::vector<robot>::size_type);
		void checkfoodcollision(std::vector<robot>::size_type);
		void drawworld();
};
#endif