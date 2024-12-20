#include "scenes/gameplay.h"

#include "raylib.h"
#include "raymath.h"

#include "entities/player.h"
#include "entities/bullet.h"
#include "entities/asteroid.h"
#include "ui/button.h"
#include "engine/game_data.h"
#include "engine/state_machine.h"

namespace pixeloids_luchelli
{

enum class Borders {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

const int playerMaxLives = 5;
const int maxBullets = 100;
const int maxAsteroids = 100;
const int lifeTextSize = 20;
const float asteroidStartSpeed = 100.0f;
const float asteroidSpawnInterval = 3.0f;

Player player;
Bullet bullets[maxBullets];
Asteroid asteroids[maxAsteroids];
Button backToMenuButton;
static Button resumeButton, exitButton,  pauseButton, replayButton;

extern GameStateMachine gameState;

Texture2D aSprite;
Texture2D currentBulletSprite;
Texture2D backgroundImage;
Texture2D hudLifeSprite;
Texture2D hudScoreSprite;
Texture2D shieldTexture;
Music gameplayMusic;
Sound shootSfx;
Sound asteroidDestroySfx;
Sound defeatSfx;
Sound buttonSfx;
extern Music mainMenuMusic;
extern bool wantToExit;

float asteroidSpawnTimer = 0.0f;
int activeAsteroidCount = 0;
int playerScore = 0;
int smallAsteroidDestroyedCount = 0;
int playerCurrentLives = playerMaxLives;

static void initializeGameButtons();
static void initializeBulletArray(Bullet bullets[], int arraySize);
static void initializeAsteroids(Asteroid asteroidsArray[]);
static void initializeGameAudio();
static void initializeGameSprites();


static void drawGameplayBackground();
static void drawAsteroids(Asteroid asteroidsArray[], Texture2D asteroidSprite);
static void drawPlayerLives(int lives);
static void drawScore(int score);

static void handleBulletAsteroidCollisions(Bullet bullets[], Asteroid asteroidsArray[], int& asteroidCount);
static void handlePlayerAsteroidCollisions(Player& auxPlayer, Asteroid asteroidsArray[], int& asteroidCount);
static void getRandomPosAndVelocity(Vector2& position, Vector2& velocity);
static void spawnAsteroid(int& asteroidCount);



void initializeGame()
{
    if (gameState.currentState != GameStates::GAME_OVER)
    {
        gameState.currentState = GameStates::MENU;
        gameState.nextState = GameStates::MENU;
        InitWindow(screenWidth, screenHeight, "Pixeloids");
    }

    playerScore = 0;
    playerCurrentLives = playerMaxLives;

    initializePlayer(player);

    initializeBulletArray(bullets, maxBullets);

    initializeGameSprites();

    initializeGameAudio();

    initializeAsteroids(asteroids);

    initializeGameButtons();
}

void initializeGameAudio()
{
    gameplayMusic = LoadMusicStream("res/gameplay_music.mp3");
    SetMusicVolume(gameplayMusic, 0.75f);
    buttonSfx = LoadSound("res/button_press_sfx.mp3");
    SetSoundVolume(buttonSfx, 0.75f);
    shootSfx = LoadSound("res/player_laser_fire_sfx.mp3");
    SetSoundVolume(shootSfx, 0.75f);
    asteroidDestroySfx = LoadSound("res/asteroid_explosion_sfx.mp3");
    SetSoundVolume(asteroidDestroySfx, 0.2f);
    defeatSfx = LoadSound("res/defeat_sfx.mp3");
    SetSoundVolume(defeatSfx, 0.3f);
}

void initializeGameSprites()
{
    backgroundImage = LoadTexture("res/background_image.png");
    shieldTexture = LoadTexture("res/shield.png");
    aSprite = LoadTexture("res/asteroid.png");
    hudLifeSprite = LoadTexture("res/hud_life.png");
    hudScoreSprite = LoadTexture("res/hud_score.png");
    currentBulletSprite = LoadTexture("res/bullet_sprite.png");
}


void initializeGameButtons() 
{
    float backToMenuX = (screenWidth / 16);
    float pauseX = (screenWidth / 16) * 14;
    float pauseY = (screenHeight / 8) * 7;
    float replayX = screenWidth / 2 - 60;
    float replayY = screenHeight / 2;
    resumeButton = createButton({ pauseX, pauseY }, { 100, 50 }, "Resume", PINK_MINE);
    pauseButton = createButton({ pauseX, pauseY }, { 100, 50 }, "Pause", PINK_MINE);
    backToMenuButton = createButton({ backToMenuX, pauseY }, { 100, 50 }, "Menu", PINK_MINE);
    replayButton = createButton({ replayX, replayY }, { 120, 60 }, "Replay", PINK_MINE);
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

void updateGame()
{
    if (gameState.currentState != gameState.nextState)
    {
        StopMusicStream(mainMenuMusic);
        PlayMusicStream(gameplayMusic);
    }

    UpdateMusicStream(gameplayMusic);

    // Player Update
    if (player.shieldActive) 
    {
        player.shieldTimer -= GetFrameTime();
        if (player.shieldTimer <= 0) 
        {
            player.shieldActive = false;
        }
    }
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
            PlaySound(buttonSfx);
        }
        else
        {
            Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
            fireBullet(bullets, maxBullets, player.position, direction, shootSfx);
        }
    }

    // Asteroids update
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= asteroidSpawnInterval)
    {
        spawnAsteroid(activeAsteroidCount);
        asteroidSpawnTimer = 0.0f;
    }

    for (int i = 0; i < maxLargeAsteroids + maxMediumAsteroids + maxSmallAsteroids; i++)
    {
        if (asteroids[i].active)
        {
            UpdateAsteroid(asteroids[i]);
        }
    }

    // Collisions update
    handleBulletAsteroidCollisions(bullets, asteroids, activeAsteroidCount);
    handlePlayerAsteroidCollisions(player, asteroids, activeAsteroidCount);
}


void updatePause()
{
    if (isButtonClicked(resumeButton))
    {
        gameState.nextState = GameStates::PLAYING;
        PlaySound(buttonSfx);
    }

    if (isButtonClicked(backToMenuButton))
    {
        PlaySound(buttonSfx);
        gameState.nextState = GameStates::MENU;
        PlayMusicStream(mainMenuMusic);
    }
}

void updateGameOver()
{
    if (isButtonClicked(replayButton))
    {
        PlaySound(buttonSfx);
        gameState.nextState = GameStates::PLAYING;
        initializeGame();
        PlayMusicStream(gameplayMusic);
    }
    if (isButtonClicked(backToMenuButton))
    {
        PlaySound(buttonSfx);
        gameState.nextState = GameStates::MENU;
        initializeGame();
    }
    if (isButtonClicked(exitButton))
        gameState.nextState = GameStates::EXIT;
}

void drawGame()
{
    drawGameplayBackground();

    for (int i = 0; i < maxBullets; i++)
    {
        drawBullet(bullets[i], currentBulletSprite, player.rotation);
    }

    drawPlayer(player);
    if (player.shieldActive) 
    {
        Rectangle sourceRec = { 0, 0, (float)shieldTexture.width, (float)shieldTexture.height };
        Rectangle destRec = { player.position.x, player.position.y, 64.0f, 64.0f };
        Vector2 origin = { 32.0f, 32.0f };
        DrawTexturePro(shieldTexture, sourceRec, destRec, origin, 0.0f, Fade(WHITE, 0.5f));
    }
    drawAsteroids(asteroids, aSprite);
    drawButton(pauseButton);
    drawPlayerLives(playerCurrentLives);
    drawScore(playerScore);
}

void drawGameplayBackground()
{
    Rectangle sourceRect = { 0.0f, 0.0f, static_cast<float>(backgroundImage.width), static_cast<float>(backgroundImage.height) };
    Rectangle destRect = { 0.0f, 0.0f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };
    Vector2 origin = { 0.0f, 0.0f }; 

    DrawTexturePro(backgroundImage, sourceRect, destRect, origin, 0.0f, DARKGRAY);
}

void drawAsteroids(Asteroid asteroidsArray[], Texture2D asteroidSprite)
{
    for (int i = 0; i < maxAsteroids; i++) 
    {
        DrawAsteroid(asteroidsArray[i], asteroidSprite);
    }
}

void drawPlayerLives(int lives)
{
    Rectangle sourceRect = { 0.0f, 0.0f, 256.0f, 128.0f };
    Rectangle destRect = { 0.0f, 0.0f, 256.0f, 128.0f };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(hudLifeSprite, sourceRect, destRect, origin, 0.0f, WHITE);

    int posX = static_cast<int>(destRect.x + 20 + destRect.width / 2 - MeasureText(TextFormat("%d", lives), lifeTextSize) / 2);
    int posY = static_cast<int>(destRect.y + 6 + destRect.height / 2 - lifeTextSize / 2);

    DrawText(TextFormat("%d", lives), posX, posY, lifeTextSize, CYAN);
}

void drawScore(int score)
{
    Rectangle sourceRect = { 0.0f, 0.0f, 256.0f, 128.0f };
    Rectangle destRect = { screenWidth - 256.0f, 0.0f, 256.0f, 128.0f };
    Vector2 origin = { 0.0f, 0.0f };
    int scoreTextSize = 20;

    DrawTexturePro(hudScoreSprite, sourceRect, destRect, origin, 0.0f, WHITE);

    int posX = static_cast<int>(destRect.x - 10 + destRect.width / 2 - MeasureText(TextFormat("%d", score), scoreTextSize) / 2);
    int posY = static_cast<int>(destRect.y + 6 + destRect.height / 2 - scoreTextSize / 2);

    DrawText(TextFormat("%d", score), posX, posY, scoreTextSize, YELLOW);
}


void drawPause()
{
    DrawText("Paused", GetScreenWidth() / 2, (GetScreenHeight() / 5) * 2, 20, WHITE);
    drawButton(resumeButton);
    drawButton(backToMenuButton);
}

void drawGameOver()
{

    int titleFontSize = 100;
    int scoreFontSize = 40;
    int spacing = 40;

    const char* gameOverText = "Game Over!";
    const char* scoreText = TextFormat("Your score: %d", playerScore);

    int titleWidth = MeasureText(gameOverText, titleFontSize);
    int titleX = (screenWidth - titleWidth) / 2;
    int titleY = screenHeight / 4;              
    DrawText(gameOverText, titleX, titleY, titleFontSize, CYAN);

    int scoreWidth = MeasureText(scoreText, scoreFontSize);
    int scoreX = (screenWidth - scoreWidth) / 2;
    int scoreY = titleY + titleFontSize + spacing;
    DrawText(scoreText, scoreX, scoreY, scoreFontSize, RAYWHITE);



    drawButton(replayButton);
    drawButton(backToMenuButton);
    drawButton(exitButton);
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
    if (player.shieldActive) return;

    for (int i = 0; i < asteroidCount; i++)
    {
        if (asteroidsArray[i].active)
        {
            float distance = Vector2Distance(auxPlayer.position, asteroidsArray[i].position);
            if (distance < auxPlayer.radius + asteroidsArray[i].radius)
            {
                playerCurrentLives--;
                asteroidsArray[i].active = false;

                player.shieldActive = true;
                player.shieldTimer = 3.0f;

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

void getRandomPosAndVelocity(Vector2& position, Vector2& velocity)
{
    // Borders (0 = left, 1 = right, 2 = top, 3 = bottom)
    int border = GetRandomValue(static_cast<int>(Borders::LEFT), static_cast<int>(Borders::BOTTOM));
    switch (static_cast<Borders>(border))
    {
    case Borders::LEFT: // Left border
        position = { 0, static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
        velocity = { asteroidStartSpeed,
                     static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))) };
        break;
    case Borders::RIGHT: // Right border
        position = { static_cast<float>(GetScreenWidth()), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
        velocity = { -asteroidStartSpeed,
                     static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))) };
        break;
    case Borders::TOP: // Top border
        position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), 0 };
        velocity = { static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))),
                     asteroidStartSpeed };
        break;
    case Borders::BOTTOM: // Bottom border
        position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), static_cast<float>(GetScreenHeight()) };
        velocity = { static_cast<float>(GetRandomValue(-static_cast<int>(asteroidStartSpeed), static_cast<int>(asteroidStartSpeed))),
                     -asteroidStartSpeed };
        break;
    }
}

void spawnAsteroid(int& asteroidCount)
{
    for (int i = 0; i < maxAsteroids; i++)
    {
        if (!asteroids[i].active)
        {
            Vector2 position = {};
            Vector2 velocity = {};
            getRandomPosAndVelocity(position,velocity);
            InitializeAsteroid(asteroids[i], position, velocity, AsteroidSize::LARGE);
            asteroidCount++;
            break;
        }
    }
}

void unloadGameRes()
{
    UnloadTexture(player.sprite);
    UnloadTexture(aSprite);
    UnloadTexture(currentBulletSprite);
    UnloadTexture(backgroundImage);
    UnloadTexture(hudLifeSprite);
    UnloadTexture(hudScoreSprite);
    UnloadTexture(shieldTexture);
    UnloadSound(shootSfx);
    UnloadSound(asteroidDestroySfx);
    UnloadSound(defeatSfx);
    UnloadSound(buttonSfx);
    UnloadMusicStream(gameplayMusic);
}

} // namespace pixeloids_luchelli