#ifndef ROBOT_H
#define ROBOT_H

class robot{
	
	//variables
	private:


	public:
		//position of robot
		int x, y;
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
		robot(int,int,int);
		void simulate(bool);
};
#endif