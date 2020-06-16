#include "worldmanager.h"

worldmanager::worldmanager(int _nworlds, int _generations, bool _identicalrobots){
	nworlds = _nworlds;
	generations = _generations;
	identicalrobots = _identicalrobots;
	for(int i = 0; i < nworlds; i++){
		world Newworld;
		//if this is a world with identical robots clone the robots so they are the same
		if(identicalrobots){
			Newworld.clonerobots();
		}

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
	/* //  code for random food (also replace the 0 in randomizeworld with newrandfood)
	std::random_device dev;
    std::mt19937 rng(dev());
    //get maxfood from world 0 since it always exists and is the same for all worlds
    std::uniform_int_distribution<std::mt19937::result_type> fooddist (0,Worlds[0].maxfood);
    //all worlds need the same amound of food otherwise you can't compare the fitness since more food = more fitness
	int newrandfood = fooddist(rng);
	*/
	float inputith[MAX][MAX];
	float inputhto[MAX][MAX];

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
	Worlds[bestworld].robots[0].adjustlearningrate((float)1/generations);
	//clone the robots to adjust the learningrate of the other robots
	Worlds[bestworld].clonerobots();
	//update the other worlds with the best robot's nodes then randomize the world
	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		if(i != bestworld){
			Worlds[i].newith(inputith);
			Worlds[i].newhto(inputhto);
			Worlds[i].robots[0].adjustlearningrate((float)1/generations);
			//we need to clone the robots since newith and newhto only update and mutate robot[0]
			Worlds[i].clonerobots();
		}
		Worlds[i].randomizeworld(0);
	}
}

void worldmanager::update(int n){
	//if there are too few worlds a bestworlds index will be -1 so output is invalid and will likely crash I could use a try catch but thats too much work
	if(n > nworlds){
		std::cout<<"WARNING: there too few worlds to use: "<<n<<" best worlds please disregard any output!"<<std::endl;
	}
	float inputith[n][MAX][MAX];
	float inputhto[n][MAX][MAX];
	float bestfitnesses[n];
	std::vector<world>::size_type bestworlds[n];
	for(int i = 0; i<n; i++){
		bestfitnesses[i] = -2000000;
		bestworlds[i] = -1;
	}
	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		averagefitness += Worlds[i].getmaxfitness();
		bool done = false;
		for(int j = 0; j<n && !done; j++){
			//We found the j'th best robot
			if(Worlds[i].getmaxfitness() > bestfitnesses[j]){
				//we found the best robot so far
				if(j == 0){
					maxfitness = Worlds[i].getmaxfitness();
					bestworld = i;
				}
				//we need to shift all bestfitnesses one place down under our number. 
				for(int i = n; i>j; i--){
					bestfitnesses[i] = bestfitnesses[i-1];
					bestworlds[i] = bestworlds[i-1];
				}
				//now that we shifted all fitnesses under ours down we can put this fitness in
				bestfitnesses[j]=Worlds[i].getmaxfitness();
				bestworlds[j] = i;
				done = true;
			}
		}
	}
	//now we have the fitness and the index of the n best worlds
	//get the nodess from the best robot
	for(int i = 0; i < n; i++){
		Worlds[bestworlds[i]].getith(inputith[i]);
		Worlds[bestworlds[i]].gethto(inputhto[i]);
		Worlds[bestworlds[i]].robots[0].adjustlearningrate((float)1/generations);
	}
	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		if(i != bestworld){
			Worlds[i].newith(inputith[i%n]);
			Worlds[i].newhto(inputhto[i%n]);
			Worlds[i].robots[0].adjustlearningrate((float)1/generations);
			//we need to clone the robots since newith and newhto only update and mutate robot[0]
			Worlds[i].clonerobots();
		}
		Worlds[i].randomizeworld(0);
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

void worldmanager::randomizerobots(){
	for(std::vector<world>::size_type i = 0; i != Worlds.size(); i++) {
		Worlds[i].randomizerobots();
	}
}
