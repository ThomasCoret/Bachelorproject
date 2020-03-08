#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <stdlib.h> 
#include <time.h> 
#include <math.h>
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

	//functionS
	private:
		bool foodahead(robot temprobot);
		
	public:
		world();
		bool done();
		void simulate();


};
#endif