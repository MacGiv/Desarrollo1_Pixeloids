#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

namespace pixeloids_luchelli
{
    struct Player 
    {
        Vector2 position;
        Vector2 velocity;
        float rotation; // Angulo hacia el mouse
        float radius;   // Radio del circulo que representa la nave
        float speed;    // Velocidad de aceleracion
        float maxSpeed; // Velocidad maxima
    };

    void InitializePlayer(Player& player);

    void UpdatePlayer(Player& player);

    void DrawPlayer(const Player& player);

}

#endif // PLAYER_H