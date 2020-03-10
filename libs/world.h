#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <math.h>
#include <iostream>
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
		bool foodahead(robot temprobot);
		
	public:
		world();
		bool done();
		void simulate();
		void moverobot(robot i);
		void drawworld();
};
#endif