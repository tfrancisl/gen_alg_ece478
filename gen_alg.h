#ifndef GEN_ALG_
#define GEN_ALG_

#define STATE_SIZE                      4

//one bit for apex warning signal, 2 bits for what is directly in front of the eater, 2 bits for which action to take 
#define EATER_TRAITS                    5         
#define EATER_GENE_COUNT                1<<(EATER_TRAITS+STATE_SIZE)
#define EATER_GENE_LENGTH               (2+STATE_SIZE)  //plus 2 for action
#define CHROMO_LENGTH				    (EATER_GENE_COUNT)*(EATER_GENE_LENGTH)

//4 bits for total eater count in facing grid, 2 bits for action
#define APEX_TRAITS                     6
#define APEX_GENE_COUNT                 1<<(APEX_TRAITS+STATE_SIZE)
#define APEX_GENE_LENGTH               	(2+STATE_SIZE)  //plus 2 for action
#define APEX_CHROMO_LENGTH				(APEX_GENE_COUNT)*(APEX_GENE_LENGTH)


#define CROSSOVER_RATE            	0.7
#define MUTATION_RATE             	0.001
#define DELETION_RATE               0.001
#define MAX_DELETION_LENGTH         3*EATER_GENE_LENGTH

#define EATER_POP_SIZE              50              //Must be even (roulette rules)
#define PLANT_POP_SIZE              100
#define APEX_POP_SIZE               16              //Must be even (roulette rules)

#define WORLD_SIZE 					40              // should be 4 more than a mutiple of (APEX_POP_SIZE/2)
#define DAYS_PER_GENERATION 		250
#define GENERATIONS					2000

#define MAKE_CSV 1

//returns a float between 0 & 1
#define RANDOM_NUM			((float)rand()/((float)(RAND_MAX)+1))

//returns a random integer between 0 and N-1
#define RANDOM_NUM_RANGE(N)	rand() % (int)(N)

#endif //GEN_ALG_