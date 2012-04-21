
#include "ant_colony.h"

AntColony::AntColony(std::string name, std::string color, float x, float y) {
    this->name = name;
    this->color = Color::FromHexString(color);
    this->SetPosition(x, y);
}


