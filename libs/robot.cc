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
			//initiate hiddenlayers with random values [-4:4]
			inputtohidden[i][j] = (((float)widthdist (rng))-500)/500 * 4;
			hiddentooutput[i][j] = (((float)widthdist (rng))-500)/500 * 4;
		}
	}

	for(int i = 0; i < MAX; i++){
		input[i] = 0;
	}
}

float robot::activation(float x){
	return 1 / ( 1 + exp(-1*x) );
}

//simulate the robot, 
void robot::simulate(float foodtotheleft, float foodahead, float foodtotheright, float distahead,float distleft,float distright){
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
		acthidden[i] = activation(inhidden[i]);
	}

	for(int i = 0; i < outputs; i++){
		for(int j = 0; j < hiddenlayers + 1; j++){
			inoutput[i] += acthidden[j] * hiddentooutput[i][j];
		}
	}

	for(int i =0;i<outputs;i++){
		netoutput[i] = activation(inoutput[i]);
	}

	//std::cout<<"--------------------------"<<std::endl;
	//std::cout<<"before: speed: "<<speed<<", rotation: "<<rotation<<std::endl;
	//update stats based on output
	//netoutput decides where to turn [-turnspeed,turnspeed]
	rotation += (netoutput[0] - 0.5) * turnspeed*2;
	//std::cout<<netoutput[0]<<"  "<<(netoutput[0] - 0.5) * turnspeed*2<<std::endl;
	//netoutput 1 decides the speed [0, maxspeed]
	speed = netoutput[1] * maxspeed; 
	//fix the rotation if it got too big or below 0
	rotation = fixrotation(rotation);

	//std::cout<<"after: speed: "<<speed<<", rotation: "<<rotation<<std::endl;
	//std::cout<<"input1: "<<input[1]<<", input2: "<<input[2]<<", input3: "<<input[3]<<", wallahead: "<<input[4]<<", wallleft: "<<input[5]<<", wallright: "<<input[6]<<std::endl;
	//std::cout<<"inoutput1: "<<inoutput[0]<<", inoutput2: "<<inoutput[1]<<std::endl;
	//std::cout<<"output1: "<<netoutput[0]<<", output2: "<<netoutput[1]<<std::endl;
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
    //std::cout<<"before ith[1][1]"<<inputtohidden[1][1]<<", input: "<<inputarray[1][1]<<std::endl;
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			//add small variation to the recieved genes relative to the weight [-0.05% - 0.05%]
			inputtohidden[i][j] = inputarray[i][j] + inputarray[i][j] * (((float)widthdist (rng))-500)/500 * learningrate;

			//add small variation to the recieved genes not relative to the weight [-learningrate - learningrate]
			//inputtohidden[i][j] = inputarray[i][j] + (((float)widthdist (rng))-500)/500 * learningrate;
		}
	}
	//std::cout<<"after ith[1][1]"<<inputtohidden[1][1]<<std::endl;
}

void robot::newhto(float inputarray[MAX][MAX]){
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> widthdist  (0,1000);

	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			//add small variation to the recieved genes relative to the weight [-0.05% - 0.05%]
			hiddentooutput[i][j] = inputarray[i][j] + inputarray[i][j] * (((float)widthdist (rng))-500)/500 * learningrate;
			//add small variation to the recieved genes not relative to the weight [-learningrate - learningrate]
			//hiddentooutput[i][j] = inputarray[i][j] + (((float)widthdist (rng))-500)/500 * learningrate;
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
		return 360.0 + rotation;
	if(rotation > 360)
		return rotation-360.0;
	return rotation;
}

void robot::adjustlearningrate(float adapt){
	//don't go below endlearningrate
	if(learningrate>endlearningrate){
		float totalshift = startlearningrate - endlearningrate;
		learningrate -= adapt * totalshift;
	}
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