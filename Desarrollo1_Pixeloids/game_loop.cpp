#include "game_loop.h"
#include "player.h"
#include "game_data.h"
#include "bullet.h"
#include "raymath.h"

namespace pixeloids_luchelli
{

static void initializeGame();

static void update();

static void draw();

static void close();

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

    initializePlayer(player);

    initializeBulletArray(bullets, maxBullets);
}

void update() 
{

    // GAMEPLAY UPDATE
    updatePlayer(player);

    // Bullet Update
    for (int i = 0; i < maxBullets; i++) 
    {
        updateBullet(bullets[i]);
    }

    // Shoot update
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
        fireBullet(bullets, maxBullets, player.position, direction);
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
        drawBullet(bullets[i]);
    }

    drawPlayer(player);
    // END OF GAMEPLAY DRAW

    EndDrawing();
}

void close()
{
    CloseWindow();
}

}