#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"


namespace pixeloids_luchelli 
{

struct Button
{
    Rectangle rect;
    const char* text;
    Color color;
};

Button CreateButton(Vector2 position, Vector2 size, const char* text);
void DrawButton(const Button& button);
bool IsButtonClicked(const Button& button);

}

#endif 

