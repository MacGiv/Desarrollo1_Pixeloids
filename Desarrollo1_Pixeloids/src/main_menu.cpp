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
    static Button button1, button2, button3, button4, button5, button6;
    static Texture2D menuBackground;

    static void initializeCreditsButtons();
    
    void initializeMenuButtons()
    {
        titleFont = LoadFont("res/ethnocentric_rg_it.otf");
        menuBackground = LoadTexture("res/menu_background.png");
        menuOthersBackground = LoadTexture("res/menu_others_background_1.png");

        const Vector2 buttonSize = { 150, 50 };
        const float buttonSpacing = 50.0f;    
        const float totalHeight = 4 * buttonSize.y + 3 * buttonSpacing;
        const float startY = (GetScreenHeight() - totalHeight) / 2.0f;

        const float posX = (GetScreenWidth() - buttonSize.x) / 2.0f;

        playButton = createButton({ posX, startY }, buttonSize, "Play", PINK_MINE);
        creditsButton = createButton({ posX, startY + buttonSize.y + buttonSpacing }, buttonSize, "Credits", PINK_MINE);
        howToPlayButton = createButton({ posX, startY + 2 * (buttonSize.y + buttonSpacing) }, buttonSize, "How To Play", PINK_MINE);
        quitButton = createButton({ posX, startY + 3 * (buttonSize.y + buttonSpacing) }, buttonSize, "Exit", PINK_MINE);

        initializeCreditsButtons();
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
        DrawText("PIXELOIDS", static_cast<int>(titlePosition.x), static_cast<int>(titlePosition.y), 40, CYAN);

        Rectangle source = { 0.0f, 0.0f, static_cast<float>(menuBackground.width), static_cast<float>(menuBackground.height) };
        Rectangle dest = { 0.0f, 0.0f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenWidth()) };
        Vector2 origin = { 0.0f, 0.0f };
        DrawTexturePro(menuBackground, source, dest, origin, 0.0f, GRAY);
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
        DrawText(title, titleX, titleY, titleSize, CYAN);

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


    void initializeCreditsButtons()
    {
        const float buttonWidth = 175.0f;
        const float buttonHeight = 50.0f;
        const float buttonSpacing = 45.0f;
        const float posX = (screenWidth / 5) * 4 - buttonWidth / 2;
        const float startY = 200.0f;

        button1 = createButton({ posX, startY }, { buttonWidth, buttonHeight }, "Itch.io");
        button2 = createButton({ posX, startY + buttonHeight + buttonSpacing }, { buttonWidth, buttonHeight }, "GitHub");
        button3 = createButton({ posX, startY + 2 * (buttonHeight + buttonSpacing) }, { buttonWidth, buttonHeight }, "MS Designer");
        button4 = createButton({ posX, startY + 3 * (buttonHeight + buttonSpacing) }, { buttonWidth, buttonHeight }, "ElevenLabs");
        button5 = createButton({ posX, startY + 4 * (buttonHeight + buttonSpacing) }, { buttonWidth, buttonHeight }, "SORA");
        button6 = createButton({ posX, startY + 5 * (buttonHeight + buttonSpacing) }, { buttonWidth, buttonHeight }, "SUNO");
    }

    void updateCredits()
    {
        if (isButtonClicked(backToMenuButton))
        {
            PlaySound(buttonSfx);
            gameState.nextState = GameStates::MENU;
            PlayMusicStream(mainMenuMusic);
        }

        if (isButtonClicked(button1))
        {
            PlaySound(buttonSfx);
            OpenURL("https://macgivdev.itch.io/");
        }
        if (isButtonClicked(button2))
        {
            PlaySound(buttonSfx);
            OpenURL("https://github.com/MacGiv/Desarrollo1_Pixeloids");
        }
        if (isButtonClicked(button3))
        {
            PlaySound(buttonSfx);
            OpenURL("https://designer.microsoft.com/");
        }
        if (isButtonClicked(button4))
        {
            PlaySound(buttonSfx);
            OpenURL("https://elevenlabs.io/sound-effects");
        }
        if (isButtonClicked(button5))
        {
            PlaySound(buttonSfx);
            OpenURL("https://www.instagram.com/soradecodesign/profilecard/?igsh=MWkzYWk4bDdsMGx3");
        }
        if (isButtonClicked(button6))
        {
            PlaySound(buttonSfx);
            OpenURL("https://suno.ai/");
        }
    }

    void drawCredits()
    {
        ClearBackground(BLACK);

        int titleSize = 60;
        int normalTextSize = 30;
        int spacing = normalTextSize + 60;

        const char* title = "Credits";
        int titleX = screenWidth / 2 - MeasureText(title, titleSize) / 2;
        int titleY = screenHeight / 8;
        DrawText(title, titleX, titleY, titleSize, CYAN);

        const char* line1 = "Developed by Tomas F. Luchelli";
        int line1X = screenWidth / 3 - MeasureText(line1, normalTextSize) / 2;
        int line1Y = titleY + titleSize + spacing;
        DrawText(line1, line1X, line1Y, normalTextSize, WHITE);

        const char* line2 = "Sprites and bg by Microsoft Designer AI";
        int line2X = screenWidth / 3 - MeasureText(line2, normalTextSize) / 2;
        int line2Y = line1Y + static_cast<int>(spacing * 1.75f);
        DrawText(line2, line2X, line2Y, normalTextSize, WHITE);

        const char* line3 = "Sfx made with ElevenLabs AI";
        int line3X = screenWidth / 3 - MeasureText(line3, normalTextSize) / 2;
        int line3Y = line2Y + spacing;
        DrawText(line3, line3X, line3Y, normalTextSize, WHITE);

        const char* line4 = "Game HUD art made by Juliana Menna";
        int line4X = screenWidth / 3 - MeasureText(line4, normalTextSize) / 2;
        int line4Y = line3Y + spacing;
        DrawText(line4, line4X, line4Y, normalTextSize, WHITE);

        const char* line5 = "Music made with SUNO";
        int line5X = screenWidth / 3 - MeasureText(line5, normalTextSize) / 2;
        int line5Y = line4Y + spacing;
        DrawText(line5, line5X, line5Y, normalTextSize, WHITE);

        drawButton(button1);
        drawButton(button2);
        drawButton(button3);
        drawButton(button4);
        drawButton(button5);
        drawButton(button6);
        drawButton(backToMenuButton);
    }

    void unloadMenuRes()
    {
        UnloadTexture(menuBackground);
    }

} // namespace pixeloids_luchelli