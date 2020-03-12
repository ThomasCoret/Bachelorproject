#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <math.h>
#include <iostream>

#include "food.h"
#include "robot.h"

#define ROT 8

class world{
	
	//variables
	private:
		
	public:
		int frames;
		int width, height;
		int nrobots, nfood, maxfood;
		std::vector<robot> robots;
		std::vector<food> foods;

	//functionS
	private:
		bool foodahead(float, float, float, int);
		
	public:
		world();
		void randomizeworld();
		bool done();
		void simulate();
		void moverobot(std::vector<robot>::size_type);
		void checkfoodcollision(std::vector<robot>::size_type);
		void drawworld();
		void updaterobots();
};
#endif