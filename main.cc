#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"
using namespace std;

int main(){
	srand (time(NULL));
	int trainingiterations = 10000;
	int generations = 1;

	int its = trainingiterations;
	bool generational = false;

	world World(generational);

	//generational learning
	if (generational){
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
	//reinforcement learning
	else{
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
}
