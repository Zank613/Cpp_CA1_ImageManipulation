//
// Created by floodd on 27/01/2026.
//

#ifndef IMAGEPROJECT_BUTTON_H
#define IMAGEPROJECT_BUTTON_H
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Image.h"

using namespace sf;
using namespace std;

typedef std::function<void (void)> Handler;
class Button :public Drawable
{
    Font font;
    Vector2f size;
    Vector2i position;
    string buttonText;
    Handler handler;
public :
    Button( Font &font):font(font){};
    Button( Font &font, Vector2i position,Vector2f size,  string txt, Handler handler):font(font), position(position), size(size), buttonText(txt), handler(handler){};
    void draw(RenderTarget &target, RenderStates states)const;
    void click();
    bool contains(Vector2i pos) const;

};


#endif //IMAGEPROJECT_BUTTON_H