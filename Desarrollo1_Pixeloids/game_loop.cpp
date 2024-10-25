#include "game_loop.h"
#include "player.h"
#include "game_data.h"
#include "bullet.h"
#include "raymath.h"

namespace pixeloids_luchelli
{

Player player;
const int maxBullets = 100;
Bullet bullets[maxBullets];


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
    InitWindow(screenWidth, screenHeight, "Asteroids");

    InitializePlayer(player);

    InitializeBulletArray(bullets, maxBullets);
}

void update() 
{

    // GAMEPLAY UPDATE
    UpdatePlayer(player);

    // Bullet Update
    for (int i = 0; i < maxBullets; i++) 
    {
        UpdateBullet(bullets[i]);
    }

    // Shoot update
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
        FireBullet(bullets, maxBullets, player.position, direction);
    }
    // END OF GAMEPLAY UPDATE
}

void draw() 
{
    BeginDrawing();

    // GAMEPLAY DRAW
    ClearBackground(BLACK);

    //Draw Bullets
    for (int i = 0; i < maxBullets; i++) 
    {
        DrawBullet(bullets[i]);
    }

    DrawPlayer(player);
    // END OF GAMEPLAY DRAW

    EndDrawing();
}

void close()
{
    CloseWindow();
}

}