#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"
#include <fstream>
using namespace std;

#define generations 200
#define trainingiterations 200

void generationallearning(world World);
void reinforcementlearning(world World);

int main(){
	srand (time(NULL));
	
	bool generational = true;

	world World(generational);
	generational ? generationallearning(World) : reinforcementlearning(World);
	
	
}

void generationallearning(world World){
	ofstream outputfile;
	outputfile.open("graphs/baseline.txt");
	int its = trainingiterations;
	string faka;
	for(int i = 0; i < generations; i++){
		while (!World.done()&& its > 0){
			//cout<<endl<<"Current generation: "<<i<<" frames: "<<World.frames<<" food left: "<<World.nfood<<endl;
			//World.drawworld();
			World.simulate();
			its--;
		}
		cout<<"generation: "<<i<<" food left: "<<World.nfood<<endl;
		its = trainingiterations;
		//World.drawworld();
		World.updaterobots();
		outputfile<<i<<";"<<World.currentaveragefitness<<";"<<World.currentmaxfitness<<"\n";
		World.randomizeworld();
	}
	/*
	//test the trained robots
	while (!World.done()&& its > 0){
		cout<<endl<<"Current generation: final gen, frames: "<<World.frames<<" food left: "<<World.nfood<<endl;
		World.drawworld();
		//cin>>faka;
		World.simulate();
		its--;
	}
	cout<<endl<<"Current iteration: final gen, food left: "<<World.nfood<<endl;
	World.drawworld();
	*/
	outputfile.close();
	
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
