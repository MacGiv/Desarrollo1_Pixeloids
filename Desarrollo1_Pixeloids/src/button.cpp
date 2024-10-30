#include "button.h"
#include "button.h"

namespace pixeloids_luchelli 
{

    Button createButton(Vector2 position, Vector2 size, const char* text) 
    {
        Button button;
        button.rect = { position.x, position.y, size.x, size.y };
        button.text = text;
        button.color = LIGHTGRAY;
        return button;
    }

    void drawButton(Button& button) 
    {
        DrawRectangleRec(button.rect, button.color);
        DrawText(button.text, static_cast<int>(button.rect.x) + 10, static_cast<int>(button.rect.y) + 10, 20, BLACK);
    }

    bool isButtonClicked(Button& button) 
    {
        return CheckCollisionPointRec(GetMousePosition(), button.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

}