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
    };

    void InitializePlayer(Player& player);

    void UpdatePlayer(Player& player);

    void DrawPlayer(const Player& player);

}

#endif