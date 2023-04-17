#include "entity.h"
using std::string;

Entity::Entity() {
    this->type = "none";
}

Entity::Entity(string t) {
    this->type = t;
    this->last_action = -1;
    this->last_alert = -100;
    this->spec = 0;
}
