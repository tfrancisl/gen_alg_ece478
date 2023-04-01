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

using std::string;
using std::vector;
using std::array;

#define CROSSOVER_RATE            	0.7
#define MUTATION_RATE             	0.001
#define EATER_POP_SIZE              50
#define PLANT_POP_SIZE              250


//#define GENE_COUNT             	  	4*256
//#define STATE_SIZE                  4
//#define GENE_LENGTH               	(2+STATE_SIZE)
//#define CHROMO_LENGTH				(GENE_COUNT)*(GENE_LENGTH)

#define WORLD_SIZE 					25
#define DAYS_PER_GENERATION 		365
#define GENERATIONS					1001


//returns a float between 0 & 1
#define RANDOM_NUM		((float)rand()/((float)(RAND_MAX)+1))

string  		GetRandomBits(int length);
string  	Roulette(int total_fitness, vector<Entity> population);

void    		Mutate(string &bits);
void   			Crossover(string &offspring1, string &offspring2);
int 			GetRandomLocation();
vector<string> WorldToStrings(Entity world[WORLD_SIZE][WORLD_SIZE]);
float 			ProgressTime();


int time_step = 0; //current "day"
//Entity world[WORLD_SIZE][WORLD_SIZE];
array<array<Entity, WORLD_SIZE>, WORLD_SIZE> world;


int main() {
    //seed the random number generator
	srand((int)time(NULL));
	int x,y;
	float max_fitness = 0.0f,total_fitness = 0.0f,last_ten_fitness = 0.0f;

	
	//Entity eater_population[EATER_POP_SIZE];
	//Entity plant_population[PLANT_POP_SIZE];
	vector<Entity> eater_population(EATER_POP_SIZE);
	vector<Entity> tmp_eater_population(EATER_POP_SIZE);

	string genes1,genes2;
	Entity tmp_ent, child1, child2;
	Chromosome tmp_chrm,new_chrm1,new_chrm2;

	vector<string> str_world;


	//create the eaters and place them in the world
	for (int i=0; i<EATER_POP_SIZE; i++) {


		tmp_chrm = Chromosome(GetRandomBits(CHROMO_LENGTH), 0.0f, CHROMO_LENGTH);

		tmp_ent = Entity("eater", tmp_chrm);
		tmp_ent.state = GetRandomBits(STATE_SIZE);	//need a random starting state
		tmp_ent.genes.BitsToRules();

		do {
			x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		} while(world[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

		world[x][y] = tmp_ent;

		/*if (i==0){
			for(int j=0; j<GENE_COUNT; j++) {
				std::cout << j << ": " << tmp_ent.genes.rules[std::bitset<4+STATE_SIZE>(j).to_string()] << std::endl;
			}
		}*/
	}

	//create the plants and place them in the world
	for (int i=0; i<PLANT_POP_SIZE; i++) {
		tmp_ent = Entity("plant");

		do {
			x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		} while(world[x][y].type != "none");

		world[x][y] = tmp_ent;
	}

	//do one generation

	/////////////////////////////////////////////////////////
	for(int g=0; g<GENERATIONS; g++) {
		tmp_eater_population.clear();
		eater_population.clear();

		for (int d=0; d<DAYS_PER_GENERATION; d++) {
			max_fitness = ProgressTime();
		}

		

		total_fitness = 0.0f;
		for (x=0; x<WORLD_SIZE; x++) {
			for (y=0; y<WORLD_SIZE; y++) {
				if(world[x][y].type == "eater") {
					total_fitness += world[x][y].genes.fitness;
					eater_population.push_back(world[x][y]);
				}
			}
		}
		last_ten_fitness += total_fitness;

		//std::cout << "Max fitness after generation " << g << ": " << max_fitness << std::endl;
		if (g!=0 && g%10 == 0) { 
			//std::cout << "Generation " << g << ": " << max_fitness << " max fitness, " << total_fitness/(float)EATER_POP_SIZE << " avg fitness" << std::endl;
			std::cout << "Generation " << g << ": " << max_fitness << " max fitness, " << last_ten_fitness/(float)(10.0*EATER_POP_SIZE) << " avg fitness (10 gens)" << std::endl;
			last_ten_fitness = 0.0f;
		} else if (g<10) {
			std::cout << "Generation " << g << ": " << max_fitness << " max fitness, " << total_fitness/(float)(EATER_POP_SIZE) << " avg fitness" << std::endl;
		}
		//std::cout << "Total fitness in generation " << g << ": " << total_fitness << std::endl;

		for (int i=0; i<EATER_POP_SIZE/2; i++) {
			//Select two
			genes1 = Roulette(total_fitness, eater_population);
			genes2 = Roulette(total_fitness, eater_population);

			//crossover
			Crossover(genes1, genes2);
			
			//mutate
			Mutate(genes1);
			Mutate(genes2);

			new_chrm1 = Chromosome(genes1, 0.0f, CHROMO_LENGTH); 
			new_chrm2 = Chromosome(genes2, 0.0f, CHROMO_LENGTH); 

			//set up the new entity
			child1 = Entity("eater", new_chrm1);
			child1.state = GetRandomBits(STATE_SIZE);
			child1.genes.BitsToRules();
			child2 = Entity("eater", new_chrm2);
			child2.state = GetRandomBits(STATE_SIZE);
			child2.genes.BitsToRules();
			
			tmp_eater_population.push_back(child1);
			tmp_eater_population.push_back(child2);
		}

		//clear out the world?
		world = array<array<Entity, WORLD_SIZE>, WORLD_SIZE>();

		//place the new population into the world
		for (int i=0; i<EATER_POP_SIZE; i++) {
			do {
				x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
				y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			} while(world[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

			world[x][y] = tmp_eater_population[i];
		}


		//create the plants and place them in the world
		for (int i=0; i<PLANT_POP_SIZE; i++) {
			tmp_ent = Entity("plant");

			do {
				x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
				y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
			} while(world[x][y].type != "none");

			world[x][y] = tmp_ent;
		}
	}
	///////////////////////////////////////////////////////////////


}


//When an eater eats a plant, the plant should respawn somewhere else.
void RespawnPlant() {
	int x,y;
	Entity tmp_ent = Entity("plant");

	do {
		x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
		y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
	} while(world[x][y].type != "none");

	world[x][y] = tmp_ent;

}

// Progress time in the world.
// Going left-to-right and top-to-bottom, check the entity in each grid square
// If they're an eater, check the 4 nearby grids for plants. Concat these 4 bits with
// the 4 bits of the current state, then key into the map to check the action and next state.
// Take the action (if possible), and change to the next state. Move on.
// Presently, plants do nothing and "none" squares do nothing
float ProgressTime() {
	int x,y,m;
	string view,rule_key,rule,mvmt;
	//	bit     3    2    1    0
 	//  wpos    7    5    3    1
	//  x       -    0    +    0
	//  y       0    +    0    -
	std::bitset<4> view_bits(0); 
	float max_fitness = 0.0f;

	//std::cout << "time: " << time_step << std::endl;

	for (x=0; x<WORLD_SIZE; x++) {
		for (y=0; y<WORLD_SIZE; y++) {
			if(world[x][y].type == "eater" && world[x][y].last_action != time_step) {

				//std::cout << "time: " << time_step << std::endl;
				//std::cout << "current ent last action: " << world[x][y].last_action << std::endl;
				world[x][y].last_action = time_step;	//only one action allowed per day so keep these together

				if (x != WORLD_SIZE-1) {
					if (world[x+1][y].type == "plant")
						view_bits.set(1);
				} else if (x != 0) {
					if (world[x-1][y].type == "plant")
						view_bits.set(3);
				}
				if (y != WORLD_SIZE-1) {
					if (world[x][y+1].type == "plant")
						view_bits.set(2);
				} else if (y != 0) {
					if (world[x][y-1].type == "plant")
						view_bits.set(0);
				}


				view = view_bits.to_string();
				rule_key = view + world[x][y].state; 	//thanks string concatenation

				//std::cout << "rule key: " << rule_key << std::endl;
				rule = world[x][y].genes.rules[rule_key];
				//std::cout << "rule: " << rule << std::endl;

				mvmt = rule.substr(0, 2);			 	//get movement out of the rule
				world[x][y].state = rule.substr(2, STATE_SIZE);	//update the eater's state regardless

				m = std::stoi(mvmt, NULL, 2);
				//std::cout << m << std::endl;

				if (max_fitness < world[x][y].genes.fitness) {
					max_fitness = world[x][y].genes.fitness;
				}

				//should find some alternative logic here - switch/case/break is not great
				switch(m) {
					case 0:
						if( !(y-1 < 0) ) {
							if(world[x][y-1].type == "plant") {
								world[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								world[x][y-1] = world[x][y]; 			//move the eater
								world[x][y] = Entity();	
								RespawnPlant();				
							} else if(world[x][y-1].type == "none") {
								world[x][y-1] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
							}
						}	
						break;
					case 1:
						if( !(x+1 > WORLD_SIZE-1) )  {
							if(world[x+1][y].type == "plant") {
								world[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								world[x+1][y] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
								RespawnPlant();
							} else if(world[x+1][y].type == "none") {
								world[x+1][y] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
							}
						}
						break;
					case 2:
						if( !(y+1 > WORLD_SIZE-1) ) {
							if(world[x][y+1].type == "plant") {
								world[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								world[x][y+1] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
								RespawnPlant();
							} else if(world[x][y+1].type == "none") {
								world[x][y+1] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
							}
						}
						break;
					case 3:
						if( !(x-1 < 0) ) {
							if(world[x-1][y].type == "plant") {
								world[x][y].genes.fitness += 1.0; 		//ate a plant, fitness up
								world[x-1][y] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
								RespawnPlant();
							} else if(world[x-1][y].type == "none") {
								world[x-1][y] = world[x][y]; 			//move the eater
								world[x][y] = Entity();					//replace the eater with nothing
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


vector<string> WorldToStrings(Entity w[WORLD_SIZE][WORLD_SIZE]) {
	int x,y;
	string ent_sym;
	vector<string> str_world = vector<string>(WORLD_SIZE, string(WORLD_SIZE, '.'));

	for (x=0; x< WORLD_SIZE; x++) {
		for (y=0; y< WORLD_SIZE; y++) {
			
			if (w[x][y].type == "eater") {
				ent_sym = "T";
			} else if(w[x][y].type == "plant") { 
				ent_sym = "P";
			} else if(w[x][y].type == "none") {
				ent_sym = ".";
			} else { //error case
				ent_sym = "X";
			}

			str_world[y].replace(x, 1, ent_sym);

		}
	}

	for (int i = 0; i<WORLD_SIZE; i++) {
		std::cout << str_world[i] << std::endl;
	}
	std::cout << std::endl;

	return str_world;
}


string	GetRandomBits(int length) {
	string bits;

	for (int i=0; i<length; i++)
	{
		if (RANDOM_NUM > 0.5f)

			bits += "1";

		else

			bits += "0";
	}

	return bits;
}

void Mutate(string &bits) {
	for (int i=0; i<bits.length(); i++) {
		if (RANDOM_NUM < MUTATION_RATE) {
			if (bits.at(i) == '1')

				bits.at(i) = '0';

			else

				bits.at(i) = '1';
		}
	}

	return;
}

void Crossover(string &offspring1, string &offspring2) {
  //dependent on the crossover rate
  if (RANDOM_NUM < CROSSOVER_RATE)
  {
    //create a random crossover point
    int crossover = (int) (RANDOM_NUM * CHROMO_LENGTH);

    string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMO_LENGTH);
    string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMO_LENGTH);

    offspring1 = t1; offspring2 = t2;				  
  }
}


string Roulette(int total_fitness, vector<Entity> population) {
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

	std::cout << "Failed to select a candidate. Crashing soon :)" << std::endl;
	return "";
}

