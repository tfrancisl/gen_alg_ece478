#include "chromosome.h"
#include <iostream>
#include <vector>
#include <bitset>
using std::string;
using std::vector;


#define GENE_COUNT             	  	256
#define GENE_LENGTH               	5
#define CHROMO_LENGTH				(GENE_COUNT)*(GENE_LENGTH)

Chromosome::Chromosome(){
    this->bits = "";
    this->fitness = 0.0f;
    this->length = CHROMO_LENGTH;
}

Chromosome::Chromosome(string b, float f, int l) {
    this->bits = b;
    this->fitness = f;
    this->length = l;
}

void Chromosome::PrintChromosome() {
    for (int i=0; i<this->length; i++) {
            
        std::cout << this->bits[i];
    }

    std::cout << std::endl;
}

void Chromosome::BitsToRules() {
    string key;
    string val;

    for(int i=0; i<GENE_COUNT; i++) {
        for (int j=0; j<GENE_LENGTH; j++) {
            val = bits.substr(i*j, GENE_LENGTH);
            key = std::bitset<4+2>(i).to_string(); 
            rules[key] = val; 

        }
    }


}