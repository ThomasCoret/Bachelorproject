#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
#include <math.h>
#include <iostream>

#include "world.h"

class worldmanager{
	
	//variables
	public:
		float maxfitness;
		float averagefitness;
		std::vector<world>::size_type bestworld;	
		std::vector<world> Worlds;

	private:
		int nworlds;
		int generations;
		
		bool identicalrobots;

	//functions
	public:
		worldmanager(int, int, bool);
		//do one step for all the worlds
		void simulate();
		//vind the best world(s) and update the rest of the worlds with their nodes
		void update();
		//reset the fitness counters
		void resetfitness();
		//get ith of best world
		void getith(float[MAX][MAX]);
		//get hto of best world
		void gethto(float[MAX][MAX]);
		//randomize the robots	
		void randomizerobots();
};

#endif
