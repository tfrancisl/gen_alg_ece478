#include "entity.h"
using std::string;

Entity::Entity() {
    this->type = "none";
    this->is_creature = false;
}

Entity::Entity(string t, bool cre) {
    this->type = t;
    this->is_creature = cre;
    this->last_action = -1;
    this->last_alert = -100;
    this->spec = 0;
}
