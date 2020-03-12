#include "food.h"

food::food(){
	x=0.0;
	y=0.0;
}

food::food(float _x, float _y, int _nfood){
	x = _x;
	y = _y;
	nfood = _nfood;
}