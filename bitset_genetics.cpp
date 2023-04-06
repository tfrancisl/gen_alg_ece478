#include "bitset_genetics.h"



void Mutate(bitset<CHROMO_LENGTH> &bits) {
	for (int i=0; i<CHROMO_LENGTH; i++) {
		if (RANDOM_NUM < MUTATION_RATE) {
			if (bits[i]) {
				bits[i] = 0;
			} else {
				bits[i] = 1;
			}
		}
	}

	return;
}

void Crossover(bitset<CHROMO_LENGTH> &offspring1, bitset<CHROMO_LENGTH> &offspring2) {
  bitset<CHROMO_LENGTH> t1,t2;
  int crossover;

  //dependent on the crossover rate
  if (RANDOM_NUM < CROSSOVER_RATE)
  {
    //create a random crossover point
    crossover = (int)(RANDOM_NUM*CHROMO_LENGTH);
	for (int i=0; i<CHROMO_LENGTH; i++) {
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

bitset<CHROMO_LENGTH> Roulette(int total_fitness, vector<Entity> population) {
	//generate a random number between 0 & total fitness count
	float rand_fit = (float)(RANDOM_NUM * total_fitness);

	//go through the chromosones adding up the fitness so far
	float track_fitness = 0.0f;
	
	for (vector<Entity>::iterator	 i = population.begin(); i != population.end(); ++i)
	{
		track_fitness += i->genes.fitness;
		
		//if the fitness so far > random number return the chromo at this point
		if (track_fitness >= rand_fit)
		{
			return i->genes.bits;
		}

	}

	return bitset<CHROMO_LENGTH>();
}
