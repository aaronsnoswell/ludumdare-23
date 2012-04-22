
#include "food.h"

Food::Food(float x, float y) {
    FOOD_COLOR = "#f6ff29";
    
    this->consumed = false;
    
    this->SetPosition(x, y);
    this->SetColor(Color::FromHexString(FOOD_COLOR));
    this->SetRotation(rand_range(0, 360));
    this->SetSprite("Resources/Images/food.png");
    this->SetSize(1.3f);
    
    this->being_carried = false;
    this->value = VALUE_STANDARD;
}

/**
 * Removes this peice of food from the game
 */
void Food::Remove() {
    int pos = 0;
    for(std::vector<Food *>::iterator i=theGame.foodbits.begin(); i!=theGame.foodbits.end(); i++) {
        if((*i) == this) {
            break;
        }
        pos++;
    }
    
    theWorld.Remove(this);
    theGame.foodbits.erase(theGame.foodbits.begin() + pos);
}