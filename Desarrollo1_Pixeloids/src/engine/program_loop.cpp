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

Music mainMenuMusic;
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
    unloadGameRes();
    UnloadMusicStream(mainMenuMusic);
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
    optionsMusic = LoadMusicStream("res/options_music.mp3");
    SetMusicVolume(optionsMusic, 0.75f);
    

    PlayMusicStream(mainMenuMusic);
}

} // namespace pixeloids_luchelli