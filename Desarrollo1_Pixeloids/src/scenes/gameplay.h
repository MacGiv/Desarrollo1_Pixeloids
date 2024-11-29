#ifndef GAME_LOOP
#define GAME_LOOP

namespace pixeloids_luchelli
{

void initializeGame();

void updateGame();
void updatePause();
void updateGameOver();

void drawGame();
void drawPause();
void drawGameOver();

} // namespace pixeloids_luchelli

#endif // GAME_LOOP