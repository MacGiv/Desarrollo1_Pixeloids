#include "game_loop.h"
#include "player.h"
#include "game_data.h"
#include "bullet.h"
#include "raymath.h"
#include "asteroid.h"
using namespace std;

namespace pixeloids_luchelli
{
enum class BORDERS {LEFT, RIGHT, TOP, BOTTOM};

const int maxBullets = 100;
const int totalAsteroids = maxLargeAsteroids + maxMediumAsteroids + maxSmallAsteroids;
const float asteroidStartSpeed = 150.0f;
Player player;
Bullet bullets[maxBullets];
Asteroid asteroids[totalAsteroids];
int activeAsteroidCount = 0;

static void initializeGame();
static void update();
static void draw();
static void close();
static void initializeBulletArray(Bullet bullets[], int arraySize);
static void initializeAsteroids(Asteroid asteroidsArray[]);
static void updateAsteroids(Asteroid asteroidsArray[]);
static void drawAsteroids(Asteroid asteroidsArray[]);


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

    initializeAsteroids(asteroids);
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

    // ASteroids update
    updateAsteroids(asteroids);

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
    
    drawAsteroids(asteroids);
    // END OF GAMEPLAY DRAW

    EndDrawing();
}

void close()
{
    CloseWindow();
}


void initializeBulletArray(Bullet bulletsArray[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        bulletsArray[i].active = false;
        bulletsArray[i].radius = bulletDefaultRadius;
        bulletsArray[i].speed = bulletDefaultSpeed;
    }
}

void initializeAsteroids(Asteroid asteroidsArray[])
{
    for (int i = 0; i < maxLargeAsteroids; i++)
    {
        // Random border (0 = left, 1 = right, 2 = top, 3 = bottom)
        int border = GetRandomValue(static_cast<int>(BORDERS::LEFT), static_cast<int>(BORDERS::BOTTOM));
        Vector2 position = { 0,0 };
        Vector2 velocity = { 0,0 };

        switch (static_cast<BORDERS>(border))
        {
        case BORDERS::LEFT: // Left border
            position = { 0, static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
            velocity = { asteroidStartSpeed, static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))) };
            break;
        case BORDERS::RIGHT: // Right border
            position = { static_cast<float>(GetScreenWidth()), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
            velocity = { -asteroidStartSpeed, static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))) };
            break;
        case BORDERS::TOP: // Top border
            position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), 0 };
            velocity = { static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))), asteroidStartSpeed };
            break;
        case BORDERS::BOTTOM: // Bottom border
            position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), static_cast<float>(GetScreenHeight()) };
            velocity = { static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))), -asteroidStartSpeed };
            break;
        }

        InitializeAsteroid(asteroidsArray[i], position, velocity, AsteroidSize::LARGE);
        activeAsteroidCount++;
    }

}

void updateAsteroids(Asteroid asteroidsArray[])
{
    for (int i = 0; i < totalAsteroids; i++) 
    {
        UpdateAsteroid(asteroidsArray[i]);
    }
}

void drawAsteroids(Asteroid asteroidsArray[])
{
    for (int i = 0; i < totalAsteroids; i++) 
    {
        DrawAsteroid(asteroidsArray[i]);
    }
}

}