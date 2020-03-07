#ifndef ROBOT_H
#define ROBOT_H

class robot{
	
	//variables
	private:


	public:
		int x, y;
		int rotation;
		float speed;
	//functionS
	private:


	public:
		robot();
		robot(int,int,int);
		void simulate();
};
#endif