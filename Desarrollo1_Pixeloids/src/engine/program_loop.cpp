#include "engine/program_loop.h"

#include "raylib.h"

#include "engine/state_machine.h"
#include "scenes/main_menu.h"
#include "entities/player.h"
#include "scenes/gameplay.h"

namespace pixeloids_luchelli
{

extern Player player;

GameStateMachine gameState{};
Texture2D aSprite;
Texture2D currentBulletSprite;
Texture2D backgroundImage;
Texture2D hudLifeSprite;
Texture2D hudScoreSprite;
Texture2D shieldTexture;
Sound shootSfx;
Sound asteroidDestroySfx;
Sound defeatSfx;
Sound buttonSfx;
Music mainMenuMusic;
Music gameplayMusic;
Music optionsMusic;
Font titleFont;

bool wantToExit = false;

static void initialize();
static void update();
static void draw();
static void close();

static void initializeAudio();

void runGame()
{
    initialize();

    while (!WindowShouldClose() && !wantToExit)
    {
        update();

        draw();
    }

    close();
}

void initialize()
{
    initializeAudio();
    initializeGame();
    initializeMenuButtons();
}

void update()
{
    switch (gameState.currentState)
    {
    case pixeloids_luchelli::GameStates::MENU:
        updateMenu();
        break;
    case pixeloids_luchelli::GameStates::HOW_TO_PLAY:
        updateHowToPlay();
        break;
    case pixeloids_luchelli::GameStates::PLAYING:
        updateGame();
        break;
    case pixeloids_luchelli::GameStates::PAUSED:
        updatePause();
        break;
    case pixeloids_luchelli::GameStates::GAME_OVER:
        updateGameOver();
        break;
    case pixeloids_luchelli::GameStates::CREDITS:
        updateCredits();
        break;
    case pixeloids_luchelli::GameStates::EXIT:
        wantToExit = true;
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
    case pixeloids_luchelli::GameStates::HOW_TO_PLAY:
        drawHowToPlay();
        break;
    case pixeloids_luchelli::GameStates::PLAYING:
        drawGame();
        break;
    case pixeloids_luchelli::GameStates::PAUSED:
        drawPause();
        break;
    case pixeloids_luchelli::GameStates::GAME_OVER:
        drawGameOver();
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
    unloadMenuRes();
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
    UnloadMusicStream(mainMenuMusic);
    UnloadMusicStream(gameplayMusic);
    UnloadMusicStream(optionsMusic);
    if (IsAudioDeviceReady())
    {
        CloseAudioDevice();
    }
    CloseWindow();
}

void initializeAudio()
{
    if (!IsAudioDeviceReady())
    {
        InitAudioDevice();
    }

    mainMenuMusic = LoadMusicStream("res/main_menu_music.mp3");
    SetMusicVolume(mainMenuMusic, 0.75f);
    gameplayMusic = LoadMusicStream("res/gameplay_music.mp3");
    SetMusicVolume(gameplayMusic, 0.75f);
    optionsMusic = LoadMusicStream("res/options_music.mp3");
    SetMusicVolume(optionsMusic, 0.75f);
    buttonSfx = LoadSound("res/button_press_sfx.mp3");
    SetSoundVolume(buttonSfx, 0.75f);
    shootSfx = LoadSound("res/player_laser_fire_sfx.mp3");
    SetSoundVolume(shootSfx, 0.75f);
    asteroidDestroySfx = LoadSound("res/asteroid_explosion_sfx.mp3");
    SetSoundVolume(asteroidDestroySfx, 0.2f);
    defeatSfx = LoadSound("res/defeat_sfx.mp3");
    SetSoundVolume(defeatSfx, 0.3f);
    PlayMusicStream(mainMenuMusic);
}

} // namespace pixeloids_luchelli