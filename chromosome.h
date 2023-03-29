#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_
#include <string>
#include <iostream>

using std::string;

class Chromosome {

public:
    string bits;
    int length;
    float fitness;

    Chromosome();
    Chromosome(string b, float f, int l);

    void PrintChromosome(string bits);

};

#endif //CHROMOSOME_H_