#include "game_loop.h"

namespace pixeloids_luchelli
{

void runGame()
{
	// Initialization
	initializeGame();

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		update();

		draw();
	}

	//Close game
	close();
}

void initializeGame()
{

	InitWindow(screenWidth, screenHeight, "PONG");

}

void update()
{

}

void draw()
{

}

void close()
{
	CloseWindow();
}

}