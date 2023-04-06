#ifndef ENTITY_H_
#define ENTITY_H_

#include "chromosome.h"
using std::string;

class Entity {

public:
    Entity();
    Entity(string t);
    Entity(string t, Chromosome g);
    Chromosome genes;

    int last_action;  //what time step was their last action taken on?
    string type;
    int spec;

    bitset<STATE_SIZE> state;
    bitset<2> facing;   //encodes facing dir 00 = up, 01 = right, 10 = down, 11 = left
};


#endif