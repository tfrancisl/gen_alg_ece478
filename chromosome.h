#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <bitset>
#include "gen_alg.h"

using std::bitset;
using std::map;
using std::array;


class Chromosome {

public:
    bitset<CHROMO_LENGTH> bits;
    //map<int, bitset<GENE_LENGTH>> rules;
    array<bitset<GENE_LENGTH>, GENE_COUNT> rules;
    int length;
    float fitness;  

    Chromosome();
    Chromosome(bitset<CHROMO_LENGTH> b, float f, int l);

    void PrintChromosome();
    void BitsToRules();

};

#endif //CHROMOSOME_H_