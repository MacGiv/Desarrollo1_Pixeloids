#include "game_loop.h"
#include "player.h"
#include "game_data.h"

namespace pixeloids_luchelli
{

Player player;

void runGame() 
{
    initializeGame();

    while (!WindowShouldClose()) 
    {
        update();

        draw();
    }

    close();
}

void initializeGame()
{
    InitWindow(screenWidth, screenHeight, "Asteroids - Player Movement");

    InitializePlayer(player);
}

void update() 
{
    UpdatePlayer(player);
}

void draw() 
{
    BeginDrawing();

    // Gameplay
    ClearBackground(BLACK);

    DrawPlayer(player);
    // Fin de gameplay

    EndDrawing();
}

void close()
{
    CloseWindow();
}

}