#include "button.h"

#include "game_data.h"

namespace pixeloids_luchelli 
{

    Button createButton(Vector2 position, Vector2 size, const char* text, Color color) 
    {
        Button button;
        button.rect = { position.x, position.y, size.x, size.y };
        button.text = text;
        button.color = color;
        return button;
    }

    void drawButton(Button& button) 
    {
        DrawRectangleRec(button.rect, button.color);

        DrawRectangleLines(static_cast<int>(button.rect.x), static_cast<int>(button.rect.y),
            static_cast<int>(button.rect.width), static_cast<int>(button.rect.height), CYAN);

        int textWidth = MeasureText(button.text, 20); 
        int textHeight = 20;

        int xPos = static_cast<int>(button.rect.x) + static_cast<int>((button.rect.width - textWidth) / 2);
        int yPos = static_cast<int>(button.rect.y) + static_cast<int>((button.rect.height - textHeight) / 2);

        DrawText(button.text, xPos, yPos, 20, BLACK);
    }

    bool isButtonClicked(Button& button) 
    {
        return CheckCollisionPointRec(GetMousePosition(), button.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

}