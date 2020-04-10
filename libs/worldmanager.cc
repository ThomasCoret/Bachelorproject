#include "worldmanager.h"

worldmanager::worldmanager(int _nworlds, int _generations){
	nworlds = _nworlds;
	generations = _generations;

	for(int i = 0; i < nworlds; i++){
		world Newworld;
		Worlds.push_back(Newworld);
	}

}

void worldmanager::simulate(){
	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		//only simulate if world is not done yet
		if(!Worlds[i].done()){
			Worlds[i].simulate();
		}
	}
}

void worldmanager::update(){
	std::random_device dev;
    std::mt19937 rng(dev());
    //get maxfood from world 0 since it always exists and is the same for all worlds
    std::uniform_int_distribution<std::mt19937::result_type> fooddist (0,Worlds[0].maxfood);

	float inputith[MAX][MAX];
	float inputhto[MAX][MAX];

	//all worlds need the same amound of food otherwise you can't compare the fitness since more food = more fitness
	int newrandfood = fooddist(rng);

	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		averagefitness += Worlds[i].getmaxfitness();
		//save the world with the best robot
		if(Worlds[i].getmaxfitness() > maxfitness){
			maxfitness = Worlds[i].getmaxfitness();
			bestworld = i;
		}
	}
	//get the nodes from the best robot
	Worlds[bestworld].getith(inputith);
	Worlds[bestworld].gethto(inputhto);

	//update the other worlds with the best robot's nodes then randomize them
	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		if(i != bestworld){
			Worlds[i].newith(inputith);
			Worlds[i].newhto(inputhto);
			Worlds[i].robots[0].adjustlearningrate((float)1/generations);
		}
		Worlds[i].randomizeworld(newrandfood);
	}
}

void worldmanager::resetfitness(){
	maxfitness = -2000000;
	averagefitness = 0;
	bestworld = -1;
}

void worldmanager::getith(float inputith[MAX][MAX]){
	//get the ith nodes from the best robot
	Worlds[bestworld].getith(inputith);
}
void worldmanager::gethto(float inputhto[MAX][MAX]){
	//get the hto nodes from the best robot
	Worlds[bestworld].gethto(inputhto);
}