#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include "gen_alg.h"

using std::string;
using std::map;

class Chromosome {

public:
    string bits;
    map<string, string> rules;
    int length;
    float fitness;  

    Chromosome();
    Chromosome(string b, float f, int l);

    void PrintChromosome();
    void BitsToRules();

};

#endif //CHROMOSOME_H_