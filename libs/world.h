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
		bool worlddone;
		float foodwidth;
		float robotwidth;
		bool social;

	//functions
	private:
		//check if there is food ahead of the given parameters
		float foodahead(float, float, float, float, int);
		//check if there is food ahead of the given parameters
		float robotahead(float, float, float, float, int, std::vector<robot>::size_type);
		//return a char based on a robots rotation (for drawworld)
		char robotchar(int);
		//cast a ray in the direction of the rotation from the passed location
		float castrayfood(float, float, float);
		//cast a ray in the direction of the rotation from the passed location
		float castrayrobot(float, float, float, std::vector<robot>::size_type);
		//distance to wall in the direction of the rotation
		float distancetowall(float, float, float);
		
	public:
		world();
		//reset randomize the world 
		void randomizeworld(int);
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
		//save the nodes of the best robot to a file
		void savebestrobot(std::string);
		//load robot from .bot file
		void loadrobot(std::string);
		//get the average fitness of the current robots
		float getaveragefitness();
		//get the max fitness of the current robots
		float getmaxfitness();
		//return the ith of robot 0 (used for only 1 robot or identical robots)
		void getith(float[MAX][MAX]);
		//return the hto of robot 0 (used for only 1 robot or identical robots)
		void gethto(float[MAX][MAX]);
		//give new ith to all the robots (the same mutation)
		void newith(float[MAX][MAX]);
		//give new hto to all the robots (the same mutation)
		void newhto(float[MAX][MAX]);
		//make the robots in the world the same
		void clonerobots();
		//return the totalfitness of this world
		float getworldfitness();
};
#endif
