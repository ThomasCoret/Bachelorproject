#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"
using namespace std;

#define generations 1000
#define trainingiterations 10000

void generationallearning(world World);
void reinforcementlearning(world World);

int main(){
	srand (time(NULL));
	
	bool generational = true;

	world World(generational);
	generational ? generationallearning(World) : reinforcementlearning(World);
	
	
}

void generationallearning(world World){
	int its = trainingiterations;
	for(int i = 0; i < generations; i++){
		cout<<i<<endl;
		while (!World.done()&& its > 0){
			//cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
			//World.drawworld();
			World.simulate();
			its--;
		}
		its = trainingiterations;
		World.updaterobots();
		World.randomizeworld();
	}
	//test the trained robots
	while (!World.done()&& its > 0){
		cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
		World.drawworld();
		World.simulate();
		its--;
	}
	cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
	World.drawworld();
}

void reinforcementlearning(world World){
	int its = trainingiterations;
	for(int i = 0; i < generations; i++){
		cout<<i<<endl;
		while (!World.done()&& its > 0){
			cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
			World.drawworld();
			World.simulate();
			its--;
		}
		its = trainingiterations;
		World.updaterobots();
		World.randomizeworld();
	}
}