#ifndef BITSET_GENETICS_
#define BITSET_GENETICS_
#include "gen_alg.h"

#include <bitset>
#include "entity.h"
#include <vector>
using std::bitset;
using std::vector;

void Mutate(bitset<CHROMO_LENGTH> &bits);
void Crossover(bitset<CHROMO_LENGTH> &offspring1, bitset<CHROMO_LENGTH> &offspring2);
void Deletion(bitset<CHROMO_LENGTH> &bits);

//template<int gene_len, int gene_count>
//bitset<gene_len*gene_count> Roulette(int total_fitness, vector<Chromosome<gene_len,gene_count>> population);

template<int gene_len, int gene_count>
bitset<gene_len*gene_count> Roulette(int total_fitness, vector<Chromosome<gene_len, gene_count>> population) {
	//generate a random number between 0 & total fitness count
	float rand_fit = (float)(RANDOM_NUM * total_fitness);

	//go through the chromosones adding up the fitness so far
	float track_fitness = 0.0f;
	
	for (auto i = population.begin(); i != population.end(); ++i)
	{
		track_fitness += i->fitness;
		
		//if the fitness so far > random number return the chromo at this point
		if (track_fitness >= rand_fit)
		{
			return i->bits;
		}

	}

	//Should never, ever have to run. 
	//If the population passed to this and the total fitness calculated come from the same place, track_fitness will always exceed rand_fit at some point
	std::cout << "Failed to a select a roulette candidate!!! Cumulative fitness: " << track_fitness << "/" << rand_fit << " (rand fitness), " << total_fitness << " (total fitness)." << std::endl;
	return bitset<CHROMO_LENGTH>();
}

#endif //BITSET_GENETICS_