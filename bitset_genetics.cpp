#include "bitset_genetics.h"

void Mutate(bitset<CHROMO_LENGTH> &bits) {
	for (int i=0; i<CHROMO_LENGTH; i++) {
		if (RANDOM_NUM < MUTATION_RATE) {
			bits[i].flip();
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

void Deletion(bitset<CHROMO_LENGTH> &bits) {
	int del_pt,del_len;

	if (RANDOM_NUM < DELETION_RATE) {
		del_pt = (int)(RANDOM_NUM*CHROMO_LENGTH);
		del_len = RANDOM_NUM_RANGE(MAX_DELETION_LENGTH) + 1;

		for (int i=del_pt; i<del_pt+del_len; i++) {
			if (i<CHROMO_LENGTH) {
				bits[i] = 0;
			}
		}
	}

}
