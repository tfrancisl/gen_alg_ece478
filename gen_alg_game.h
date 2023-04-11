#ifndef GEN_ALG_GAME_
#define GEN_ALG_GAME_

#include <array>
#include <vector>
#include "chromosome.h"
#include "entity.h"

using std::vector;
class GenAlgGame {

public:
    GenAlgGame();
    GenAlgGame(int eater_pop_size, int plant_pop_size);

    int time_step,eater_pop_size,plant_pop_size,gen;
    array<array<Entity, WORLD_SIZE>, WORLD_SIZE> *world;

    vector< Chromosome<EATER_GENE_LENGTH, EATER_GENE_COUNT> > eater_pop;
    vector< Chromosome<APEX_GENE_LENGTH, APEX_GENE_COUNT> > apex_pop;

    void ProgressTime();      //game logic more similar to the original plant and eater 

    void Generation();
    //void ApexGeneration();
    void RespawnPlantNearby(int x1, int y1);

    void GetRandomCoord(Entity ent);
    void GetRandomCoord(Entity ent, int r);

};

#endif //GEN_ALG_GAME_