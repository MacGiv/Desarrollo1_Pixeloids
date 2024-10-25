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

    void initializePlayer(Player& player);

    void updatePlayer(Player& player);

    void drawPlayer(const Player& player);

}

#endif