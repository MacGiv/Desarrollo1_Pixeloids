#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include <conio.h>


namespace pixeloids_luchelli {

    struct Button
    {
        Rectangle rect;
        String text;
        Color color;
    };

    Button CreateButton(Vector2 position, Vector2 size, String text);
    void DrawButton(const Button& button);
    bool IsButtonClicked(const Button& button);

}

#endif 

