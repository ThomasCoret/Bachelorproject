#include "libs/food.h"
#include "libs/robot.h"
#include "libs/world.h"

using namespace std;

#define generations 200
#define trainingiterations 500

void generationallearning(world World);
void reinforcementlearning(world World);

int main(){
	srand (time(NULL));
	
	bool generational = true;

	world World(generational);
	generational ? generationallearning(World) : reinforcementlearning(World);
	
	
}

void generationallearning(world World){
	bool makingrobot = false;
	int its = trainingiterations;
	ofstream outputfile;
	outputfile.open("graphs/randomrobot.txt");
	//string faka;
	if(makingrobot){
		for(int i = 0; i < generations; i++){
			while (!World.done()&& its > 0){
				//cout<<endl<<"Current generation: "<<i<<" frames: "<<World.frames<<" food left: "<<World.nfood<<endl;
				//World.drawworld();
				
				World.simulate();
				//cin>>faka;
				its--;
			}
			cout<<"generation: "<<i<<" food left: "<<World.nfood<<endl;
			its = trainingiterations;
			//World.drawworld();
			//every generation the learningrate of the robots is adapted 
			World.updaterobots(1/generations);
			//save the best robot of the last generation
			if(i == generations -1){
				World.savebestrobot("robotsaves/robot2");
			}
			outputfile<<i<<";"<<World.currentaveragefitness<<";"<<World.currentmaxfitness<<"\n";
			World.randomizeworld();

		} 
	}
	else{
		//World.loadrobot("robotsaves/robot1.bot");
		//test the trained robots
		for(int i = 0; i < generations; i++){
			while (!World.done()&& its > 0){
				//cout<<endl<<"Current generation: testrobot, frames: "<<World.frames<<" food left: "<<World.nfood<<endl;
				//World.drawworld();
				//cin>>faka;
				World.simulate();
				its--;
			}
			cout<<"generation: "<<i<<" food left: "<<World.nfood<<endl;
			its = trainingiterations;
			outputfile<<i<<";"<<World.getaveragefitness()<<";"<<World.getmaxfitness()<<"\n";
			World.randomizeworld();
		}	
	}
	outputfile.close();
	
}

void reinforcementlearning(world World){
	int its = trainingiterations;
	for(int i = 0; i < generations; i++){
		cout<<i<<endl;
		while (!World.done()&& its > 0){
			cout<<endl<<"Current iteration: "<<World.frames<<" food left: "<<World.nfood<<endl;
			World.drawworld();
			//World.simulate();
			its--;
		}
		its = trainingiterations;
		//World.updaterobots();
		World.randomizeworld();
	}
}
