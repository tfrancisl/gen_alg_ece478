#include "entity.h"
//#include "chromosome.h"
using std::string;

Entity::Entity() {
    //this->type = "eater";
    this->type = "none";
}

Entity::Entity(string t) {
    this->type = t;
}

Entity::Entity(string t, Chromosome g) {
    this->type = t;
    this->genes = g;
}

//bool operator<(const Entity &l, const Entity &r)
//{
//    return l.genes.fitness < r.genes.fitness;
//}
