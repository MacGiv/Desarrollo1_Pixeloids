#include "game_loop.h"
#include "player.h"
#include "game_data.h"
#include "bullet.h"
#include "raymath.h"
#include "asteroid.h"
#include "button.h"

namespace pixeloids_luchelli
{

enum class Borders {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};
enum class GameStates {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    CREDITS,
    EXIT
};
struct GameStateMachine
{
    GameStates currentState;
    GameStates nextState;
};

const int playerMaxLives = 3;
const int maxBullets = 100;
const int totalAsteroids = maxLargeAsteroids + maxMediumAsteroids + maxSmallAsteroids;
const float asteroidStartSpeed = 150.0f;
Player player;
Bullet bullets[maxBullets];
Asteroid asteroids[totalAsteroids];
GameStateMachine gameState{};
int activeAsteroidCount = 0;
int smallAsteroidDestroyedCount = 0;
int playerCuurentLives = playerMaxLives;

static Button playButton, resumeButton, exitButton, backToMenuButton;

static void initializeGame();
static void update();
static void draw();
static void close();
static void initializeButtons();
static void initializeBulletArray(Bullet bullets[], int arraySize);
static void initializeAsteroids(Asteroid asteroidsArray[]);
static void updateMenu();
static void updateAsteroids(Asteroid asteroidsArray[]);
static void drawAsteroids(Asteroid asteroidsArray[]);
static void handleBulletAsteroidCollisions(Bullet bullets[], Asteroid asteroidsArray[], int& asteroidCount);
static void getRandomPosAndVelocity(Vector2& position, Vector2& velocity);



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
    gameState.currentState = GameStates::MENU;
    gameState.nextState = GameStates::MENU;
    playerCuurentLives = playerMaxLives;

    InitWindow(screenWidth, screenHeight, "Asteroids");

    initializePlayer(player);

    initializeBulletArray(bullets, maxBullets);

    initializeAsteroids(asteroids);

    initializeButtons();
}

void update() 
{
    switch (gameState.currentState)
    {
    case pixeloids_luchelli::GameStates::MENU:
        updateMenu();
        break;
    case pixeloids_luchelli::GameStates::PLAYING:
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

        // Asteroids update
        updateAsteroids(asteroids);
        handleBulletAsteroidCollisions(bullets, asteroids, activeAsteroidCount);

        break;
    case pixeloids_luchelli::GameStates::PAUSED:
        if (IsButtonClicked(resumeButton))
            gameState.nextState = GameStates::PLAYING;

        if (IsButtonClicked(backToMenuButton))
            gameState.nextState = GameStates::MENU;
        break;
    case pixeloids_luchelli::GameStates::GAME_OVER:
        if (IsButtonClicked(backToMenuButton))
            gameState.nextState = GameStates::MENU;

        if (IsButtonClicked(exitButton))
            gameState.nextState = GameStates::EXIT;
        break;
    case pixeloids_luchelli::GameStates::EXIT:

        break;
    default:
        break;
    }
    
    gameState.currentState = gameState.nextState;
}



void draw() 
{
    BeginDrawing();
    ClearBackground(BLACK);

    switch (gameState.currentState)
    {
    case pixeloids_luchelli::GameStates::MENU:
        DrawButton(playButton);
        DrawButton(exitButton);
        break;
    case pixeloids_luchelli::GameStates::PLAYING:
        for (int i = 0; i < maxBullets; i++)
        {
            drawBullet(bullets[i]);
        }

        drawPlayer(player);

        drawAsteroids(asteroids);
        break;
    case pixeloids_luchelli::GameStates::PAUSED:
        DrawText("Paused", 350, 250, 20, WHITE);
        DrawButton(resumeButton);
        DrawButton(backToMenuButton);
        break;
    case pixeloids_luchelli::GameStates::GAME_OVER:
        DrawText("Game Over", 350, 250, 20, WHITE);
        DrawButton(backToMenuButton);
        DrawButton(exitButton);
        break;
    case pixeloids_luchelli::GameStates::EXIT:
        break;
    default:
        break;
    }

    EndDrawing();
}

void close()
{
    CloseWindow();
}


void initializeButtons() 
{
    playButton = CreateButton({ 100, 100 }, { 150, 50 }, "Play");
    resumeButton = CreateButton({ 100, 200 }, { 150, 50 }, "Resume");
    exitButton = CreateButton({ 100, 300 }, { 150, 50 }, "Exit");
    backToMenuButton = CreateButton({ 100, 400 }, { 150, 50 }, "Menu");
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
        Vector2 position = { 0,0 };
        Vector2 velocity = { 0,0 };

        getRandomPosAndVelocity(position, velocity);

        InitializeAsteroid(asteroidsArray[i], position, velocity, AsteroidSize::LARGE);
        activeAsteroidCount++;
    }

}

void getRandomPosAndVelocity(Vector2& position, Vector2& velocity)
{
    // Random border (0 = left, 1 = right, 2 = top, 3 = bottom)
    int border = GetRandomValue(static_cast<int>(Borders::LEFT), static_cast<int>(Borders::BOTTOM));
    switch (static_cast<Borders>(border))
    {
    case Borders::LEFT: // Left border
        position = { 0, static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
        velocity = { asteroidStartSpeed, static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))) };
        break;
    case Borders::RIGHT: // Right border
        position = { static_cast<float>(GetScreenWidth()), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
        velocity = { -asteroidStartSpeed, static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))) };
        break;
    case Borders::TOP: // Top border
        position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), 0 };
        velocity = { static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))), asteroidStartSpeed };
        break;
    case Borders::BOTTOM: // Bottom border
        position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), static_cast<float>(GetScreenHeight()) };
        velocity = { static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))), -asteroidStartSpeed };
        break;
    }
}

void updateMenu()
{
    if (IsButtonClicked(playButton))
        gameState.nextState = GameStates::PLAYING;
    if (IsButtonClicked(exitButton))
        gameState.nextState = GameStates::EXIT;
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

void handleBulletAsteroidCollisions(Bullet bulletsArray[], Asteroid asteroidsArray[], int& asteroidCount)
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bulletsArray[i].active)
        {
            for (int j = 0; j < asteroidCount; j++)
            {
                if (asteroidsArray[j].active)
                {
                    float distance = Vector2Distance(bulletsArray[i].position, asteroidsArray[j].position);
                    if (distance < bulletsArray[i].radius + asteroidsArray[j].radius)
                    {
                        // Deactivate bullet and destroy asteroid
                        bullets[i].active = false;
                        DestroyAsteroid(asteroidsArray[j], asteroidsArray, asteroidCount);

                        if (asteroidsArray[j].size == AsteroidSize::SMALL)
                        {
                            smallAsteroidDestroyedCount++;
                            if (smallAsteroidDestroyedCount >= 2)
                            {
                                Vector2 velocity = { 0,0 };
                                Vector2 position = { 0,0 };
                                getRandomPosAndVelocity(position, velocity);
                                InitializeAsteroid(asteroidsArray[j], position, velocity, AsteroidSize::LARGE);
                                smallAsteroidDestroyedCount = 0;
                            }
                        }
                                               
                        break;
                    }
                }
            }
        }
    }
}

}