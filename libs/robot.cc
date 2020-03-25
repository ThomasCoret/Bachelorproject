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
	maxspeed = 0.5;
 	speed = 1.0;
 	turnspeed = 45.0;
	inputs = 6;
	outputs = 2;
	hiddenlayers = 4;
	fitness = 0;
	startlearningrate = 0.2;
	endlearningrate = 0.01;
	learningrate = startlearningrate;
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
void robot::simulate(float foodahead, float foodtotheright, float foodtotheleft, float distahead,float distleft,float distright){
	//setup the inputs for the neural network
	//first input for the robot is the intensity of food in the 0-60 degrees of the robots fov
	input[1] = foodtotheleft;
	//first input for the robot is the intensity of food in the 60-120 degrees of the robots fov
	input[2] = foodahead;
	//first input for the robot is the intensity of food in the 120-180 degrees of the robots fov
	input[3] = foodtotheright;
	//distance to top wall
	input[4] = distahead;
	//distance to left wall
	input[5] = distleft;
	//distance to right wall
	input[6] = distright;
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
	//netoutput decides where to turn [-turnspeed,turnspeed]
	rotation += (netoutput[0] - 0.5) * turnspeed*2;
	//netoutput 1 decides the speed [0, maxspeed]
	speed = netoutput[1] * maxspeed; 
	//fix the rotation if it got too big or below 0
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
			//add small variation to the recieved genes relative to the weight [-0.05% - 0.05%]
			inputtohidden[i][j] = inputarray[i][j] + inputarray[i][j] * (((float)widthdist (rng))-500)/500 * learningrate;
			//std::cout<<"variatie: "<<inputarray[i][j] * (((float)widthdist (rng))-500)/500 * learningrate<<std::endl;
		}
	}
}

void robot::newhto(float inputarray[MAX][MAX]){
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,1000);

	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			//add small variation to the recieved genes relative to the weight [-0.05% - 0.05%]
			hiddentooutput[i][j] = inputarray[i][j] + inputarray[i][j] * (((float)widthdist (rng))-500)/500 * learningrate;
		}
	}
}

void robot::copyith(float inputarray[MAX][MAX]){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			inputtohidden[i][j] = inputarray[i][j];
		}
	}
}

void robot::copyhto(float inputarray[MAX][MAX]){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			hiddentooutput[i][j] = inputarray[i][j];
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
				inputtohidden[k][j] += learningrate * input[j] * deltahidden[i][j];
			}
		}

		for(int j = 0; j < hiddenlayers+1; j++){
			hiddentooutput[i][j] += learningrate * acthidden[j] * delta[i];
		}
	}
}

void robot::adjustlearningrate(float adapt){
	float totalshift = startlearningrate - endlearningrate;
	learningrate -= adapt * totalshift;
}

void robot::savenodes(std::string filename){
	std::ofstream outputfile;
	outputfile.open(filename);

	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			outputfile<<inputtohidden[i][j]<<" ";
		}
		outputfile<<"\n";
	}
	outputfile<<"end\n";
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			outputfile<<hiddentooutput[i][j]<<" ";
		}
		outputfile<<"\n";
	}


	outputfile.close();
}