#ifndef GEN_ALG_GAME_
#define GEN_ALG_GAME_

#include <array>
#include "chromosome.h"
#include "entity.h"

class GenAlgGame {

public:
    GenAlgGame();
    GenAlgGame(int eater_pop_size, int plant_pop_size);

    int time_step,eater_pop_size,plant_pop_size,gen;
    array<array<Entity, WORLD_SIZE>, WORLD_SIZE> *world;


    float ProgressTime();
    void Generation();
    void RespawnPlantNearby(int x1, int y1);

    void GetRandomCoord(Entity ent);

};

#endif //GEN_ALG_GAME_