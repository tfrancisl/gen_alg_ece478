#include <bitset>
#include "entity.h"
#include <vector>
using std::bitset;
using std::vector;


//optimization idea: there should be a way to generate a random number such that the 0.1% mutation rate applies in the case of bitwise or (or some other operator) against the genome

template<int gene_len, int gene_count>
void Mutate(bitset<gene_len*gene_count> &bits, float mutation_rate) {
	for (int i=0; i<(gene_len*gene_count); i++) {
		if (RANDOM_NUM < mutation_rate) {
			bits[i].flip();
		}
	}

	return;
}

template<int gene_len, int gene_count>
void Crossover(bitset<gene_len*gene_count> &offspring1, bitset<gene_len*gene_count> &offspring2, float crossover_rate) {
  bitset<gene_len*gene_count> t1,t2;
  int crossover;

  //dependent on the crossover rate
  if (RANDOM_NUM < crossover_rate)
  {
    //create a random crossover point
    crossover = (int)(RANDOM_NUM*(gene_len*gene_count));
	for (int i=0; i<(gene_len*gene_count); i++) {
		if (i < crossover) {
			t1[i] = offspring1[i];
			t2[i] = offspring2[i];
		} else {
			t1[i] = offspring2[i];
			t2[i] = offspring1[i];
		}
	}
    offspring1 = t1; offspring2 = t2;				  

  }
}

template<int gene_len, int gene_count>
void Deletion(bitset<gene_len*gene_count> &bits, float deletion_rate) {
	int del_pt,del_len;

	if (RANDOM_NUM < deletion_rate) {
		del_pt = (int)(RANDOM_NUM*(gene_len*gene_count));
		del_len = RANDOM_NUM_RANGE(MAX_DELETION_LENGTH) + 1;

		for (int i=del_pt; i<del_pt+del_len; i++) {
			if (i<(gene_len*gene_count)) {
				bits[i] = 0;
			}
		}
	}

}


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
	//std::cout << "Failed to a select a roulette candidate!!! Cumulative fitness: " << track_fitness << "/" << rand_fit << " (rand fitness), " << total_fitness << " (total fitness)." << std::endl;
	return bitset<gene_len*gene_count>();
}