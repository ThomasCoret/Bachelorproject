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
		//radius in which the robot is able to pick up food
		float radius;
	//functionS
	private:


	public:
		robot();
		robot(float, float, int);
		void simulate(bool);
		void neuralnetwork();
};
#endif