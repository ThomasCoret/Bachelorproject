#include "robot.h"

//redundant
robot::robot(){
	x = 0.0;
	y = 0.0; 
}

robot::robot(float _x, float _y, int _rotation, int _nrobot){
	srand (time(NULL));
	x = _x;
	y = _y;
	nrobot = _nrobot;
	rotation = _rotation;
	radius = 5.0;
	grabradius = 2.0;
 	speed = 1.0;
	inputs = 4;
	outputs = 3;
	hiddenlayers = 4;
	foodcollected = 0;
	alpha = 0.1;
	//bias
	input[0] = -1;
	//bias
	acthidden[0] = -1;

	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,1000); // distribution in range [0, width]

	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			//initiate hiddenlayers with random values [-1:1]
			inputtohidden[i][j] = (((float)widthdist (rng))-500)/500;
			hiddentooutput[i][j] = (((float)widthdist (rng))-500)/500;
		}
	}

	for(int i = 0; i < MAX; i++){
		input[i] = 0;
	}
}

float robot::r(float x){
	return x > 0 ? x : 0;
}

float robot::rprime(float x ){
	return x > 0 ? 1 : 0;
}

//simulate the robot, 
void robot::simulate(bool foodahead, bool foodtotheright, bool foodtotheleft){
	//setup the inputs for the neural network
	//first input for the robot is whether he has foodahead in his current rotation
	input[1] = foodahead;
	//second input is the current rotation of the robot
	input[2] = rotation;
	input[3] = foodtotheright;
	input[4] = foodtotheleft;
	neuralnetwork();
}

void robot::neuralnetwork(){
	for (int i = 0;i < MAX; i++){
		inhidden[i] = 0;
		inoutput[i] = 0;
	}
	
	//add the weighted inputs to the input for each hidden node
	for(int i = 1; i < hiddenlayers + 1; i++){
		for(int j = 0; j < inputs + 1; j++){
			inhidden[i] += input[j] * inputtohidden[i][j];
		}
	}

	for(int i = 1; i < hiddenlayers + 1; i++){
		acthidden[i] = r(inhidden[i]);
	}

	for(int i = 0; i < outputs; i++){
		for(int j = 0; j < hiddenlayers + 1; j++){
			inoutput[i] += acthidden[i] * hiddentooutput[i][j];
		}
	}

	for(int i =0;i<outputs;i++)
		netoutput[i] = r(inoutput[i]);
	//update stats based on output
	rotation += (int)netoutput[0];
	rotation -= (int) netoutput[1];
	speed = netoutput[2]; 
}

void robot::returnith(float inputarray[MAX][MAX]){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			inputarray[i][j] = inputtohidden[i][j];
		}
	}
} 

void robot::returnhto(float inputarray[MAX][MAX]){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			inputarray[i][j] = hiddentooutput[i][j];
		}
	}
} 

void robot::newith(float inputarray[MAX][MAX]){
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,1000);

	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			inputtohidden[i][j] = inputarray[i][j] + (((float)widthdist (rng))-500)/500 * alpha;
		}
	}
}

void robot::newhto(float inputarray[MAX][MAX]){
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,10);

	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			hiddentooutput[i][j] = inputarray[i][j] + (((float)widthdist (rng))-500)/500 * alpha;
		}
	}
}