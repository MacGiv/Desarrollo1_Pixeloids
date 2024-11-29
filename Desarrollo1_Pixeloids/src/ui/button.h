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

Button createButton(Vector2 position, Vector2 size, const char* text, Color color);
void drawButton(Button& button);
bool isButtonClicked(Button& button);

} // namespace pixeloids_luchelli

#endif // BUTTON_H

