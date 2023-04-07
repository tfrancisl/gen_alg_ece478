#include "gen_alg_game.h"
#include "bitset_genetics.h"
#include <vector>

using std::vector;

GenAlgGame::GenAlgGame() {}

GenAlgGame::GenAlgGame(int eater_pop_size, int plant_pop_size) {

    bitset<CHROMO_LENGTH> genes1;
	Entity tmp_ent;
	Chromosome<GENE_LENGTH,GENE_COUNT> tmp_chrm;
    int x,y;

	this->world = new array<array<Entity, WORLD_SIZE>, WORLD_SIZE>;
	//clear out the world
	for (int s=0; s<WORLD_SIZE; s++) {
		(*world)[s].fill(Entity());
	}

    this->eater_pop_size = eater_pop_size;
    this->plant_pop_size = plant_pop_size;

	//make empty eater pop vector
	eater_pop.assign(this->eater_pop_size, Chromosome<GENE_LENGTH,GENE_COUNT>());

    time_step = 0;
    gen = 0;

    for (int i=0; i<eater_pop_size; i++) {
		genes1.reset();

		for (int j=0; j<CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes1[j].flip();
			}
		}

		tmp_chrm = Chromosome<GENE_LENGTH,GENE_COUNT>(genes1, 1.0f, CHROMO_LENGTH);

		tmp_ent = Entity("eater");
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state
		tmp_ent.facing = bitset<2>( RANDOM_NUM_RANGE(4) );

		tmp_chrm.BitsToRules();
		eater_pop.at(i) = tmp_chrm;

		tmp_ent.pop_index = i;
        GetRandomCoord(tmp_ent);
		
	}

	//create the plants and place them in the world
	for (int i=0; i<plant_pop_size; i++) {
		GetRandomCoord(Entity("plant"));
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

    vector<Chromosome<GENE_LENGTH,GENE_COUNT>> tmp_eater_population;
    int max_fitness,total_fitness;
    int x,y;
	int current_pop_index;

    gen++;

	//make empty tmp eater population vector
    tmp_eater_population.assign(this->eater_pop_size, Chromosome<GENE_LENGTH,GENE_COUNT>());;

    for (int d=0; d<DAYS_PER_GENERATION; d++) {
        max_fitness = ProgressTime2();
    }

    total_fitness = 0.0f;
	for (auto i = eater_pop.begin(); i < eater_pop.end(); ++i) {
		total_fitness += i->fitness;
	}


    #if MAKE_CSV
    std::cout << gen << "," << max_fitness << "," << total_fitness/(float)(EATER_POP_SIZE) << "," << std::endl;
    #endif

	 //clear out the world
    for (int s=0; s<WORLD_SIZE; s++) {
        (*world)[s].fill(Entity());
    }


    for (int i=0; i<EATER_POP_SIZE/2; i++) {
        bitset<CHROMO_LENGTH> genes1,genes2;
        Entity child1, child2;
	    Chromosome<GENE_LENGTH,GENE_COUNT> new_chrm1,new_chrm2;


        //Select two
        genes1 = Roulette(total_fitness, eater_pop);
        genes2 = Roulette(total_fitness, eater_pop);

        //crossover
        Crossover(genes1, genes2);
        
        //mutate
        Mutate(genes1);
        Mutate(genes2);

		//random deletion
		Deletion(genes1);
		Deletion(genes2);

        new_chrm1 = Chromosome<GENE_LENGTH,GENE_COUNT>(genes1, 1.0f, CHROMO_LENGTH); 
        new_chrm2 = Chromosome<GENE_LENGTH,GENE_COUNT>(genes2, 1.0f, CHROMO_LENGTH); 

        //set up the new entity
        child1 = Entity("eater");
        child1.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child1.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        new_chrm1.BitsToRules();
        child2 = Entity("eater");
        child2.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child2.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        new_chrm2.BitsToRules();

        if (i==0) {
            child1.spec = 1;
        }
        
		child1.pop_index = 2*i;
		child2.pop_index = 2*i+1;

        tmp_eater_population.at(2*i) = new_chrm1;
        tmp_eater_population.at(2*i+1) = new_chrm2;

		//place the two new eaters into the empty world
		GetRandomCoord(child1);
		GetRandomCoord(child2);
    }
	eater_pop.clear();
	eater_pop = tmp_eater_population;

    //create the plants and place them in the world
    for (int i=0; i<PLANT_POP_SIZE; i++) {
        GetRandomCoord(Entity("plant"));
    }

}

float GenAlgGame::ProgressTime2() {
    int x,y,m,xf,yf,xb,yb, current_pop_index;
	bitset<4> view(0);
	bitset<2> action(0);
	bitset<GENE_LENGTH+2> rule_key(0);
	bitset<GENE_LENGTH> rule(0);
	float max_fitness = 0.0f;
	string facing_type,behind_type;
	unsigned long face_dir;


	//std::cout << "day " << time_step << ": " << std::endl;

	for (x=0; x<WORLD_SIZE; x++) {
		for (y=0; y<WORLD_SIZE; y++) {
			if( (*world)[x][y].type == "eater" && (*world)[x][y].last_action != time_step) {
				
				current_pop_index = (*world)[x][y].pop_index;
				//std::cout << "pop index: " << current_pop_index << std::endl;
				(*world)[x][y].last_action = time_step;	//only one action allowed per day so keep these together

				face_dir = (*world)[x][y].facing.to_ulong();

				if (face_dir == 0) {
					xf = x;
					xb = x;
					yf = y-1;
					yb = y+1;
				} else if (face_dir == 1) {
					xf = x+1;
					xb = x-1;
					yf = y;
					yb = y;
				} else if (face_dir == 2) {
					xf = x;
					xb = x;
					yf = y+1;
					yb = y-1;
				} else if (face_dir == 3){
					xf = x-1;
					xb = x+1;
					yf = y; 
					yb = y; 
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

				if (xb < 0 || yb < 0 || xb>WORLD_SIZE-1 || yb>WORLD_SIZE-1) {  //must be looking at a wall
					behind_type = "wall";
				} else {
					behind_type = (*world)[xb][yb].type;
				}

				for (int i=0; i<(2+GENE_LENGTH); i++) {
					

					//I think this first one is right, verify
					//#1
					if (i<4) {
						rule_key[i] = view[i];
					} else {
						rule_key[i] = (*world)[x][y].state[i-STATE_SIZE];
					}
				
					//if (i<STATE_SIZE) {
					//	rule_key[i] = (*world)[x][y].state[i];
					//} else {
					//	rule_key[i] = view[i-STATE_SIZE];
					//}
				}

				//std::cout << "rule key: " << rule_key.to_ulong() << std::endl;
				rule = eater_pop[current_pop_index].rules[rule_key.to_ulong()];
				//std::cout << "rule: " << rule << std::endl;

				for(int i=0; i<GENE_LENGTH; i++) {
					if (i<2) {
						action[i] = rule[i];
					} else {
						(*world)[x][y].state[i-2] = rule[i];   //update the eater's state regardless
					}

					//if (i<STATE_SIZE) {
					//	(*world)[x][y].state[i] = rule[i];
					//} else {
					//	action[i-STATE_SIZE] = rule[i];
					//}
				}

				m = action.to_ulong();
				//std::cout << m << std::endl;
				//00 = forward, 01 = backward, 10 = +1 to facing, 11 = -1 to facing

				if (max_fitness < eater_pop[current_pop_index].fitness) {
					max_fitness = eater_pop[current_pop_index].fitness;
				}

				if (m==2) {
					(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()+1);
				} else if (m==3) {
					(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()-1);
				} else if ((view[2] || view[3]) && facing_type != "eater") { // (view[0] || view[1]) means they aren't looking at a wall
					
					//move the eater
					if (m == 0) {
						(*world)[xf][yf] = (*world)[x][y]; 	
						(*world)[x][y] = Entity();	

						if(facing_type == "plant") {
							eater_pop[current_pop_index].fitness += 1.0;
							//GetRandomCoord(Entity("plant"));
							RespawnPlantNearby(xf,yf);
						}

					} else if (behind_type != "wall" && behind_type != "eater") {	//unfortunately I have to manually check the grid behind them
						(*world)[xb][yb] = (*world)[x][y]; 
						(*world)[x][y] = Entity();	

						if(behind_type == "plant") {
							eater_pop[current_pop_index].fitness += 1.0;
							//GetRandomCoord(Entity("plant"));
							RespawnPlantNearby(xb,yb);
						}
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

	do {
		x2 = (int)std::round( x1 + ((RANDOM_NUM-0.5)*(float)(4)));
		y2 = (int)std::round( y1 + ((RANDOM_NUM-0.5)*(float)(4)));
		if (x2 < 0) x2 = 0;
		if (y2 < 0) y2 = 0;
		if (x2 > WORLD_SIZE-1) x2 = WORLD_SIZE-1;
		if (y2 > WORLD_SIZE-1) y2 = WORLD_SIZE-1;

	} while((*world)[x2][y2].type != "none");

	(*world)[x2][y2] = Entity("plant");;
}