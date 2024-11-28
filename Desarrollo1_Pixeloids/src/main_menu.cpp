#include "main_menu.h"

#include "button.h"
#include "raylib.h"
#include "game_data.h"
#include "state_machine.h"

#include "raymath.h"

namespace pixeloids_luchelli 
{

    extern GameStateMachine gameState;
    extern Sound buttonSfx;
    extern Music mainMenuMusic;
    extern Music gameplayMusic;
    extern Button backToMenuButton;

    static Button playButton, creditsButton, howToPlayButton, quitButton;

    void initializeMenuButtons() 
    {
        playButton = createButton({ 100, 100 }, { 150, 50 }, "Play");
        creditsButton = createButton({ 100, 300 }, { 150, 50 }, "Credits");
        howToPlayButton = createButton({ 100, 500 }, { 150, 50 }, "How To Play");
        quitButton = createButton({ 100, 700 }, { 150, 50 }, "Exit");
    }

    void updateMenu() 
    {
        UpdateMusicStream(mainMenuMusic);

        if (isButtonClicked(playButton)) 
        {
            PlaySound(buttonSfx);
            StopMusicStream(mainMenuMusic);
            gameState.nextState = GameStates::PLAYING;
            PlayMusicStream(gameplayMusic);
        }
        if (isButtonClicked(quitButton)) 
        {
            PlaySound(buttonSfx);
            StopMusicStream(mainMenuMusic);
            gameState.nextState = GameStates::EXIT;
        }
        if (isButtonClicked(creditsButton)) 
        {
            PlaySound(buttonSfx);
            StopMusicStream(mainMenuMusic);
            gameState.nextState = GameStates::CREDITS;
        }
        if (isButtonClicked(howToPlayButton)) 
        {
            PlaySound(buttonSfx);
            StopMusicStream(mainMenuMusic);
            gameState.nextState = GameStates::HOW_TO_PLAY;
        }
    }

    void drawMenu() 
    {
        Vector2 titlePosition = { (static_cast<float>(GetScreenWidth()) - MeasureText("PIXELOIDS", 40)) / 2.0f, 50.0f };
        DrawText("PIXELOIDS", static_cast<int>(titlePosition.x), static_cast<int>(titlePosition.y), 40, ORANGE);

        drawButton(playButton);
        drawButton(creditsButton);
        drawButton(howToPlayButton);
        drawButton(quitButton);
    }

    void updateHowToPlay() 
    {
        if (isButtonClicked(backToMenuButton))
        {
            PlaySound(buttonSfx);
            PlayMusicStream(mainMenuMusic);
            gameState.nextState = GameStates::MENU;
        }
    }

    void drawHowToPlay() 
    {
        ClearBackground(BLACK);

        const char* title = "How to Play";
        int titleSize = 60;
        int titleX = screenWidth / 2 - MeasureText(title, titleSize) / 2;
        int titleY = (screenHeight / 8);
        DrawText(title, titleX, titleY, titleSize, ORANGE);

        int textSize = 30;
        int spacing = textSize + 25;

        const char* line1 = "Use the mouse to control the ship.";
        int line1X = screenWidth / 2 - MeasureText(line1, textSize) / 2;
        int line1Y = titleY + titleSize + spacing;
        DrawText(line1, line1X, line1Y, textSize, WHITE);

        const char* line2 = "Left click: Shoot bullets.";
        int line2X = screenWidth / 2 - MeasureText(line2, textSize) / 2;
        int line2Y = line1Y + spacing;
        DrawText(line2, line2X, line2Y, textSize, WHITE);

        const char* line3 = "Right click: Accelerate towards the cursor.";
        int line3X = screenWidth / 2 - MeasureText(line3, textSize) / 2;
        int line3Y = line2Y + spacing;
        DrawText(line3, line3X, line3Y, textSize, WHITE);

        const char* line4 = "Avoid asteroids and destroy them to gain points.";
        int line4X = screenWidth / 2 - MeasureText(line4, textSize) / 2;
        int line4Y = line3Y + spacing;
        DrawText(line4, line4X, line4Y, textSize, WHITE);

        const char* line5 = "Game ends when you lose all lives.";
        int line5X = screenWidth / 2 - MeasureText(line5, textSize) / 2;
        int line5Y = line4Y + spacing;
        DrawText(line5, line5X, line5Y, textSize, WHITE);

        drawButton(backToMenuButton);
    }

    void updateCredits() 
    {
        if (isButtonClicked(backToMenuButton))
        {
            PlaySound(buttonSfx);
            gameState.nextState = GameStates::MENU;
            PlayMusicStream(mainMenuMusic);
        }
    }

    void drawCredits() 
    {
        ClearBackground(BLACK);

        int titleSize = 60;
        int normalTextSize = 30;
        int spacing = normalTextSize + 25;

        const char* title = "Credits";
        int titleX = screenWidth / 2 - MeasureText(title, titleSize) / 2;
        int titleY = screenHeight / 8;
        DrawText(title, titleX, titleY, titleSize, ORANGE);

        const char* credits = "Made by Tomas Francisco Luchelli";
        int creditsX = screenWidth / 2 - MeasureText(credits, normalTextSize) / 2;
        int creditsY = titleY + titleSize + spacing * 4;
        DrawText(credits, creditsX, creditsY, normalTextSize, WHITE);

        drawButton(backToMenuButton);
    }

} // namespace pixeloids_luchelli