#ifndef GEN_ALG_GAME_
#define GEN_ALG_GAME_

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include "chromosome.h"
#include "entity.h"

using std::vector;
using std::ofstream;
class GenAlgGame {

public:
    GenAlgGame();
    GenAlgGame(int eater_pop_size, int plant_pop_size, int apex_pop_size);

    int time_step,eater_pop_size,plant_pop_size,apex_pop_size,gen;
    float crossover_rate, mutation_rate, deletion_rate;
    //ofstream csv_file;

    vector<float> max_eater_fitness;
    vector<float> max_apex_fitness;
    vector<float> avg_eater_fitness;
    vector<float> avg_apex_fitness;
    vector<float> plant_count;
    vector<float> gen_times;

    array<array<Entity, WORLD_SIZE>, WORLD_SIZE> *world;

    vector< Chromosome<EATER_GENE_LENGTH, EATER_GENE_COUNT> > eater_pop;
    vector< Chromosome<APEX_GENE_LENGTH, APEX_GENE_COUNT> > apex_pop;

    void SetGeneticParams(float crossover_rate, float mutation_rate, float deletion_rate);

    void ProgressTime();

    void Generation(int days);

    void RespawnPlantNearby(int x1, int y1);

    void GetRandomCoord(Entity ent);
    void GetRandomCoord(Entity ent, int r);

};

#endif //GEN_ALG_GAME_