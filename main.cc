#include <iostream>
#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"

using namespace std;

int main(){
	srand (time(NULL));
	world World;
	//while (!World.done()){
	for(int i = 0;i<8;i++){
		cout<<endl<<World.frames<<endl;
		World.drawworld();
		World.simulate();
	}
	World.drawworld();
}
