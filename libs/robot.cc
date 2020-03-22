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
	radius = 20.0;
	grabradius = 3.0;
 	speed = 0.5;
	inputs = 7;
	outputs = 3;
	hiddenlayers = 4;
	fitness = 0;
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

float robot::g(float x){
	return 1 / ( 1 + exp(-1*x) );
}

float robot::gprime(float x ){
	return 1 * g(x) * ( 1 - g(x) );
}

//simulate the robot, 
void robot::simulate(float foodahead, float foodtotheright, float foodtotheleft, float disttop,float distleft,float distright,float distbot){
	//setup the inputs for the neural network
	//first input for the robot is the intensity of food in the 0-60 degrees of the robots fov
	input[1] = foodtotheleft;
	//first input for the robot is the intensity of food in the 60-120 degrees of the robots fov
	input[2] = foodahead;
	//first input for the robot is the intensity of food in the 120-180 degrees of the robots fov
	input[3] = foodtotheright;
	//distance to top wall
	input[4] = disttop;
	//distance to left wall
	input[5] = distleft;
	//distance to right wall
	input[6] = distright;
	//distance to bottom wall
	input[7] = distbot;
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
		acthidden[i] = g(inhidden[i]);
	}

	for(int i = 0; i < outputs; i++){
		for(int j = 0; j < hiddenlayers + 1; j++){
			inoutput[i] += acthidden[i] * hiddentooutput[i][j];
		}
	}

	for(int i =0;i<outputs;i++)
		netoutput[i] = g(inoutput[i]);

	//update stats based on output
	//netoutput 0 decides whether to turn right
	rotation += netoutput[0] > 0.5 ? 1 : 0;
	//netoutput 1 decides whether to turn left
	rotation -= netoutput[1] > 0.5 ? 1 : 0;
	//netoutput 2 decides whether to move or not
	//speed = netoutput[2] > 0.5 ? 0.1 : 0; 
	rotation = fixrotation(rotation);
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
			//add small variation to the recieved genes [-0.05-0.05]
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
			//add small variation to the recieved genes
			hiddentooutput[i][j] = inputarray[i][j] + (((float)widthdist (rng))-500)/500 * alpha;
		}
	}
}
float robot::fixrotation(float rotation){
	if(rotation < 0)
		return 360 + rotation;
	if(rotation > 360)
		return rotation-360;
	return rotation;
}

void robot::qlearn(float ahead, float right, float left){
	//calculate the error for each output
	float error[outputs];
	float delta[outputs];
	float deltahidden[outputs][hiddenlayers];
	//if food to the right of the robot he should turn right
	error[0] = right ? 1 - netoutput[0] : 0;
	//if food to the left of the robot he should turn left
	error[1] = left ? 1 - netoutput[1] : 0;
	//robot should always be moving since it is the most efficient
	error[2] = netoutput[2] > 0.5 ? 0 : 1 - netoutput[2];
	for(int i = 0; i<outputs; i++){
		delta[i] = error[i] * gprime(inoutput[i]);
		for(int j = 1; j<hiddenlayers+1; j++){
			deltahidden[i][j] = gprime(inoutput[i]) * hiddentooutput[i][j] * delta[i];
		}
	}

	//update the weights for each output delta
	for(int i = 0; i < outputs; i++){
		for(int j = 0; j < hiddenlayers+1; j++){
			for(int k = 0; k < inputs; k++){
				inputtohidden[k][j] += alpha * input[j] * deltahidden[i][j];
			}
		}

		for(int j = 0; j < hiddenlayers+1; j++){
			hiddentooutput[i][j] += alpha * acthidden[j] * delta[i];
		}
	}
}