#ifndef BITSET_GENETICS_
#define BITSET_GENETICS_
#include "gen_alg.h"
#include "bitset_genetics.cpp"

template<int gene_len, int gene_count>
void Mutate(bitset<gene_len*gene_count> &bits);

template<int gene_len, int gene_count>
void Crossover(bitset<gene_len*gene_count> &offspring1, bitset<gene_len*gene_count> &offspring2);

template<int gene_len, int gene_count>
void Deletion(bitset<gene_len*gene_count> &bits);

template<int gene_len, int gene_count>
bitset<gene_len*gene_count> Roulette(int total_fitness, vector<Chromosome<gene_len, gene_count>> population);


#endif //BITSET_GENETICS_