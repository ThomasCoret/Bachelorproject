#include "robot.h"

robot::robot(){
	x = 0;
	y = 0; 
}

robot::robot(int _x, int _y, int _rotation){
	x = _x;
	y = _y;
	rotation = _rotation;
	radius = 3.0;
}

void robot::simulate(bool foodahead){

}