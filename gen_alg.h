#ifndef GEN_ALG_
#define GEN_ALG_

#define STATE_SIZE                  4
#define GENE_COUNT                  1<<(4+STATE_SIZE)
//#define GENE_COUNT             	  	pow(2,4+STATE_SIZE)
#define GENE_LENGTH               	(2+STATE_SIZE)
#define CHROMO_LENGTH				(GENE_COUNT)*(GENE_LENGTH)

#define CROSSOVER_RATE            	0.7
#define MUTATION_RATE             	0.001
#define EATER_POP_SIZE              20
#define PLANT_POP_SIZE              100

#define WORLD_SIZE 					25
#define DAYS_PER_GENERATION 		200
#define GENERATIONS					1000

namespace std {
template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
}
}

#endif //GEN_ALG_