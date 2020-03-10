#include "robot.h"

//redundant
robot::robot(){
	x = 0.0;
	y = 0.0; 
}

robot::robot(float _x, float _y, int _rotation){
	srand (time(NULL));
	x = _x;
	y = _y;
	rotation = _rotation;
	radius = 5.0;
	grabradius = 2.0;
 	speed = 1.0;
	inputs = 2;
	hiddenlayers = 4;
	foodcollected = 0;

	for (int i = 0; i < MAX; i++){
		//initiate hiddenlayers with random values [0-1]
		hiddentooutput[i] = (float)((rand()%1000)-500)/500;
		for (int j = 0; j < MAX; j++){
			inputtohidden[i][j] = (float)((rand()%1000)-500)/500;
		}
	}

	for(int i = 0; i < MAX; i++){
		input[i] = 0;
	}
}

//simulate the robot, 
void robot::simulate(bool foodahead){
	//first input for the robot is whether he has foodahead in his current rotation
	input[0] = foodahead;
	//second input is the current rotation of the robot
	input[1] = rotation;
	neuralnetwork();
}

void robot::neuralnetwork(){
	for (int i = 0;i < MAX; i++){
		inhidden[i]=0;
	}
	inoutput = 0;
}