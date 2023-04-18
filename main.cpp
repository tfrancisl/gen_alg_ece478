#include "gen_alg.h"
#include "gen_alg_game.h"
#include <vector>
#include "time.h"
using std::vector;


vector<string> WorldToStrings(array<array<Entity, WORLD_SIZE>, WORLD_SIZE> w);

int main(int argc, char *argv[]) {
	if (argc < 2) { 
		//time based seed (randomish)
		srand((int)time(NULL));
	} else if (argc >= 2) {
		//set seed based on argv[1]
		srand(std::atoi(argv[1]));
	}

    GenAlgGame *game1 = new GenAlgGame(EATER_POP_SIZE, PLANT_POP_SIZE);


    #if MAKE_CSV
	std::cout << "generation,max_fitness,avg_fitness,max_fitness_apex,avg_fitness_apex,total_plants" << std::endl;
	#endif
    
    for(int g=0; g<GENERATIONS; g++) {
        game1->Generation();
    }

	#if !MAKE_CSV
    for (int d=0; d<DAYS_PER_GENERATION; d++) {
		game1->ProgressTime();
		
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
			} else if(w[x][y].type == "apex") {
				ent_sym = "A";
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