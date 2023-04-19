#include "gen_alg.h"
#include "gen_alg_game.h"
#include <vector>
#include "time.h"
#include <chrono>
using std::vector;

#define NUM_TESTS 20

vector<string> WorldToStrings(array<array<Entity, WORLD_SIZE>, WORLD_SIZE> w);

int main(int argc, char *argv[]) {			//	--------------------------------etrs-------------------------------gens/days----------------------------etrs--------------------------
	// 										    pl1		pl2		pl3		mut	 c	---								  *++++++*								 ---	
	array<int, NUM_TESTS> eater_pop_sizes     = {10,    10,    10,    10,    10,    50,    50,    50,    50,    50,       10,    10,    10,    10,    10,    50,    50,    50,    50,    50};
	array<int, NUM_TESTS> plant_pop_sizes     = {0,     50,    250,   150,   150,   0,     50,    250,   150,   150,      0,     50,    250,   150,   150,   0,     50,    250,   150,   150};
	array<int, NUM_TESTS> apex_pop_sizes      = {16,    16,    16,    16,    16,    16,    16,    16,    16,    16,       16,    16,    16,    16,    16,    16,    16,    16,    16,    16};
	array<int, NUM_TESTS> gens 				  = {2000,  2000,  2000,  2000,  2000,  2000,  2000,  2000,  2000,  2000,     1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000};
	array<int, NUM_TESTS> days	 			  = {100,   100,   100,   100,   100,   100,   100,   100,   100,   100,      200,   200,   200,   200,   200,   200,   200,   200,   200,   200};
	array<float, NUM_TESTS> crossover_rates   = {0.75,  0.75,  0.75,  0.75,  0.25,  0.75,  0.75,  0.75,  0.75,  0.25,     0.75,  0.75,  0.75,  0.75,  0.25,  0.75,  0.75,  0.75,  0.75,  0.25};
	array<float, NUM_TESTS> mutation_rates    = {0.001, 0.001, 0.001, 0.01,  0.001, 0.001, 0.001, 0.001, 0.01,  0.001,    0.001, 0.001, 0.001, 0.01,  0.001, 0.001, 0.001, 0.001, 0.01,  0.001};
	//gate string type check behind bool flag isCreature check in genalg

	for (int i=0; i<NUM_TESTS; i++) {

		if (argc < 2) { 
			//time based seed (randomish)
			srand((int)time(NULL));
		} else {
			//set seed based on argv[1]
			srand(std::atoi(argv[1]));
		}

		ofstream f;
		string fn = "test" + std::to_string(i) + ".csv";

		f.open(fn, std::ios::out);
		f << "generation,max_fitness,avg_fitness,max_fitness_apex,avg_fitness_apex,total_plants\n";
		auto bef_game = std::chrono::high_resolution_clock::now();

		GenAlgGame *game1 = new GenAlgGame(eater_pop_sizes[i], plant_pop_sizes[i], apex_pop_sizes[i]);
		game1->SetGeneticParams(crossover_rates[i], mutation_rates[i], 0.000);
		for(int g=0; g<gens[i]; g++) {
			game1->Generation(days[i]);
		}
		auto after_game = std::chrono::high_resolution_clock::now();
		std::cout << "Game " << i << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(after_game-bef_game).count() << "ms\n";

		for (int g=0; g<gens[i]; g++) {
			f << g << "," << game1->max_eater_fitness[g] << "," \
						<< game1->avg_eater_fitness[g] << "," \
						<< game1->max_apex_fitness[g]  << "," \
						<< game1->avg_apex_fitness[g]  << ","  \
						<< game1->plant_count[g] << "\n";
		}
		f.close();

		delete game1;
	}


	//turn this to file stream
	/*#if !MAKE_CSV
    for (int d=0; d<DAYS_PER_GENERATION; d++) {
		game1->ProgressTime();
		
		WorldToStrings(*(game1->world));
		
	}
	#endif*/

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