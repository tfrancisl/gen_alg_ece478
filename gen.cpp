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
#define POP_SIZE                  	10			//must be an even number
#define GENE_COUNT             	  	128
#define GENE_LENGTH               	5
#define CHROMO_LENGTH				GENE_COUNT*GENE_LENGTH
#define MAX_ALLOWABLE_GENERATIONS	400
#define WORLD_SIZE 					5

//returns a float between 0 & 1
#define RANDOM_NUM		((float)rand()/((float)(RAND_MAX)+1))

typedef std::map<vector<int>, Entity> population;

string  		GetRandomBits(int length);
float   		CalculateFitness(string bits);
string  		Roulette(int total_fitness, population pop);
void    		Mutate(string &bits);
void   			Crossover(string &offspring1, string &offspring2);
int 			GetRandomLocation();
vector<string> 	PopulationToStrings(population pop, vector<string> world);
population 		CreatePopulation(int pop_size, string type, Chromosome *gene_pool);


int main() {
    //seed the random number generator
	srand((int)time(NULL));
	Chromosome GenePool[POP_SIZE];
	population Eaters;
	population Plants;
	vector<string> the_world;

	Eaters = CreatePopulation(POP_SIZE, "eater", GenePool);
	Plants = CreatePopulation(POP_SIZE/2, "plant", (Chromosome *)NULL);

	the_world = vector<string>(WORLD_SIZE, string(WORLD_SIZE, '0'));

	for (int i = 0; i<WORLD_SIZE; i++) {
		std::cout << the_world[i] << std::endl;
	}
	std::cout << std::endl;

	the_world = PopulationToStrings(Eaters, the_world);
	for (int i = 0; i<WORLD_SIZE; i++) {
		std::cout << the_world[i] << std::endl;
	}
	std::cout << std::endl;

	the_world = PopulationToStrings(Plants, the_world);
	for (int i = 0; i<WORLD_SIZE; i++) {
		std::cout << the_world[i] << std::endl;
	}
	std::cout << std::endl;

}

// My algorithm for whatever "game" or thing I want to simulate
// Likely will take inputs beyond the genes
float CalculateFitness(string bits) {
    
}


//Takes a population and a WORLD_SIZE long vector of WORLD_SIZE long strings
//Returns that population placed into that world (if possible?)
vector<string> PopulationToStrings(population pop, vector<string> world) {
	int x, y;
	string ent_sym;
	int size = WORLD_SIZE;
	vector<string> new_world = world;


	for (population::iterator i = pop.begin(); i != pop.end(); ++i) {
		x = i->first[0];
		y = i->first[1];

		if (i->second.type == "eater") {
			ent_sym = "T";
		} else if(i->second.type == "plant") { 
			ent_sym = "P";
		} else {
			ent_sym = "X";
		}
		
		new_world[y].replace(x, 1, ent_sym);

	}
    return new_world;
}

population CreatePopulation(int pop_size, string type, Chromosome *gene_pool)
{
    vector<int> coords = {0, 0};
	population Population;
	Entity ent;


	if (gene_pool != NULL) {
		for (int i=0; i<pop_size; i++) {
			gene_pool[i] = Chromosome(GetRandomBits(CHROMO_LENGTH), 0.0f, CHROMO_LENGTH);

			ent = Entity(type, gene_pool[i]);
		
			do {
				int cs[2] = {(int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1))), (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)))};
				coords.assign(cs, cs+2);

			} while (Population.find(coords) != Population.end());
			
			Population.insert({coords, ent});

		}
	} else {
		for (int i=0; i<pop_size; i++) {
			ent = Entity(type);
		
			do {
				int cs[2] = {(int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1))), (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)))};
				coords.assign(cs, cs+2);

			} while (Population.find(coords) != Population.end());
			
			Population.insert({coords, ent});

		}
	}

	std::cout << "Created a population of " << pop_size << " " << type << "(s)." << std::endl; 

	return Population;
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

//------------------------------------Mutate---------------------------------------
//
//	Mutates a chromosome's bits dependent on the MUTATION_RATE
//-------------------------------------------------------------------------------------
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

//---------------------------------- Crossover ---------------------------------------
//
//  Dependent on the CROSSOVER_RATE this function selects a random point along the 
//  lenghth of the chromosomes and swaps all the  bits after that point.
//------------------------------------------------------------------------------------
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


//--------------------------------Roulette-------------------------------------------
//
//	selects a chromosome from the population via roulette wheel selection
//------------------------------------------------------------------------------------
string Roulette(int total_fitness, population Population) {
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
}	
