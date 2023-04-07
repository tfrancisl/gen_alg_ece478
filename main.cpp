#include "gen_alg.h"
#include "gen_alg_game.h"
#include <vector>
#include "time.h"
using std::vector;


vector<string> WorldToStrings(array<array<Entity, WORLD_SIZE>, WORLD_SIZE> w);

int main(void) {

	srand((int)time(NULL));

    GenAlgGame *game1 = new GenAlgGame(EATER_POP_SIZE, PLANT_POP_SIZE);
    int max_fitness;


    #if MAKE_CSV
	std::cout << "generation,max_fitness,avg_fitness,avg_fitness_10" << std::endl;
	#endif
    
    for(int g=0; g<GENERATIONS; g++) {
        game1->Generation();
    }

	#if !MAKE_CSV
    for (int d=0; d<DAYS_PER_GENERATION; d++) {
		max_fitness = game1->ProgressTime2();
		
		WorldToStrings(*(game1->world));
		
	}
	#endif

}


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