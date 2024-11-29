#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

namespace pixeloids_luchelli
{
struct Player 
{
    Vector2 position;
    Vector2 velocity;
    float rotation; // Angle to mouse position
    float radius;
    float speed;
    float maxSpeed;
    Texture2D sprite;
    bool shieldActive = false;
    float shieldTimer = 0.0f;
};

void initializePlayer(Player& player);

void updatePlayer(Player& player);

void drawPlayer(Player& player);

void activateShield(Player& player);

} // namespace pixeloids_luchelli

#endif // PLAYER_H