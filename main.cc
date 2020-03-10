#include <iostream>
#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"

using namespace std;

int main(){
	srand (time(NULL));
	world World;
	//while (!World.done()){
		cout<<endl<<World.frames<<endl<<endl;
		World.simulate();
		World.drawworld();
	//}
}
