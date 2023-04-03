#include "chromosome.h"
#include <iostream>
#include <vector>
#include <bitset>
using std::string;
using std::vector;




Chromosome::Chromosome(){
    this->bits = bitset<CHROMO_LENGTH>(0);
    this->fitness = 1.0f;
    this->length = CHROMO_LENGTH;
}

Chromosome::Chromosome(bitset<CHROMO_LENGTH> b, float f, int l) {
    this->bits = b;
    this->fitness = f;
    this->length = l;
}

void Chromosome::PrintChromosome() {
    std::cout << this->bits.to_string() << std::endl;
}

void Chromosome::BitsToRules() {
    int key;
    bitset<GENE_LENGTH> val;

    for(int i=0; i<GENE_COUNT; i++) {
        //for (int j=0; j<GENE_LENGTH; j++) {

            //val = bits.substr(i*j, GENE_LENGTH);
            for (int k=0; k<GENE_LENGTH; k++) {
                val[k] = bits[GENE_LENGTH*i+k];
            }

            key = i;
            rules[key] = val; 


        //}
    }
}