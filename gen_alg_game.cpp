#include "gen_alg_game.h"
#include "bitset_genetics.h"
#include <vector>

using std::vector;
using std::pair;

GenAlgGame::GenAlgGame() {}

GenAlgGame::GenAlgGame(int eater_pop_size, int plant_pop_size) {

    bitset<CHROMO_LENGTH> genes1;
	bitset<APEX_CHROMO_LENGTH> genes2;
	Entity tmp_ent;
	Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT> tmp_chrm;
	Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT> tmp_chrm2;
    int x,y;

	this->world = new array<array<Entity, WORLD_SIZE>, WORLD_SIZE>;
	//clear out the world
	for (int s=0; s<WORLD_SIZE; s++) {
		(*world)[s].fill(Entity());
	}

    this->eater_pop_size = eater_pop_size;
    this->plant_pop_size = plant_pop_size;

	//make empty eater pop vector
	eater_pop.assign(this->eater_pop_size, Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>());
	apex_pop.assign(APEX_POP_SIZE, Chromosome<APEX_GENE_LENGTH, APEX_GENE_COUNT>());

    time_step = 0;
    gen = 0;

    for (int i=0; i<eater_pop_size; i++) {
		genes1.reset();

		for (int j=0; j<CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes1[j].flip();
			}
		}

		tmp_chrm = Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>(genes1, 1.0f, CHROMO_LENGTH);

		tmp_ent = Entity("eater");
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state
		tmp_ent.facing = bitset<2>( RANDOM_NUM_RANGE(4) );

		tmp_chrm.BitsToRules();
		eater_pop.at(i) = tmp_chrm;

		tmp_ent.pop_index = i;
        GetRandomCoord(tmp_ent);
		
	}

	for (int i=0; i<APEX_POP_SIZE; i++) {
		genes2.reset();

		for (int j=0; j<APEX_CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes2[j].flip();
			}
		}

		tmp_chrm2 = Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>(genes2, 1.0f, APEX_CHROMO_LENGTH);

		tmp_ent = Entity("apex");
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state
		tmp_ent.facing = bitset<2>(0); //apex always faces up

		tmp_chrm2.BitsToRules();
		apex_pop.at(i) = tmp_chrm2;

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

    do {	//clamp to 1 and WORLD_SIZE-2
        x = 1 + RANDOM_NUM_RANGE(WORLD_SIZE-2);
        y = 1 + RANDOM_NUM_RANGE(WORLD_SIZE-2);
    } while((*world)[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

    (*world)[x][y] = ent;
}

void GenAlgGame::Generation() {

    vector<Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>> tmp_eater_population;
	vector<Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>> tmp_apex_population;
    int max_fitness_eaters = 0.0f,total_fitness_eaters = 0.0f;
	int max_fitness_apex = 0.0f, total_fitness_apex = 0.0f;
    int x,y;
	int current_pop_index;

    gen++;

	//make empty tmp eater population vector
    tmp_eater_population.assign(this->eater_pop_size, Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>());
	tmp_apex_population.assign(APEX_POP_SIZE, Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>());

    for (int d=0; d<DAYS_PER_GENERATION; d++) {
       	ProgressTime();
    }

	for (auto i = eater_pop.begin(); i < eater_pop.end(); ++i) {
		if (i->fitness > max_fitness_eaters) {
			max_fitness_eaters = i->fitness;
		}
		total_fitness_eaters += i->fitness;
	}

	for (auto i = apex_pop.begin(); i < apex_pop.end(); ++i) {
		if (i->fitness > max_fitness_apex) {
			max_fitness_apex = i->fitness;
		}
		total_fitness_apex += i->fitness;
	}


	//TODO: add apex to csv
    #if MAKE_CSV
    std::cout << gen << "," << max_fitness_eaters << "," << total_fitness_eaters/(float)(EATER_POP_SIZE) << "," << max_fitness_apex << "," << total_fitness_apex/(float)(APEX_POP_SIZE) << std::endl;
    #endif

	 //clear out the world
    for (int s=0; s<WORLD_SIZE; s++) {
        (*world)[s].fill(Entity());
    }


    for (int i=0; i<EATER_POP_SIZE/2; i++) {
        bitset<CHROMO_LENGTH> genes1,genes2;
        Entity child1, child2;
	    Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT> new_chrm1,new_chrm2;


        //Select two
        genes1 = Roulette<EATER_GENE_LENGTH, EATER_GENE_COUNT>(total_fitness_eaters, eater_pop);
        genes2 = Roulette<EATER_GENE_LENGTH, EATER_GENE_COUNT>(total_fitness_eaters, eater_pop);

        //crossover
        Crossover<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes1, genes2);
        
        //mutate
        Mutate<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes1);
        Mutate<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes2);

		//random deletion
		Deletion<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes1);
		Deletion<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes2);

        new_chrm1 = Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>(genes1, 1.0f, CHROMO_LENGTH); 
        new_chrm2 = Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>(genes2, 1.0f, CHROMO_LENGTH); 

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


	for (int i=0; i<APEX_POP_SIZE/2; i++) {
        bitset<APEX_CHROMO_LENGTH> genes3,genes4;
        Entity child1, child2;
	    Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT> new_chrm3,new_chrm4;


        //Select two
        genes3 = Roulette<APEX_GENE_LENGTH, APEX_GENE_COUNT>(total_fitness_apex, apex_pop);
        genes4 = Roulette<APEX_GENE_LENGTH, APEX_GENE_COUNT>(total_fitness_apex, apex_pop);

        //crossover
        Crossover<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes3, genes4);
        
        //mutate
        Mutate<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes3);
        Mutate<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes4);

		//random deletion
		Deletion<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes3);
		Deletion<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes4);

        new_chrm3 = Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>(genes3, 1.0f, CHROMO_LENGTH); 
        new_chrm4 = Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>(genes4, 1.0f, CHROMO_LENGTH); 

        //set up the new entity
        child1 = Entity("apex");
        child1.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child1.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        new_chrm3.BitsToRules();
        child2 = Entity("apex");
        child2.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child2.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        new_chrm4.BitsToRules();

        if (i==0) {
            child1.spec = 1;
        }
        
		child1.pop_index = 2*i;
		child2.pop_index = 2*i+1;

        apex_pop.at(2*i) = new_chrm3;
        apex_pop.at(2*i+1) = new_chrm4;

		//place the two new eaters into the empty world
		GetRandomCoord(child1);
		GetRandomCoord(child2);
    }
	apex_pop.clear();
	apex_pop = tmp_apex_population;

    //create the plants and place them in the world
    for (int i=0; i<PLANT_POP_SIZE; i++) {
        GetRandomCoord(Entity("plant"));
    }

}

void GenAlgGame::ProgressTime() {
    int x,y,m,xf,yf,xb,yb, current_pop_index;
	bitset<4> view(0);
	bitset<2> action(0);
	bitset<EATER_TRAITS+STATE_SIZE> eater_rule_key(0);
	bitset<EATER_GENE_LENGTH> eater_rule(0);
	bitset<APEX_TRAITS+STATE_SIZE> apex_rule_key(0);
	bitset<APEX_GENE_LENGTH> apex_rule(0);
	float max_fitness = 0.0f;
	string facing_type,behind_type;
	unsigned long face_dir;
	bool eater_found = false;

	array<int, 4> pts = {0,0,0,0};
	array<pair<int, int>, 4> near_eater = {pair<int,int>{0,0},pair<int,int>{0,0},pair<int,int>{0,0},pair<int,int>{0,0}};
	array<int, 4> near_eater_idx = {0,0,0,0};
	int prox = 10;
	int yoff, xoff;

	//std::cout << "day " << time_step << ": " << std::endl;

	for (x=0; x<WORLD_SIZE; x++) {
		for (y=0; y<WORLD_SIZE; y++) {
			//apex only acts every 4 days...
			if ((*world)[x][y].type == "apex" && (*world)[x][y].last_action != time_step && time_step%4==0) {
				current_pop_index = (*world)[x][y].pop_index;
				(*world)[x][y].last_action = time_step;

				pts[0] = x+prox;	//xf
				pts[1] = x-prox;	//xb
				pts[2] = y+prox;	//yf
				pts[3] = y-prox;	//yb

				for (int i=0; i<4; i++) {
					if (pts[i] < 1) pts[i] = 1;
					if (pts[i] > WORLD_SIZE-2) pts[i] = WORLD_SIZE-2;
				}

				/// could probably simplify these 4 into 1 loop?
				//check "right"
				for (int xc=0; xc<pts[0]-x; xc++) {
					if((*world)[xc+x][y].type == "eater") { 
						apex_rule_key[0] = 1; 
						near_eater[0] = pair<int,int>{x+xc, y};
						near_eater_idx[0] = (*world)[xc+x][y].pop_index;
						break;
					}
				}

				//check "left"
				for (int xc=0; xc<pts[1]-x; xc++) {
					if((*world)[x-xc][y].type == "eater") {
						apex_rule_key[1] = 1; 
						near_eater[1] = pair<int,int>{x-xc, y};
						near_eater_idx[1] = (*world)[x-xc][y].pop_index;
						break;
					}
				}

				//check "up"
				for (int yc=0; yc<pts[2]-y; yc++) {
					if((*world)[x][yc+y].type == "eater") {
						apex_rule_key[2] = 1;
						near_eater[2] = pair<int,int>{x, y+yc}; 
						near_eater_idx[2] = (*world)[x][y+yc].pop_index;
						break;
					}
				}

				//check "down"
				for (int yc=0; yc<pts[3]-y; yc++) {
					if((*world)[x][y-yc].type == "eater") {
						apex_rule_key[3] = 1; 
						near_eater[3] = pair<int,int>{x, y-yc};
						near_eater_idx[3] = (*world)[x][y-yc].pop_index;
						break;
					}
				}

				//std::cout << "nearby eater coords: " << near_eater[0].first <<" " << near_eater[0].second << std::endl;


				for (int i=APEX_TRAITS; i<(APEX_TRAITS+STATE_SIZE); i++) { // copy current state into key
					apex_rule_key[i] = (*world)[x][y].state[i-APEX_TRAITS];
				}
				//std::cout << "apex rule key " << apex_rule_key.to_string() << std::endl;
				apex_rule = apex_pop[current_pop_index].rules[apex_rule_key.to_ulong()];

				for(int i=0; i<APEX_GENE_LENGTH; i++) {
					if (i<2) {
						action[i] = apex_rule[i];
					} else {
						(*world)[x][y].state[i-2] = apex_rule[i];
					}
				}

				m = action.to_ulong();
				//00 = go to first eater, 01 = go to second eater, 10 = ...etc, 11 = 
				if(m==0) {
					yoff=0;
					xoff=-1;
				} else if (m==1) {
					yoff=0;
					xoff=1;
				} else if (m==2) {
					yoff=-1;
					xoff=0;
				} else if (m==3) {
					yoff=1;
					xoff=0;
				}

				//this is a terribly messy bit of a game logic here--let's rethink the apex's
				//behavior perhaps
				//make sure the pairs have actually been filled (i.e., it found an eater in the given dir)
				if (near_eater[m].first != 0 && near_eater[m].second != 0) {
					int xe = near_eater[m].first + xoff;
					int ye = near_eater[m].second + yoff;
					int current_eater_pop_index = near_eater_idx[m];
					//move the apex near the eater
					(*world)[xe][ye] = (*world)[x][y]; 	
					(*world)[x][y] = Entity();

					//add to the apex's fitness
					apex_pop[current_pop_index].fitness += eater_pop[current_eater_pop_index].fitness / 4.0;

					//take from the eater's fitness
					eater_pop[current_eater_pop_index].fitness -= eater_pop[current_eater_pop_index].fitness / 4.0;
				}
				//std::cout << "Test" << std::endl;



		 	} else if( (*world)[x][y].type == "eater" && (*world)[x][y].last_action != time_step) {
				
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

				
				if (xf < 1 || yf < 1 || xf>WORLD_SIZE-2 || yf>WORLD_SIZE-2) {  //must be looking at a wall
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

				if (xb < 1 || yb < 1 || xb>WORLD_SIZE-2 || yb>WORLD_SIZE-2) {
					behind_type = "wall";
				} else {
					behind_type = (*world)[xb][yb].type;
				}

				for (int i=0; i<(EATER_TRAITS+STATE_SIZE); i++) {
					if (i==0) { //lowest bit = is there an apex near me
						if (facing_type == "apex" || behind_type == "apex"){
							eater_rule_key[i] = 1;
						} else {
							eater_rule_key[i] = 0;
						}
					} else if (i>0 && i<EATER_TRAITS) { //next 4 bits = what can i see + where am i looking
						eater_rule_key[i] = view[i];
					} else { // last STATE_SIZE is the state
						eater_rule_key[i] = (*world)[x][y].state[i-EATER_TRAITS];
					}
				}

				//std::cout << "eater_rule key: " << eater_rule_key.to_ulong() << std::endl;
				eater_rule = eater_pop[current_pop_index].rules[eater_rule_key.to_ulong()];
				//std::cout << "eater_rule: " << eater_rule << std::endl;

				for(int i=0; i<EATER_GENE_LENGTH; i++) {
					if (i<2) {
						action[i] = eater_rule[i];
					} else {
						(*world)[x][y].state[i-2] = eater_rule[i];   //update the eater's state regardless
					}
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
				} else if ((view[2] || view[3]) && facing_type != "eater") { // (view[2] || view[3]) means they aren't looking at a wall
					
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
	//return max_fitness;
}

void GenAlgGame::RespawnPlantNearby(int x1, int y1) {
    int x2,y2;

	do {
		x2 = (int)std::round( x1 + ((RANDOM_NUM-0.5)*(float)(4)));
		y2 = (int)std::round( y1 + ((RANDOM_NUM-0.5)*(float)(4)));
		if (x2 < 1) x2 = 1;
		if (y2 < 1) y2 = 1;
		if (x2 > WORLD_SIZE-2) x2 = WORLD_SIZE-2;
		if (y2 > WORLD_SIZE-2) y2 = WORLD_SIZE-2;

	} while((*world)[x2][y2].type != "none");

	(*world)[x2][y2] = Entity("plant");;
}