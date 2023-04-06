#include "gen_alg_game.h"
#include "bitset_genetics.h"
#include <vector>

using std::vector;

GenAlgGame::GenAlgGame() {}

GenAlgGame::GenAlgGame(int eater_pop_size, int plant_pop_size) {

    bitset<CHROMO_LENGTH> genes1;
	Entity tmp_ent;
	Chromosome tmp_chrm;
    int x,y;

	this->world = new array<array<Entity, WORLD_SIZE>, WORLD_SIZE>;
	//clear out the world
	for (int s=0; s<WORLD_SIZE; s++) {
		(*world)[s].fill(Entity());
	}

    this->eater_pop_size = eater_pop_size;
    this->plant_pop_size = plant_pop_size;
    time_step = 0;
    gen = 0;

    for (int i=0; i<eater_pop_size; i++) {
		genes1.reset();

		for (int j=0; j<CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes1[j].flip();
			}
		}

		tmp_chrm = Chromosome(genes1, 1.0f, CHROMO_LENGTH);

		tmp_ent = Entity("eater", tmp_chrm);
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state
		tmp_ent.facing = bitset<2>( RANDOM_NUM_RANGE(4) );

		tmp_ent.genes.BitsToRules();

        GetRandomCoord(tmp_ent);
		
	}

	//create the plants and place them in the world
	for (int i=0; i<plant_pop_size; i++) {
		tmp_ent = Entity("plant");

		GetRandomCoord(tmp_ent);
	}
}

void GenAlgGame::GetRandomCoord(Entity ent) {
    int x,y;

    do {
        x = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
        y = (int)std::round((RANDOM_NUM*(float)(WORLD_SIZE-1)));
    } while((*world)[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

    (*world)[x][y] = ent;
}

void GenAlgGame::Generation() {

    vector<Entity> tmp_eater_population;
    vector<Entity> eater_population;
    int max_fitness,total_fitness;
    int x,y;

    gen++;

    tmp_eater_population.clear();
    eater_population.clear();

    for (int d=0; d<DAYS_PER_GENERATION; d++) {
        max_fitness = ProgressTime2();
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
    std::cout << gen << "," << max_fitness << "," << total_fitness/(float)(EATER_POP_SIZE) << "," << std::endl;
    #endif

    for (int i=0; i<EATER_POP_SIZE/2; i++) {
        bitset<CHROMO_LENGTH> genes1,genes2;
        Entity child1, child2;
	    Chromosome new_chrm1,new_chrm2;


        //Select two
        genes1 = Roulette(total_fitness, eater_population);
        genes2 = Roulette(total_fitness, eater_population);

        //crossover
        Crossover(genes1, genes2);
        
        //mutate
        Mutate(genes1);
        Mutate(genes2);

		//random deletion
		Deletion(genes1);
		Deletion(genes2);

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
        GetRandomCoord(tmp_eater_population[i]);
    }

    //create the plants and place them in the world
    for (int i=0; i<PLANT_POP_SIZE; i++) {
        GetRandomCoord(Entity("plant"));
    }

}

// Progress time in the world.
// Going left-to-right and top-to-bottom, check the entity in each grid square
// If they're an eater, check the 4 nearby grids for plants. Concat these 4 bits with
// the 4 bits of the current state, then key into the map to check the action and next state.
// Take the action (if possible), and change to the next state. Move on.
// Presently, plants do nothing and "none" squares do nothing
float GenAlgGame::ProgressTime() {
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
	
				for(int i=0; i<GENE_LENGTH; i++) {
					if (i<2) {
						mvmt[i] = rule[i];
					} else {
						(*world)[x][y].state[i-2] = rule[i];   //update the eater's state regardless
					}
				}

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
								RespawnPlantNearby(x-1,y);				
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
								RespawnPlantNearby(x-1,y);			
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
								RespawnPlantNearby(x-1,y);		
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
								RespawnPlantNearby(x-1,y);	
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


float GenAlgGame::ProgressTime2() {
    int x,y,m,xf,yf;
	bitset<4> view(0);
	bitset<2> action(0);
	bitset<GENE_LENGTH+2> rule_key(0);
	bitset<GENE_LENGTH> rule(0);
	float max_fitness = 0.0f;
	string facing_type;
	unsigned long face_dir;


	for (x=0; x<WORLD_SIZE; x++) {
		for (y=0; y<WORLD_SIZE; y++) {
			if((*world)[x][y].type == "eater" && (*world)[x][y].last_action != time_step) {
				(*world)[x][y].last_action = time_step;	//only one action allowed per day so keep these together

				face_dir = (*world)[x][y].facing.to_ulong();

				if (face_dir == 0) {
					xf = x;
					yf = y-1;
				} else if (face_dir == 1) {
					xf = x+1;
					yf = y;
				} else if (face_dir == 2) {
					xf = x;
					yf = y+1;
				} else if (face_dir ==3){
					xf = x-1;
					yf = y; 
				}

				view[0] = (*world)[x][y].facing[0];
				view[1] = (*world)[x][y].facing[1];

				
				if (xf < 0 || yf < 0 || xf>WORLD_SIZE-1 || yf>WORLD_SIZE-1) {  //must be looking at a wall
					view[2] = 0;
					view[3] = 0;
				} else {
					facing_type = (*world)[xf][yf].type;
				
					if(facing_type == "none") {
						view[2] = 1;
						view[3] = 0;
					} else if (facing_type == "eater") {
						view[2] = 0;
						view[3] = 1;

					} else if (facing_type == "plant") {
						view[2] = 1;
						view[3] = 1;
					}
 
				}

				for (int i=0; i<(2+GENE_LENGTH); i++) {
					

					//I think this first one is right, verify
					//#1
					if (i<STATE_SIZE) {
						rule_key[i] = view[i];
					} else {
						rule_key[i] = (*world)[x][y].state[i-STATE_SIZE];
					}
				
					//#2
					//if (i<STATE_SIZE) {
					//	rule_key[i] = (*world)[x][y].state[i];
					//} else {
					//	rule_key[i] = view[i-STATE_SIZE];
					//}
				}

				//std::cout << "rule key: " << rule_key.to_ulong() << std::endl;
				rule = (*world)[x][y].genes.rules[rule_key.to_ulong()];
				//std::cout << "rule: " << rule << std::endl;

				for(int i=0; i<GENE_LENGTH; i++) {
					if (i<2) {
						action[i] = rule[i];
					} else {
						(*world)[x][y].state[i-2] = rule[i];   //update the eater's state regardless
					}
				}

				m = action.to_ulong();
				//00 = forward, 01 = backward, 10 = +1 to facing, 11 = -1 to facing

				if (max_fitness < (*world)[x][y].genes.fitness) {
					max_fitness = (*world)[x][y].genes.fitness;
				}

				if (m==2) {
					(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()+1);
				} else if (m==3) {
					(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()-1);
				} else if ((view[2] || view[3]) && facing_type != "eater") { // (view[0] || view[1]) means they aren't looking at a wall
					
					(*world)[xf][yf] = (*world)[x][y]; 			//move the eater
					(*world)[x][y] = Entity();					//replace it with nothing

					if(facing_type == "plant") {
						(*world)[xf][yf].genes.fitness += 1.0;
						RespawnPlantNearby(xf,yf);
					}
				}
				
			}
		}

	}


	time_step++;	//increment time step
	return max_fitness;
}


void GenAlgGame::RespawnPlantNearby(int x1, int y1) {
    int x2,y2;
	Entity tmp_ent = Entity("plant");

	do {
		x2 = (int)std::round( x1 + ((RANDOM_NUM-0.5)*(float)(4)));
		y2 = (int)std::round( y1 + ((RANDOM_NUM-0.5)*(float)(4)));
		if (x2 < 0) x2 = 0;
		if (y2 < 0) y2 = 0;
		if (x2 > WORLD_SIZE-1) x2 = WORLD_SIZE-1;
		if (y2 > WORLD_SIZE-1) y2 = WORLD_SIZE-1;

	} while((*world)[x2][y2].type != "none");

	(*world)[x2][y2] = tmp_ent;
}