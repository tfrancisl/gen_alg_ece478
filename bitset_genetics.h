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
bitset<CHROMO_LENGTH> Roulette(int total_fitness, vector<Entity> population);

#endif //BITSET_GENETICS_