#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "chromosome.h"
#include "time.h"
#include "entity.h"
#include <unistd.h>
#include <cmath>
#include <bitset>
#include "gen_alg.h"

using std::string;
using std::vector;
using std::array;

#define MAKE_CSV 0

//returns a float between 0 & 1
#define RANDOM_NUM			((float)rand()/((float)(RAND_MAX)+1))

//returns a random integer between 0 and N-1
#define RANDOM_NUM_RANGE(N)	rand() % (int)N

//bitset<CHROMO_LENGTH>  	GetRandomBits(int length);
bitset<CHROMO_LENGTH>  	Roulette(int total_fitness, vector<Entity> population);

void Mutate(bitset<CHROMO_LENGTH> &bits);
void Crossover(bitset<CHROMO_LENGTH> &offspring1, bitset<CHROMO_LENGTH> &offspring2);
int GetRandomLocation();
vector<string> WorldToStrings(array<array<Entity, WORLD_SIZE>, WORLD_SIZE>);

int time_step = 0; //current "day"

// Progress time in the world.
// Going left-to-right and top-to-bottom, check the entity in each grid square
// If they're an eater, check the 4 nearby grids for plants. Concat these 4 bits with
// the 4 bits of the current state, then key into the map to check the action and next state.
// Take the action (if possible), and change to the next state. Move on.
// Presently, plants do nothing and "none" squares do nothing
template <class T, long unsigned int N>
float ProgressTime(array<array<T, N>, N> *world) {
	int x,y,m;
	//	bit     3    2    1    0
 	//  (*world)pos    7    5    3    1
	//  x       -    0    +    0
	//  y       0    +    0    -
	bitset<4> view(0);
	bitset<2> mvmt(0);
	bitset<GENE_LENGTH+2> rule_key(0);
	bitset<GENE_LENGTH> rule(0);
	float max_fitness = 0.0f;

	//std::cout << "time: " << time_step << std::endl;

	for (x=0; x<WORLD_SIZE; x++) {
		for (y=0; y<WORLD_SIZE; y++) {
			if((*world)[x][y].type == "eater" && (*world)[x][y].last_action != time_step) {

				//std::cout << "time: " << time_step << std::endl;
				//std::cout << "current ent last action: " << world[x][y].last_action << std::endl;
				(*world)[x][y].last_action = time_step;	//only one action allowed per day so keep these together

				if (x != WORLD_SIZE-1) {
					if ((*world)[x+1][y].type == "plant")
						view.set(1);
				} else if (x != 0) {
					if ((*world)[x-1][y].type == "plant")
						view.set(3);
				}
				if (y != WORLD_SIZE-1) {
					if ((*world)[x][y+1].type == "plant")
						view.set(2);
				} else if (y != 0) {
					if ((*world)[x][y-1].type == "plant")
						view.set(0);
				}

				for (int i=0; i<(2+GENE_LENGTH); i++) {
					if (i<GENE_LENGTH) {
						rule_key[i] = (*world)[x][y].state[i];
					} else {
						rule_key[i] = view[i-GENE_LENGTH];
					}
				}

				//std::cout << "rule key: " << rule_key.to_ulong() << std::endl;
				rule = (*world)[x][y].genes.rules[rule_key.to_ulong()];
				//std::cout << "rule: " << rule << std::endl;

				//mvmt = rule.substr(0, 2);			 	//get movement out of the rule
				//world[x][y].state = rule.substr(2, STATE_SIZE);	
				for(int i=0; i<GENE_LENGTH; i++) {
					if (i<2) {
						mvmt[i] = rule[i];
					} else {
						(*world)[x][y].state[i-2] = rule[i];   //update the eater's state regardless
					}
				}


				//m = std::stoi(mvmt, NULL, 2);
				m = mvmt.to_ulong();
				//std::cout << m << std::endl;

				if (max_fitness < (*world)[x][y].genes.fitness) {
					max_fitness = (*world)[x][y].genes.fitness;
				}

				//should find some alternative logic here - switch/case/break is not great
				switch(m) {
					case 0:
						if( !(y-1 < 0) ) {
							if((*world)[x][y-1].type == "plant") {
								(*world)[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								(*world)[x][y-1] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();	
								//RespawnPlant();	
								//RespawnPlantNearby(x,y-1);			
							} else if((*world)[x][y-1].type == "none") {
								(*world)[x][y-1] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
							}
						}	
						break;
					case 1:
						if( !(x+1 > WORLD_SIZE-1) )  {
							if((*world)[x+1][y].type == "plant") {
								(*world)[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								(*world)[x+1][y] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
								//RespawnPlant();
								//RespawnPlantNearby(x+1,y);		
							} else if((*world)[x+1][y].type == "none") {
								(*world)[x+1][y] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
							}
						}
						break;
					case 2:
						if( !(y+1 > WORLD_SIZE-1) ) {
							if((*world)[x][y+1].type == "plant") {
								(*world)[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								(*world)[x][y+1] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
								//RespawnPlant();
								//RespawnPlantNearby(x,y+1);	
							} else if((*world)[x][y+1].type == "none") {
								(*world)[x][y+1] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
							}
						}
						break;
					case 3:
						if( !(x-1 < 0) ) {
							if((*world)[x-1][y].type == "plant") {
								(*world)[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								(*world)[x-1][y] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
								//RespawnPlant();
								//RespawnPlantNearby(x-1,y);	
							} else if((*world)[x-1][y].type == "none") {
								(*world)[x-1][y] = (*world)[x][y]; 			//move the eater
								(*world)[x][y] = Entity();					//replace the eater with nothing
							}
						}
						break;
				}

				

			}
		}

	}


	time_step++;	//increment time step
	return max_fitness;
}
//array<array<Entity, WORLD_SIZE>, WORLD_SIZE> world;


int main() {
    //seed the random number generator
	srand((int)time(NULL));
	int x,y;
	float max_fitness = 0.0f,total_fitness = 0.0f;

	array<array<Entity, WORLD_SIZE>, WORLD_SIZE> *world = new array<array<Entity, WORLD_SIZE>, WORLD_SIZE>;

	vector<Entity> eater_population(EATER_POP_SIZE);
	vector<Entity> tmp_eater_population(EATER_POP_SIZE);

	bitset<CHROMO_LENGTH> genes1,genes2;
	Entity tmp_ent, child1, child2;
	Chromosome tmp_chrm,new_chrm1,new_chrm2;

	vector<string> str_world;

	//std::cout << "Creating " << EATER_POP_SIZE << " eaters and " << PLANT_POP_SIZE << " plant in a " << WORLD_SIZE << " square world." << std::endl;

	//create the eaters and place them in the world
	for (int i=0; i<EATER_POP_SIZE; i++) {
		genes1.reset();

		for (int j=0; j<CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes1[j].flip();
			}
		}


		tmp_chrm = Chromosome(genes1, 1.0f, CHROMO_LENGTH);

		tmp_ent = Entity("eater", tmp_chrm);
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state

		//std::cout << "Storing genetic code in rules table." << std::endl;
		tmp_ent.genes.BitsToRules();

		do {
			x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		} while((*world)[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

		(*world)[x][y] = tmp_ent;

		/*if (i==1){
			std::cout << "first eater state and genetic code " << tmp_ent.state << std::endl;

			for(int j=0; j<GENE_COUNT; j++) {
				std::cout << j << ": " << tmp_ent.genes.rules[j] << std::endl;
			}
		}*/
	}

	//create the plants and place them in the world
	for (int i=0; i<PLANT_POP_SIZE; i++) {
		tmp_ent = Entity("plant");

		do {
			x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		} while((*world)[x][y].type != "none");

		(*world)[x][y] = tmp_ent;
	}

	/////////////////////////////////////////////////////////
	#if MAKE_CSV
	std::cout << "generation,max_fitness,avg_fitness,avg_fitness_10" << std::endl;
	#endif
	
	for(int g=0; g<GENERATIONS; g++) {
		tmp_eater_population.clear();
		eater_population.clear();

		for (int d=0; d<DAYS_PER_GENERATION; d++) {
			max_fitness = ProgressTime(world);
		}

		total_fitness = 0.0f;
		for (x=0; x<WORLD_SIZE; x++) {
			for (y=0; y<WORLD_SIZE; y++) {
				if((*world)[x][y].type == "eater") {
					total_fitness += (*world)[x][y].genes.fitness;
					eater_population.push_back((*world)[x][y]);
				}
			}
		}

		#if MAKE_CSV
		std::cout << g << "," << max_fitness << "," << total_fitness/(float)(EATER_POP_SIZE) << "," << std::endl;
		#endif

		for (int i=0; i<EATER_POP_SIZE/2; i++) {
			//Select two
			genes1 = Roulette(total_fitness, eater_population);
			genes2 = Roulette(total_fitness, eater_population);

			//crossover
			Crossover(genes1, genes2);
			
			//mutate
			Mutate(genes1);
			Mutate(genes2);

			new_chrm1 = Chromosome(genes1, 1.0f, CHROMO_LENGTH); 
			new_chrm2 = Chromosome(genes2, 1.0f, CHROMO_LENGTH); 

			//set up the new entity
			child1 = Entity("eater", new_chrm1);
			child1.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
			child1.genes.BitsToRules();
			child2 = Entity("eater", new_chrm2);
			child2.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
			child2.genes.BitsToRules();

			if (i==0) {
				child1.spec = 1;
			}
			
			tmp_eater_population.push_back(child1);
			tmp_eater_population.push_back(child2);
		}

		//clear out the world
		for (int s=0; s<WORLD_SIZE; s++) {
			(*world)[s].fill(Entity());
		}

		//place the new population into the world
		for (int i=0; i<EATER_POP_SIZE; i++) {
			do {
				x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
				y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			} while((*world)[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

			(*world)[x][y] = tmp_eater_population[i];
		}


		//create the plants and place them in the world
		for (int i=0; i<PLANT_POP_SIZE; i++) {
			tmp_ent = Entity("plant");

			do {
				x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
				y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			} while((*world)[x][y].type != "none");

			(*world)[x][y] = tmp_ent;
		}
	}
	///////////////////////////////////////////////////////////////

	for (int d=0; d<DAYS_PER_GENERATION; d++) {
		max_fitness = ProgressTime(world);
		#if !MAKE_CSV
		WorldToStrings(*world);
		#endif
	}

}





//When an eater eats a plant, the plant should respawn somewhere else.
/*void RespawnPlant() {
	int x,y;
	Entity tmp_ent = Entity("plant");


	do {
		x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
	} while(world[x][y].type != "none");

	world[x][y] = tmp_ent;

}*/

//plant respawns near x1,y1
/*void RespawnPlantNearby(int x1, int y1) {
	int x2,y2;
	Entity tmp_ent = Entity("plant");

	do {
		x2 = (int)std::round( x1 + ((RANDOM_NUM-0.5)*(float)(4)));
		y2 = (int)std::round( y1 + ((RANDOM_NUM-0.5)*(float)(4)));
		if (x2 < 0) x2 = 0;
		if (y2 < 0) y2 = 0;
		if (x2 > WORLD_SIZE-1) x2 = WORLD_SIZE-1;
		if (y2 > WORLD_SIZE-1) y2 = WORLD_SIZE-1;

	} while(world[x2][y2].type != "none");

}*/

vector<string> WorldToStrings(array<array<Entity, WORLD_SIZE>, WORLD_SIZE> w) {
	int x,y;
	string ent_sym;
	vector<string> str_world = vector<string>(WORLD_SIZE, string(WORLD_SIZE, '.'));

	for (x=0; x< WORLD_SIZE; x++) {
		for (y=0; y< WORLD_SIZE; y++) {
			
			if (w[x][y].type == "eater") {
				ent_sym = "T";
				if (w[x][y].spec == 1) {
					ent_sym = "*";
				}

			} else if(w[x][y].type == "plant") { 
				ent_sym = "P";
			} else if(w[x][y].type == "none") {
				ent_sym = " ";
			} else { //error case
				ent_sym = "X";
			}

			str_world[y].replace(x, 1, ent_sym);

		}
	}

	std::cout << string(WORLD_SIZE, '-') << std::endl;
	for (int i = 0; i<WORLD_SIZE; i++) {
		std::cout << str_world[i] << std::endl;
	}
	std::cout << string(WORLD_SIZE, '-') << std::endl;
	std::cout << std::endl;

	return str_world;
}

void Mutate(bitset<CHROMO_LENGTH> &bits) {
	for (int i=0; i<CHROMO_LENGTH; i++) {
		if (RANDOM_NUM < MUTATION_RATE) {
			if (bits[i]) {
				bits[i] = 0;
			} else {
				bits[i] = 1;
			}
		}
	}

	return;
}

void Crossover(bitset<CHROMO_LENGTH> &offspring1, bitset<CHROMO_LENGTH> &offspring2) {
  bitset<CHROMO_LENGTH> t1,t2;
  int crossover;

  //dependent on the crossover rate
  if (RANDOM_NUM < CROSSOVER_RATE)
  {
    //create a random crossover point
    crossover = (int)(RANDOM_NUM*CHROMO_LENGTH);
	for (int i=0; i<CHROMO_LENGTH; i++) {
		if (i < crossover) {
			t1[i] = offspring1[i];
			t2[i] = offspring2[i];
		} else {
			t1[i] = offspring2[i];
			t2[i] = offspring1[i];
		}
	}
    offspring1 = t1; offspring2 = t2;				  

  }
}


bitset<CHROMO_LENGTH> Roulette(int total_fitness, vector<Entity> population) {
	//generate a random number between 0 & total fitness count
	float Slice = (float)(RANDOM_NUM * total_fitness);

	//go through the chromosones adding up the fitness so far
	float FitnessSoFar = 0.0f;
	
	for (vector<Entity>::iterator	 i = population.begin(); i != population.end(); ++i)
	{
		FitnessSoFar += i->genes.fitness;
		
		//if the fitness so far > random number return the chromo at this point
		if (FitnessSoFar >= Slice)
		{
			return i->genes.bits;
		}

	}

	//std::cout << "Failed to select a candidate. Crashing soon :)" << std::endl;
	return bitset<CHROMO_LENGTH>();
}

