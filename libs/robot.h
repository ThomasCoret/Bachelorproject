#ifndef ROBOT_H
#define ROBOT_H

#include <time.h> 
#include <random>
#include <stdlib.h> 
#include <iostream>

#define MAX 20

class robot{
	
	//variables
	private:
		//inputs for the neural network
		float input[MAX];
		//weights from input to hidden
		float inputtohidden[MAX][MAX];
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
		//amount of inputs
		int inputs;
		//amount of outputs
		int outputs;
		//amount of hidden layers
		int hiddenlayers;
		//learningspeed
		float alpha;

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
		//fitness of the robot
		float fitness;
		//unique number for the robot
		int nrobot;
		//previous rotation (for learning purposes)
		int prevrotation;

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
		void simulate(bool, bool, bool);
		//return inputtohidden array
		void returnith(float[MAX][MAX]);
		//return hiddentooutput array
		void returnhto(float[MAX][MAX]);
		//copy the input array to inputtohidden with small changes
		void newith(float[MAX][MAX]);
		//copy the input array to hiddentoouput with small changes
		void newhto(float[MAX][MAX]);
		//correct rotation value
		int fixrotation(int rotation);
		//adjust the neural network based on what action the robot should have made
		void qlearn(bool, bool, bool);
		
};
#endif