#include "entity.h"
using std::string;

Entity::Entity() {
    this->type = "none";
}

Entity::Entity(string t) {
    this->type = t;
}

Entity::Entity(string t, Chromosome g) {
    this->type = t;
    this->genes = g;
    this->last_action = -1;
    this->spec = 0;
}
