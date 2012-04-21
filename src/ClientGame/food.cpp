
#include "food.h"

#include "ant.h"

Food::Food(float x, float y) {
    FOOD_COLOR = "#f6ff29";
    
    this->SetPosition(x, y);
    this->SetColor(Color::FromHexString(FOOD_COLOR));
}

void Food::Update(float dt) {
    if(carriedBy != NULL) {
        // TODO ajs 21/04/12 Add offset for the ant's head
        this->SetPosition(carriedBy->GetPosition());
    }
}
