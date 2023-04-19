#include "gen_alg_game.h"
#include "bitset_genetics.h"
#include <vector>
#include <chrono>

using std::vector;
using std::pair;

GenAlgGame::GenAlgGame() {}

GenAlgGame::GenAlgGame(int eater_pop_size, int plant_pop_size, int apex_pop_size) {

    bitset<CHROMO_LENGTH> genes1;
	bitset<APEX_CHROMO_LENGTH> genes2;
	Entity tmp_ent;
	Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT> tmp_chrm;
	Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT> tmp_chrm2;
    int x,y;
	int ax,ay;
	int current_row = 0;
	int apex_per_row = (apex_pop_size)/4;

	this->world = new array<array<Entity, WORLD_SIZE>, WORLD_SIZE>;
	//clear out the world
	for (int s=0; s<WORLD_SIZE; s++) {
		(*world)[s].fill(Entity());
	}

    this->eater_pop_size = eater_pop_size;
    this->plant_pop_size = plant_pop_size;
	this->apex_pop_size = apex_pop_size;

	//make empty eater pop vector
	eater_pop.assign(this->eater_pop_size, Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>());
	apex_pop.assign(this->apex_pop_size, Chromosome<APEX_GENE_LENGTH, APEX_GENE_COUNT>());

    time_step = 0;
    gen = 0;

	//apex must go first as they go in very specific locations
	for (int i=0; i<this->apex_pop_size; i++) {
		genes2.reset();

		for (int j=0; j<APEX_CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes2[j].flip();
			}
		}

		if (i==apex_per_row) current_row++;

		tmp_chrm2 = Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>(genes2, 2.5f, APEX_CHROMO_LENGTH);

		tmp_ent = Entity("apex", true);
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state
		tmp_ent.facing = bitset<2>( RANDOM_NUM_RANGE(4) );

		apex_pop.at(i) = tmp_chrm2;

		tmp_ent.pop_index = i;
		
		// Place the apex's evenly in 4 rows
		ay = ((2*current_row+1)*(WORLD_SIZE-1))/5;

		ax = ((i)%(apex_per_row))*((WORLD_SIZE-1)/(apex_per_row)) + (WORLD_SIZE/10);

		(*world)[ax][ay] = tmp_ent;


	}

    for (int i=0; i<eater_pop_size; i++) {
		genes1.reset();

		for (int j=0; j<CHROMO_LENGTH; j++) {
			if (RANDOM_NUM < 0.5) {
				genes1[j].flip();
			}
		}

		tmp_chrm = Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>(genes1, 2.5f, CHROMO_LENGTH);

		tmp_ent = Entity("eater", true);
		tmp_ent.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)));	//need a random starting state
		tmp_ent.facing = bitset<2>( RANDOM_NUM_RANGE(4) );

		eater_pop.at(i) = tmp_chrm;

		tmp_ent.pop_index = i;
        GetRandomCoord(tmp_ent, WORLD_SIZE/10);
		
	}

	//create the plants and place them in the world
	for (int i=0; i<plant_pop_size; i++) {
		GetRandomCoord(Entity("plant", false), WORLD_SIZE/5);
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

void GenAlgGame::GetRandomCoord(Entity ent, int r) {
    int x,y;

    do {	//clamp to r and WORLD_SIZE-r
        x = r + RANDOM_NUM_RANGE(WORLD_SIZE-r);
        y = r + RANDOM_NUM_RANGE(WORLD_SIZE-r);
    } while((*world)[x][y].type != "none"); //keep getting a random coordinate pair if an entity exists at the present location

    (*world)[x][y] = ent;
}

void GenAlgGame::SetGeneticParams(float crossover_rate, float mutation_rate, float deletion_rate) { 
	this->crossover_rate = crossover_rate;
	this->mutation_rate = mutation_rate;
	this->deletion_rate = deletion_rate;
}

void GenAlgGame::Generation(int days) {

    vector<Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>> tmp_eater_population;
	vector<Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>> tmp_apex_population;
    int max_fitness_eaters = 0.0f,total_fitness_eaters = 0.0f;
	int max_fitness_apex = 0.0f, total_fitness_apex = 0.0f;
    int x,y,ax,ay;
	int current_pop_index;
	int current_row = 0;
	int apex_per_row = this->apex_pop_size/4;
	int total_plants = 0;

    gen++;

	//make empty tmp eater population vector
    tmp_eater_population.assign(this->eater_pop_size, Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>());
	tmp_apex_population.assign(this->apex_pop_size, Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>());

    for (int d=0; d<(days); d++) {
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

	//clear out the world
    for (int s=0; s<WORLD_SIZE; s++) {
		for (auto itr = (*world)[s].begin(); itr != (*world)[s].end(); ++itr) {
			if(itr->type == "plant") total_plants++;
		}

        (*world)[s].fill(Entity());
    }

    //#if MAKE_CSV
    //std::cout << gen << "," << max_fitness_eaters << "," << total_fitness_eaters/(float)(this->eater_pop_size) << "," << max_fitness_apex << "," << total_fitness_apex/(float)(this->apex_pop_size) << "," << total_plants << std::endl;
    //#endif

	max_eater_fitness.push_back(max_fitness_eaters);
	max_apex_fitness.push_back(max_fitness_apex);
	avg_eater_fitness.push_back(total_fitness_eaters/(float)(this->eater_pop_size));
	avg_apex_fitness.push_back(total_fitness_apex/(float)(this->apex_pop_size));
	plant_count.push_back(total_plants);

	//this->csv_file << gen << "," << max_fitness_eaters << "," << total_fitness_eaters/(float)(this->eater_pop_size) << "," << max_fitness_apex << "," << total_fitness_apex/(float)(this->apex_pop_size) << "," << total_plants << "\n";


	for (int i=0; i<(this->apex_pop_size)/2; i++) {
        bitset<APEX_CHROMO_LENGTH> genes3,genes4;
        Entity child1, child2;
	    Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT> new_chrm3,new_chrm4;

        //Select two
        genes3 = Roulette<APEX_GENE_LENGTH, APEX_GENE_COUNT>(total_fitness_apex, apex_pop);
        genes4 = Roulette<APEX_GENE_LENGTH, APEX_GENE_COUNT>(total_fitness_apex, apex_pop);

        //crossover
        Crossover<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes3, genes4, this->crossover_rate);
        
        //mutate
        Mutate<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes3, this->mutation_rate);
        Mutate<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes4, this->mutation_rate);

		//random deletion
		//Deletion<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes3, this->deletion_rate);
		//Deletion<APEX_GENE_LENGTH, APEX_GENE_COUNT>(genes4, this->deletion_rate);

        new_chrm3 = Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>(genes3, 2.5f, CHROMO_LENGTH); 
        new_chrm4 = Chromosome<APEX_GENE_LENGTH,APEX_GENE_COUNT>(genes4, 2.5f, CHROMO_LENGTH); 

        //set up the new entity
        child1 = Entity("apex", true);
        child1.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child1.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        child2 = Entity("apex", true);
        child2.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child2.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        
		child1.pop_index = 2*i;
		child2.pop_index = 2*i+1;

        apex_pop.at(2*i) = new_chrm3;
        apex_pop.at(2*i+1) = new_chrm4;

		ax = ((i)%(apex_per_row))*((WORLD_SIZE-1)/(apex_per_row)) + 5;
		(*world)[ax][((2*current_row+1)*(WORLD_SIZE-1))/5] = child1;
		(*world)[ax][((2*current_row+2)*(WORLD_SIZE-1))/5] = child2;

		if((i%apex_per_row)==0) {
			current_row = ++current_row % 2;
		}
    }
	apex_pop.clear();
	apex_pop = tmp_apex_population;

    for (int i=0; i<(this->eater_pop_size)/2; i++) {
        bitset<CHROMO_LENGTH> genes1,genes2;
        Entity child1, child2;
	    Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT> new_chrm1,new_chrm2;


        //Select two
        genes1 = Roulette<EATER_GENE_LENGTH, EATER_GENE_COUNT>(total_fitness_eaters, eater_pop);
        genes2 = Roulette<EATER_GENE_LENGTH, EATER_GENE_COUNT>(total_fitness_eaters, eater_pop);

        //crossover
        Crossover<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes1, genes2, this->crossover_rate);
        
        //mutate
        Mutate<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes1, this->mutation_rate);
        Mutate<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes2, this->mutation_rate);

		//random deletion
		//Deletion<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes1, this->deletion_rate);
		//Deletion<EATER_GENE_LENGTH, EATER_GENE_COUNT>(genes2, this->deletion_rate);

        new_chrm1 = Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>(genes1, 2.5f, CHROMO_LENGTH); 
        new_chrm2 = Chromosome<EATER_GENE_LENGTH,EATER_GENE_COUNT>(genes2, 2.5f, CHROMO_LENGTH); 

        //set up the new entity
        child1 = Entity("eater", true);
        child1.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child1.facing = bitset<2>( RANDOM_NUM_RANGE(4) );
        child2 = Entity("eater", true);
        child2.state = bitset<STATE_SIZE>( RANDOM_NUM_RANGE(pow(2,STATE_SIZE)) );
		child2.facing = bitset<2>( RANDOM_NUM_RANGE(4) );

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
    for (int i=0; i<(this->plant_pop_size); i++) {
        GetRandomCoord(Entity("plant", false), WORLD_SIZE/5);
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

	for (x=0; x<WORLD_SIZE; x++) {
		for (y=0; y<WORLD_SIZE; y++) {
			if ( (*world)[x][y].is_creature ) {
				if ((*world)[x][y].type == "apex" && (*world)[x][y].last_action != time_step) {
					int x1,y1,x2,y2,px,py;
					int eater_count = 0;
					bitset<4> etrs;

					// Grab the current apex's population index, increment its action
					current_pop_index = (*world)[x][y].pop_index;
					(*world)[x][y].last_action = time_step;

					face_dir = (*world)[x][y].facing.to_ulong();
					
					if (face_dir == 0 || face_dir == 2) {	//up
						x1 = x-2;
						x2 = x+2;
						y1 = y-2;
						y2 = y;
					} else if (face_dir == 1 || face_dir == 3) {		// down
						x1 = x-2;
						x2 = x+2;
						y1 = y;
						y2 = y+2;
					} 

					//fill in traits section of rule_key
					for (int ax=x1; ax<x2; ax++) {
						for (int ay=y1; ay<y2; ay++) {
							if ((*world)[ax][ay].type == "eater" && (ay!=y && ax!=x)) {
								eater_count++;
							}
						}
					}

					etrs = bitset<4>(eater_count);

					// Generate rules index from facing dir and eater count
					for (int i=0; i<APEX_TRAITS; i++) {
						if (i<2) {
							apex_rule_key[i] = (*world)[x][y].facing[i];
						} else {
							apex_rule_key[i] = etrs[i-2];
						}
					}
					
					for (int i=APEX_TRAITS; i<(APEX_TRAITS+STATE_SIZE); i++) { // copy current state into key
						apex_rule_key[i] = (*world)[x][y].state[i-APEX_TRAITS];
					}
					
					for (int i=0; i<APEX_GENE_LENGTH; i++) {
						apex_rule[i] = apex_pop[current_pop_index].bits[APEX_GENE_LENGTH*(apex_rule_key.to_ulong()) + i];
					}

					// Get action and change state from rule
					for(int i=0; i<APEX_GENE_LENGTH; i++) {
						if (i<2) {
							action[i] = apex_rule[i];
						} else {
							(*world)[x][y].state[i-2] = apex_rule[i];
						}
					}

					m = action.to_ulong();
					//00 = choose a random grid square. repeat x2
						//if theres a plant, do nothing. 
						//if theres nothing, plant a plant and lose fitness. 
						//if theres an eater, take from its fitness (fitness can't go below 0.5)
	
					//01 = eat the first plant you find in your squares, gain a fitness. repeat
					//10 = rotate +1,  
					//11 = paralyze a nearby eater for 2 days (if theyre at least minimally fit)
					if (m==0) {
						for (int p=0; p<3; p++) {
							do {
								px = RANDOM_NUM_RANGE(x2-x1) + x1 + 1;		//random num between x1, x2 inclusive
								py = RANDOM_NUM_RANGE(y2-y1) + y1 + 1;
							} while (py==y && px==x);

							string face_type = (*world)[px][py].type;

							if (face_type == "eater") {
								if ( eater_pop[(*world)[px][py].pop_index].fitness >= 1.5f ) {
									eater_pop[(*world)[px][py].pop_index].fitness -= 1.0f;
									apex_pop[current_pop_index].fitness += 2.0f;
								}
								//warn eater that they were attacked
								(*world)[px][py].last_alert = time_step;

							} else if (face_type == "none") {
								if ( apex_pop[current_pop_index].fitness >= 1.5) {
									(*world)[px][py] = Entity("plant", false);
									(*world)[px][py].spec = 10;
									apex_pop[current_pop_index].fitness -= 1.0f;
								}
							} else if (face_type != "plant" && face_type == "apex") { //should never reach this
								std::cout << "Something is wrong in apex behavior :) face type: " << face_type << std::endl;
							}
						}

					} else if (m==1 && gen != 1) {
						for (int p=0; p<2; p++) {
							do {
								px = RANDOM_NUM_RANGE(x2-x1) + x1 + 1;		//random num between x1, x2 inclusive
								py = RANDOM_NUM_RANGE(y2-y1) + y1 + 1;
							} while (py==y && px==x);
							string face_type = (*world)[px][py].type;

							if (face_type == "plant") {
								(*world)[px][py] = Entity();
								apex_pop[current_pop_index].fitness += 0.75f;
							}
						}

					} else if (m==2) {
						(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()+1);
					} else if (m==3) {
						do {
							px = RANDOM_NUM_RANGE(x2-x1) + x1 + 1;		//random num between x1, x2 inclusive
							py = RANDOM_NUM_RANGE(y2-y1) + y1 + 1;
						} while (py==y && px==x);
						string face_type = (*world)[px][py].type;

						if (face_type == "eater" && eater_pop[(*world)[px][py].pop_index].fitness >= 1.5f) {
							//eater_pop[(*world)[px][py].pop_index]
							(*world)[px][py].last_action += 2;	//paralyze the eater for 2 days
							//warn eater that they were attacked
							(*world)[px][py].last_alert = time_step;
						}
					}

				} else if( (*world)[x][y].type == "eater" && (*world)[x][y].last_action < time_step) {
					
					current_pop_index = (*world)[x][y].pop_index;
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
						//Issue: what if it's an apex?
	
					}

					if (xb < 1 || yb < 1 || xb>WORLD_SIZE-2 || yb>WORLD_SIZE-2) {
						behind_type = "wall";
					} else {
						behind_type = (*world)[xb][yb].type;
					}

					for (int i=0; i<(EATER_TRAITS+STATE_SIZE); i++) {
						if (i==0) { //lowest bit = did an apex warn me recently?
							if ((*world)[x][y].last_alert == time_step || 
								(*world)[x][y].last_alert == time_step-1 || 
								(*world)[x][y].last_alert == time_step-2) {
								eater_rule_key[i] = 1;
							} else {
								eater_rule_key[i] = 0;
							}
						} else if (i>0 && i<EATER_TRAITS) { //next 4 bits = what can i see + where am i looking
							eater_rule_key[i] = view[i-1];
						} else { // last STATE_SIZE is the state
							eater_rule_key[i] = (*world)[x][y].state[i-EATER_TRAITS];
						}
					}

					for (int i=0; i<EATER_GENE_LENGTH; i++) {
						eater_rule[i] = eater_pop[current_pop_index].bits[EATER_GENE_LENGTH*(eater_rule_key.to_ulong()) + i];
					}

					for(int i=0; i<EATER_GENE_LENGTH; i++) {
						if (i<2) {
							action[i] = eater_rule[i];
						} else {
							(*world)[x][y].state[i-2] = eater_rule[i];   //update the eater's state regardless
						}
					}

					m = action.to_ulong();
					//00 = forward, 01 = backward, 10 = +1 to facing, 11 = -1 to facing

					if (m==2) {
						(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()+1);
					} else if (m==3) {
						(*world)[x][y].facing = bitset<2>((*world)[x][y].facing.to_ulong()-1);
					} else if ((view[2] || view[3]) && facing_type != "eater" && facing_type != "apex") { // (view[2] || view[3]) means they aren't looking at a wall
						
						//move the eater
						if (m == 0) {
							int face_spec = (*world)[xf][yf].spec;
							(*world)[xf][yf] = (*world)[x][y]; 	
							(*world)[x][y] = Entity();	

							if(facing_type == "plant") {
								eater_pop[current_pop_index].fitness += 1.0;

								if (face_spec != 10) RespawnPlantNearby(xf,yf); //only do random respawn on the core pool of plants
							}

						} else if (behind_type != "wall" && behind_type != "eater" && behind_type != "apex") {	//unfortunately I have to manually check the grid behind them
							int face_spec = (*world)[xf][yf].spec;
							(*world)[xb][yb] = (*world)[x][y]; 
							(*world)[x][y] = Entity();	

							if(behind_type == "plant") {
								eater_pop[current_pop_index].fitness += 1.0;	//only do random respawn on the core pool of plants
								if (face_spec != 10) RespawnPlantNearby(xb,yb);
							}
						}
			
					}	
				}
			}
		}

	}

	time_step++;	//increment time step
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

	(*world)[x2][y2] = Entity("plant", false);
}