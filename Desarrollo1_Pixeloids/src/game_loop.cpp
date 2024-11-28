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
const int lifeTextSize = 20;
const float asteroidStartSpeed = 150.0f;
Player player;
Bullet bullets[maxBullets];
Asteroid asteroids[totalAsteroids];
GameStateMachine gameState{};
Texture2D aSprite;
Texture2D currentBulletSprite;
Sound shootSfx;
Sound asteroidDestroySfx;
Sound defeatSfx;
Music mainMenuMusic;
Music gameplayMusic;


int activeAsteroidCount = 0;
int playerScore = 0;
int smallAsteroidDestroyedCount = 0;
int playerCurrentLives = playerMaxLives;

static Button playButton, resumeButton, exitButton, backToMenuButton, creditsButton, pauseButton;

static void initializeGame();
static void update();
static void draw();
static void close();
static void initializeButtons();
static void initializeAudio();
static void initializeBulletArray(Bullet bullets[], int arraySize);
static void initializeAsteroids(Asteroid asteroidsArray[]);
static void getRandomPosAndVelocity(Vector2& position, Vector2& velocity);
static void updateMenu();
static void updateAsteroids(Asteroid asteroidsArray[]);
static void drawMenu();
static void drawCredits();
static void drawAsteroids(Asteroid asteroidsArray[], Texture2D asteroidSprite);
static void drawPlayerLives(int lives);
static void drawScore(int score);
static void handleBulletAsteroidCollisions(Bullet bullets[], Asteroid asteroidsArray[], int& asteroidCount);
static void handlePlayerAsteroidCollisions(Player& auxPlayer, Asteroid asteroidsArray[], int& asteroidCount);



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
    if (gameState.currentState != GameStates::GAME_OVER)
    {
        gameState.currentState = GameStates::MENU;
        gameState.nextState = GameStates::MENU;
        InitWindow(screenWidth, screenHeight, "Asteroids");
    }

    playerScore = 0;
    playerCurrentLives = playerMaxLives;

    initializePlayer(player);

    initializeBulletArray(bullets, maxBullets);

    aSprite = LoadTexture("res/asteroid.png");

    currentBulletSprite = LoadTexture("res/asteroid.png");

    initializeAudio();

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
        if (gameState.currentState != gameState.nextState)
        {
            StopMusicStream(mainMenuMusic);
            PlayMusicStream(gameplayMusic);
        }

        UpdateMusicStream(gameplayMusic);
        updatePlayer(player);

        // Bullet Update
        for (int i = 0; i < maxBullets; i++)
        {
            updateBullet(bullets[i]);
        }

        // Shoot update
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (isButtonClicked(pauseButton))
            {
                gameState.nextState = GameStates::PAUSED;
            }
            else
            {
                Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
                fireBullet(bullets, maxBullets, player.position, direction, shootSfx);
            }
        }

        // Asteroids update
        updateAsteroids(asteroids);

        // Collision check
        handleBulletAsteroidCollisions(bullets, asteroids, activeAsteroidCount);
        handlePlayerAsteroidCollisions(player, asteroids, activeAsteroidCount);
        break;
    case pixeloids_luchelli::GameStates::PAUSED:
        if (isButtonClicked(resumeButton))
            gameState.nextState = GameStates::PLAYING;

        if (isButtonClicked(backToMenuButton))
        {
            gameState.nextState = GameStates::MENU;
            PlayMusicStream(mainMenuMusic);
        }
        break;
    case pixeloids_luchelli::GameStates::GAME_OVER:
        if (isButtonClicked(backToMenuButton))
        {
            gameState.nextState = GameStates::MENU;
            initializeGame();
        }
        if (isButtonClicked(exitButton))
            gameState.nextState = GameStates::EXIT;
        break;
    case pixeloids_luchelli::GameStates::CREDITS:
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            gameState.nextState = GameStates::MENU;
            PlayMusicStream(mainMenuMusic);
        }
        break;
    case pixeloids_luchelli::GameStates::EXIT:
        close();
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
        drawMenu();
        break;
    case pixeloids_luchelli::GameStates::PLAYING:
        for (int i = 0; i < maxBullets; i++)
        {
            drawBullet(bullets[i], currentBulletSprite);
        }

        drawPlayer(player);
        drawAsteroids(asteroids, aSprite);
        drawButton(pauseButton);
        drawPlayerLives(playerCurrentLives);
        drawScore(playerScore);
        break;
    case pixeloids_luchelli::GameStates::PAUSED:
        DrawText("Paused", GetScreenWidth() / 2, (GetScreenHeight() / 5) * 2, 20, WHITE);
        drawButton(resumeButton);
        drawButton(backToMenuButton);
        break;
    case pixeloids_luchelli::GameStates::GAME_OVER:
        DrawText("Game Over", 350, 250, 20, WHITE);
        drawButton(backToMenuButton);
        drawButton(exitButton);
        break;
    case pixeloids_luchelli::GameStates::CREDITS:
        drawCredits();
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
    UnloadTexture(player.sprite);
    UnloadTexture(aSprite);
    UnloadTexture(currentBulletSprite);
    UnloadSound(shootSfx);
    UnloadSound(asteroidDestroySfx);
    UnloadSound(defeatSfx);
    UnloadMusicStream(mainMenuMusic);
    UnloadMusicStream(gameplayMusic);
    CloseAudioDevice();
    CloseWindow();
}


void initializeButtons() 
{
    playButton = createButton({ 100, 100 }, { 150, 50 }, "Play");
    creditsButton = createButton({ 100, 300 }, { 150, 50 }, "Credits");
    exitButton = createButton({ 100, 500 }, { 150, 50 }, "Exit");

    float backToMenuX = (screenWidth / 16);
    float pauseX = (screenWidth / 16) * 14;
    float pauseY = (screenHeight / 8) * 7;
    resumeButton = createButton({ pauseX, pauseY }, { 100, 50 }, "Resume");
    pauseButton = createButton({ pauseX, pauseY }, { 100, 50 }, "Pause");
    backToMenuButton = createButton({ backToMenuX, pauseY }, { 100, 50 }, "Menu");
}

void initializeAudio()
{
    if (!IsAudioDeviceReady())
    {
        InitAudioDevice();
    }
    mainMenuMusic = LoadMusicStream("res/main_menu_music.mp3");
    gameplayMusic = LoadMusicStream("res/gameplay_music.mp3");
    SetMusicVolume(gameplayMusic, 0.75f);
    shootSfx = LoadSound("res/player_laser_fire_sfx.mp3");
    asteroidDestroySfx = LoadSound("res/asteroid_explosion_sfx.mp3");
    SetSoundVolume(asteroidDestroySfx, 0.2f);
    defeatSfx = LoadSound("res/defeat_sfx.mp3");
    SetSoundVolume(defeatSfx, 0.3f);
    PlayMusicStream(mainMenuMusic);
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
    // Borders (0 = left, 1 = right, 2 = top, 3 = bottom)
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
    UpdateMusicStream(mainMenuMusic);

    if (isButtonClicked(playButton))
    {
        StopMusicStream(mainMenuMusic);
        gameState.nextState = GameStates::PLAYING;
        PlayMusicStream(gameplayMusic);
    }
    if (isButtonClicked(exitButton))
    {
        StopMusicStream(mainMenuMusic);
        gameState.nextState = GameStates::EXIT;
    }
    if (isButtonClicked(creditsButton))
    {
        StopMusicStream(mainMenuMusic);
        gameState.nextState = GameStates::CREDITS;
    }
}

void updateAsteroids(Asteroid asteroidsArray[])
{
    for (int i = 0; i < totalAsteroids; i++) 
    {
        UpdateAsteroid(asteroidsArray[i]);
    }
}

void drawMenu()
{
    Vector2 titlePosition = { (static_cast<float>(GetScreenWidth()) - MeasureText("PIXELOIDS", 40)) / 2.0f, 50.0f };
    DrawText("PIXELOIDS", static_cast<int>(titlePosition.x), static_cast<int>(titlePosition.y), 40, ORANGE);
    
    drawButton(playButton);
    drawButton(exitButton);
    drawButton(creditsButton);
}

void drawCredits()
{
    ClearBackground(BLACK);

    int titleSize = 60;
    int normalTextSize = 30;

    const char* title = "Credits";
    int titleX = screenWidth / 2 - MeasureText(title, titleSize) / 2;
    int titleY = (screenHeight / 5) * 1;

    const char* credits = "Made by Tomas Francisco Luchelli";
    int creditsX = screenWidth / 2 - MeasureText(credits, normalTextSize) / 2;
    int creditsY = (screenHeight / 5) * 3;

    const char* returnText = "Press Mouse Button to Return";
    int returnTextX = screenWidth / 2 - MeasureText(returnText, normalTextSize) / 2;
    int returnTextY = (screenHeight / 5) * 4;

    DrawText(title, titleX, titleY, titleSize, ORANGE);
    DrawText(credits, creditsX, creditsY, normalTextSize, WHITE);
    DrawText(returnText, returnTextX, returnTextY, normalTextSize, GRAY);
}

void drawAsteroids(Asteroid asteroidsArray[], Texture2D asteroidSprite)
{
    for (int i = 0; i < totalAsteroids; i++) 
    {
        DrawAsteroid(asteroidsArray[i], asteroidSprite);
    }
}

void drawPlayerLives(int lives)
{
    const char* lifeText = TextFormat("Lives: %d", lives);
    int textWidth = MeasureText(lifeText, lifeTextSize);
    int posX = (screenWidth - textWidth) / 2;
    int posY = screenHeight / 16; 

    DrawText(lifeText, posX, posY, lifeTextSize, WHITE);
}

void drawScore(int score)
{
    // Configura la posición en la esquina superior derecha
    Vector2 position = { static_cast<float>(GetScreenWidth() - 100), 10.0f }; // Ajusta el '100' según el tamaño del texto

    // Puedes usar el tamaño y color que prefieras
    DrawText(TextFormat("Score: %d", score), static_cast<int>(position.x), static_cast<int>(position.y), 20, WHITE);
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
                        DestroyAsteroid(asteroidsArray[j], asteroidsArray, asteroidCount, playerScore, asteroidDestroySfx);

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

void handlePlayerAsteroidCollisions(Player& auxPlayer, Asteroid asteroidsArray[], int& asteroidCount)
{
    for (int i = 0; i < asteroidCount; i++)
    {
        if (asteroidsArray[i].active)
        {
            float distance = Vector2Distance(auxPlayer.position, asteroidsArray[i].position);
            if (distance < auxPlayer.radius + asteroidsArray[i].radius)
            {
                playerCurrentLives--;
                asteroidsArray[i].active = false;
                PlaySound(asteroidDestroySfx);
                if (playerCurrentLives <= 0)
                {
                    if (IsMusicStreamPlaying(gameplayMusic))
                    {
                        StopMusicStream(gameplayMusic);
                    }
                    gameState.nextState = GameStates::GAME_OVER;
                }
                break; 
            }
        }
    }
}


} // namespace pixeloids_luchelli