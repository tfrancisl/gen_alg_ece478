#ifndef GEN_ALG_
#define GEN_ALG_

#define STATE_SIZE                  4
#define GENE_COUNT                  1<<(4+STATE_SIZE)
#define GENE_LENGTH               	(2+STATE_SIZE)
#define CHROMO_LENGTH				(GENE_COUNT)*(GENE_LENGTH)

#define CROSSOVER_RATE            	0.7
#define MUTATION_RATE             	0.001
#define DELETION_RATE               0.001
#define MAX_DELETION_LENGTH         3*GENE_LENGTH
#define EATER_POP_SIZE              50              //Must be even (roulette rules)
#define PLANT_POP_SIZE              270

#define WORLD_SIZE 					40
#define DAYS_PER_GENERATION 		250
#define GENERATIONS					10000

#define MAKE_CSV 1

//returns a float between 0 & 1
#define RANDOM_NUM			((float)rand()/((float)(RAND_MAX)+1))

//returns a random integer between 0 and N-1
#define RANDOM_NUM_RANGE(N)	rand() % (int)N

#endif //GEN_ALG_