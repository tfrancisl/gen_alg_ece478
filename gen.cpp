#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "chromosome.h"
#include "time.h"
#include "entity.h"
#include <unistd.h>
#include <cmath>

using std::string;
using std::vector;

#define CROSSOVER_RATE            	0.7
#define MUTATION_RATE             	0.001
#define EATER_POP_SIZE              10
#define PLANT_POP_SIZE              4
#define GENE_COUNT             	  	128
#define GENE_LENGTH               	5
#define CHROMO_LENGTH				GENE_COUNT*GENE_LENGTH
#define MAX_ALLOWABLE_GENERATIONS	400
#define WORLD_SIZE 					12

//returns a float between 0 & 1
#define RANDOM_NUM		((float)rand()/((float)(RAND_MAX)+1))

//typedef std::map<vector<int>, Entity> population;


string  		GetRandomBits(int length);
float   		CalculateFitness(string bits);
//string  		Roulette(int total_fitness, population pop);

void    		Mutate(string &bits);
void   			Crossover(string &offspring1, string &offspring2);
int 			GetRandomLocation();
vector<string> WorldToStrings(Entity world[WORLD_SIZE][WORLD_SIZE]);

//vector<string> 	PopulationToStrings(population pop, vector<string> world);
//population 		CreatePopulation(int pop_size, string type, Chromosome *gene_pool);


int main() {
    //seed the random number generator
	srand((int)time(NULL));
	int x,y;

	Entity world[WORLD_SIZE][WORLD_SIZE];
	Entity eater_population[EATER_POP_SIZE];
	Entity plant_population[PLANT_POP_SIZE];

	Entity tmp_ent;
	Chromosome tmp_chrm;

	vector<string> str_world;


	//create the eaters and place them in the world

	for (int i=0; i<EATER_POP_SIZE; i++) {

		tmp_chrm = Chromosome(GetRandomBits(CHROMO_LENGTH), 0.0f, CHROMO_LENGTH);
		tmp_ent = Entity("eater", tmp_chrm);

		do {
			x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		} while(world[x][y].type != "none");

		world[x][y] = tmp_ent;


	}

	//create the plants and place them in the world
	for (int i=0; i<PLANT_POP_SIZE; i++) {
		tmp_ent = Entity("plant");

		do {
			x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		} while(world[x][y].type != "none");

		world[x][y] = tmp_ent;
	}

	str_world = WorldToStrings(world);

	for (int i = 0; i<WORLD_SIZE; i++) {
		std::cout << str_world[i] << std::endl;
	}
	std::cout << std::endl;

}

// My algorithm for whatever "game" or thing I want to simulate
// Likely will take inputs beyond the genes
float CalculateFitness(string bits) {
    
}

vector<string> WorldToStrings(Entity world[WORLD_SIZE][WORLD_SIZE]) {
	int x,y;
	string ent_sym;
	vector<string> str_world = vector<string>(WORLD_SIZE, string(WORLD_SIZE, '.'));

	for (x=0; x< WORLD_SIZE; x++) {
		for (y=0; y< WORLD_SIZE; y++) {
			
			if (world[x][y].type == "eater") {
				ent_sym = "T";
			} else if(world[x][y].type == "plant") { 
				ent_sym = "P";
			} else if(world[x][y].type == "none") {
				ent_sym = ".";
			} else { //error case
				ent_sym = "X";
			}

			str_world[y].replace(x, 1, ent_sym);

		}
	}

	return str_world;
}


string	GetRandomBits(int length) {
	string bits;

	for (int i=0; i<length; i++)
	{
		if (RANDOM_NUM > 0.5f)

			bits += "1";

		else

			bits += "0";
	}

	return bits;
}

void Mutate(string &bits) {
	for (int i=0; i<bits.length(); i++) {
		if (RANDOM_NUM < MUTATION_RATE) {
			if (bits.at(i) == '1')

				bits.at(i) = '0';

			else

				bits.at(i) = '1';
		}
	}

	return;
}

void Crossover(string &offspring1, string &offspring2) {
  //dependent on the crossover rate
  if (RANDOM_NUM < CROSSOVER_RATE)
  {
    //create a random crossover point
    int crossover = (int) (RANDOM_NUM * CHROMO_LENGTH);

    string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMO_LENGTH);
    string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMO_LENGTH);

    offspring1 = t1; offspring2 = t2;				  
  }
}

/*string Roulette(int total_fitness, population Population) {
	//generate a random number between 0 & total fitness count
	float Slice = (float)(RANDOM_NUM * total_fitness);
	
	//go through the chromosones adding up the fitness so far
	float FitnessSoFar = 0.0f;
	
	for (population::iterator	 i = Population.begin(); i != Population.end(); ++i)
	{
		//FitnessSoFar += Population[i].fitness;
		FitnessSoFar += i->second.genes.fitness;
		
		//if the fitness so far > random number return the chromo at this point
		if (FitnessSoFar >= Slice)
		{
			return i->second.genes.bits;
		}

	}

	return "";
}*/	
