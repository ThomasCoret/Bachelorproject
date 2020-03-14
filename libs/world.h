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
		bool generational;

	public:
		int frames;
		int width, height;
		int nrobots, nfood, maxfood;
		std::vector<robot> robots;
		std::vector<food> foods;

	//functionS
	private:
		//check if there is food ahead of the given parameters
		bool foodahead(float, float, float, int);
		//return a char based on a robots rotation (for drawworld)
		char robotchar(int rotation);
		
	public:
		world(bool);
		//reset randomize the world 
		void randomizeworld();
		//world is done when no food is left
		bool done();
		//simulate all the robots behiour
		void simulate();
		//move the robots 
		void moverobot(std::vector<robot>::size_type);
		//check if robots are colliding with food
		void checkfoodcollision(std::vector<robot>::size_type);
		//update the robots based on a generational algorithm
		void updaterobots();
		//draw the current world in the console
		void drawworld();
		
};
#endif