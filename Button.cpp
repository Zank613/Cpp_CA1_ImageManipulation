//
// Created by floodd on 27/01/2026.
//

#include "Button.h"

void Button::draw(sf::RenderTarget &target, sf::RenderStates states)const {
    sf::RectangleShape bg;
    bg.setSize({(float)this->size.x, (float)this->size.y});
    bg.setPosition({(float)this->position.x, (float)this->position.y});
    bg.setFillColor(sf::Color(220,220,220));
    string text = this->buttonText;
    bool textFits = false;
    Text txt(font) ;
    int fontSize = 20;
    txt.setCharacterSize(fontSize);
    txt.setFillColor(sf::Color::Black);
    txt.setString(text);

    while (!textFits) {
        if (txt.getGlobalBounds().size.x >= this->size.x-20) {
            txt.setCharacterSize(fontSize--);
            txt.setString(text);
        }
        else {
            textFits = true;
        }
    }
    float x = this->position.x + ((this->size.x-txt.getGlobalBounds().size.x)/2.0f);
    float y = this->position.y + ((this->size.y-txt.getGlobalBounds().size.y)/2.0f);
    txt.setPosition({x,y});
    txt.setOrigin(txt.getLocalBounds().position);
    target.draw(bg);
    target.draw(txt);

}


void Button::click() {
    handler();
}

bool Button::contains(const Vector2i pos) const{
    return pos.x > this->position.x && pos.x < this->position.x+this->size.x && pos.y > this->position.y && pos.y < this->position.y+this->size.y;
}