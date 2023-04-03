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

    //string view;   //4 bits, up to 4 plants in view (spots 1,3,5,7)
    bitset<STATE_SIZE> state;  //should look different for different genetic entities
                   //for eater: 2 bits total
};


#endif