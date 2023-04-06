#include "chromosome.h"
#include <iostream>
#include <vector>
#include <bitset>
using std::string;
using std::vector;



template<int gene_len, int gene_count>
Chromosome<gene_len, gene_count>::Chromosome(){
    this->bits = bitset<gene_len*gene_count>(0);
    this->fitness = 1.0f;
    this->length = gene_len*gene_count;
}

template<int gene_len, int gene_count>
Chromosome<gene_len, gene_count>::Chromosome(bitset<gene_len*gene_count> b, float f, int l) {
    this->bits = b;
    this->fitness = f;
    this->length = l;
}

template<int gene_len, int gene_count>
void Chromosome<gene_len, gene_count>::PrintChromosome() {
    std::cout << this->bits.to_string() << std::endl;
}

template<int gene_len, int gene_count>
void Chromosome<gene_len, gene_count>::BitsToRules() {
    int key;
    bitset<gene_len> val;

    for(int i=0; i<gene_count; i++) {
        for (int k=0; k<gene_len; k++) {
            val[k] = bits[gene_len*i+k];
        }

        key = i;
        rules[key] = val; 
    }
}