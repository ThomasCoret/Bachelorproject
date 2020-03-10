#ifndef ROBOT_H
#define ROBOT_H

#include <time.h> 
#include <stdlib.h> 

#define MAX 20

class robot{
	
	//variables
	private:
		float input[MAX];
		float inputtohidden[MAX][MAX];
		float hiddentooutput[MAX];
		float inhidden[MAX];
		float acthidden[MAX];
		float inoutput;
		float netoutput;
		int inputs;
		int hiddenlayers;
		int worldwidth; 
		int worldheight;

	public:
		//position of robot
		float x, y;
		//rotation of the robot 0-7; 0 upwards; 2 to the right; 4 downwards; 6 to the left
		int rotation;
		//current speed of the robot
		float speed;
		//radius in which the robot is able to see food;
		float radius;
		//radius in which the robot is able to pick up food
		float grabradius;
		//amount of food collected
		int foodcollected;
		
	//functionS
	private:
		void neuralnetwork();

	public:
		robot();
		robot(float, float, int);
		void simulate(bool);
		
};
#endif