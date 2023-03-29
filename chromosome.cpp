#include "chromosome.h"
#include <iostream>
using std::string;

Chromosome::Chromosome(){
    this->bits = "";
    this->fitness = 0.0f;
    this->length = 100;
}

Chromosome::Chromosome(string b, float f, int l) {
    this->bits = b;
    this->fitness = f;
    this->length = l;
}

void Chromosome::PrintChromosome(string bits) {
    for (int i=0; i<this->length; i++) {
            
        std::cout << bits[i];
    }

    std::cout << std::endl;
}