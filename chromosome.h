#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_
#include <string>
#include <map>
#include <iostream>
#include <math.h>

using std::string;
using std::map;


#define STATE_SIZE                  4
#define GENE_COUNT             	  	pow(2,4+STATE_SIZE)
#define GENE_LENGTH               	(2+STATE_SIZE)
#define CHROMO_LENGTH				(GENE_COUNT)*(GENE_LENGTH)

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