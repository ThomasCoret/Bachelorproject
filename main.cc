#include <iostream>
#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"

using namespace std;

int main(){

	world World;
	while (!World.done()){
		World.simulate();
	}
}
