#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"
using namespace std;

int main(){
	srand (time(NULL));
	world World;
	int trainingiterations = 500;
	int generations = 1000;
	for(int i = 0; i < generations; i++){
		cout<<i<<endl;
		while (!World.done()&& trainingiterations > 0){
			//cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
			//World.drawworld();
			World.simulate();
			trainingiterations--;
		}
		trainingiterations = 500;
		World.updaterobots();
		World.randomizeworld();
	}
	//test the trained robots
	while (!World.done()&& trainingiterations > 0){
		cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
		World.drawworld();
		World.simulate();
		trainingiterations--;
	}
	cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
	World.drawworld();
}
