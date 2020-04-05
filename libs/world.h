#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <math.h>
#include <iostream>

#include "food.h"
#include "robot.h"

class world{
	
	//variables
	public:
		int frames;
		int width, height;
		int nrobots, nfood, maxfood;
		std::vector<robot> robots;
		std::vector<food> foods;
		float currentmaxfitness;
		float currentaveragefitness;
		float maxdistance;

	//functions
	private:
		//check if there is food ahead of the given parameters
		float foodahead(float, float, float, float, int);
		//return a char based on a robots rotation (for drawworld)
		char robotchar(int);
		//cast a ray in the direction of the rotation from the passed location
		float castray(float, float, float);
		//distance to wall in the direction of the rotation
		float distancetowall(float, float, float);
		
	public:
		world();
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
		void updaterobots(float);
		//draw the current world in the console
		void drawworld();
		//save the nodes of the best robot to a file
		void savebestrobot(std::string);
		//load robot from .bot file
		void loadrobot(std::string);
		//get the average fitness of the current robots
		float getaveragefitness();
		//get the max fitness of the current robots
		float getmaxfitness();
		void getith(float[MAX][MAX]);
		void gethto(float[MAX][MAX]);
		void newith(float[MAX][MAX]);
		void newhto(float[MAX][MAX]);
};
#endif