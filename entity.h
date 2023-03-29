#ifndef ENTITY_H_
#define ENTITY_H_

#include "chromosome.h"

class Entity {

public:
    Entity();
    Entity(string t);
    Entity(string t, Chromosome g);
    friend bool operator<(const Entity &l, const Entity &r);
    Chromosome genes;
    //int x,y;
    string type;
    string state;
};


bool operator<(const Entity &l, const Entity &r);
#endif