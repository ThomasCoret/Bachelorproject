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
		//width of the world
		int worldwidth; 
		//height of the world
		int worldheight;
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
		//amount of food collected
		int foodcollected;
		//unique number for the robot
		int nrobot;

	//functionS
	private:
		void neuralnetwork();
		float r(float);
		float rprime(float);

	public:
		robot();
		robot(float, float, int, int);
		void simulate(bool, bool, bool);
		void returnith(float[MAX][MAX]);
		void returnhto(float[MAX][MAX]);
		void newith(float[MAX][MAX]);
		void newhto(float[MAX][MAX]);
		
};
#endif