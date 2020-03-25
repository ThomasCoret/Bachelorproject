#ifndef ROBOT_H
#define ROBOT_H

#include <time.h> 
#include <random>
#include <stdlib.h> 
#include <iostream>
#include <fstream>

#define MAX 20

class robot{
	
	//variables
	private:
		//inputs for the neural network
		float input[MAX];
		
		//weights from hidden to output
		float hiddentooutput[MAX][MAX];
		//input for the hidden layer
		float inhidden[MAX];
		//output for the hidden layer
		float acthidden[MAX];
		//input for the output layer
		float inoutput[MAX];
		//output
		float netoutput[MAX];
		
		//amount of outputs
		int outputs;
		//amount of hidden layers
		int hiddenlayers;
		//learningspeed
		float learningrate;
		//start learningrate
		float startlearningrate;
		//end learningrate
		float endlearningrate;

	public:
		//weights from input to hidden
		float inputtohidden[MAX][MAX];
		//position of robot
		float x, y;
		//rotation of the robot in degrees 0-360
		float rotation;
		//maximum speed of the robot
		float maxspeed;
		//current speed of the robot
		float speed;
		//current rotation speed of the robot
		float turnspeed;
		//radius in which the robot is able to see food;
		float radius;
		//radius in which the robot is able to pick up food
		float grabradius;
		//fitness of the robot
		float fitness;
		//unique number for the robot
		int nrobot;
		//amount of inputs
		int inputs;

	//functionS
	private:
		//run the neural network
		void neuralnetwork();
		//functions to calculate whether neuron is activated or not
		float r(float);
		float rprime(float);
		float g(float);
		float gprime(float);

	public:
		robot();
		robot(float, float, int, int);
		//simulate the robots behaviour based on whether there is food ahead, to the right or to the left.
		void simulate(float, float, float, float, float, float);
		//return inputtohidden array
		void returnith(float[MAX][MAX]);
		//return hiddentooutput array
		void returnhto(float[MAX][MAX]);
		//copy the input array to inputtohidden with small changes
		void newith(float[MAX][MAX]);
		//copy the input array to hiddentoouput with small changes
		void newhto(float[MAX][MAX]);
		//correct rotation value
		float fixrotation(float);
		//adjust the neural network based on what action the robot should have made
		void qlearn(float, float, float);
		//adjust the learning rate 
		void adjustlearningrate(float);
		//save the nodes of the NN to a file
		void savenodes(std::string);
		//copy input array to inputtohidden
		void copyith(float[MAX][MAX]);
		//copy input array to hiddentoouput
		void copyhto(float[MAX][MAX]);
	
};
#endif