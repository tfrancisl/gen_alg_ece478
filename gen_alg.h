#ifndef GEN_ALG_
#define GEN_ALG_

#define STATE_SIZE                      4

//one bit for apex warning signal, 2 bits for what is directly in front of the eater, 2 bits for facing
#define EATER_TRAITS                    5         
#define EATER_GENE_COUNT                1<<(EATER_TRAITS+STATE_SIZE)
#define EATER_GENE_LENGTH               (2+STATE_SIZE)  //plus 2 for action
#define CHROMO_LENGTH				    (EATER_GENE_COUNT)*(EATER_GENE_LENGTH)

//4 bits for total eater count in facing grid, 2 bits for facing
#define APEX_TRAITS                     6
#define APEX_GENE_COUNT                 1<<(APEX_TRAITS+STATE_SIZE)
#define APEX_GENE_LENGTH               	(2+STATE_SIZE)  //plus 2 for action
#define APEX_CHROMO_LENGTH				(APEX_GENE_COUNT)*(APEX_GENE_LENGTH)

#define MAX_DELETION_LENGTH         3*EATER_GENE_LENGTH

#define WORLD_SIZE 					40          // Cannot be moved to a variable since it is used for templating

#define MAKE_CSV 1

//returns a float between 0 & 1
#define RANDOM_NUM			((float)rand()/((float)(RAND_MAX)+1))

//returns a random integer between 0 and N-1
#define RANDOM_NUM_RANGE(N)	rand() % (int)(N)

#endif //GEN_ALG_