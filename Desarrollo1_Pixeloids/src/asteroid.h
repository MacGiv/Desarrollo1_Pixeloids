#include "raylib.h"

#ifndef ASTEROID
#define ASTEROID

namespace pixeloids_luchelli 
{

enum class AsteroidSize { LARGE, MEDIUM, SMALL };

struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
    AsteroidSize size;
};



void InitializeAsteroid(Asteroid& asteroid, Vector2 position, Vector2 velocity, AsteroidSize size);

void UpdateAsteroid(Asteroid& asteroid);

void DrawAsteroid(Asteroid& asteroid, Texture2D& asteroidSprite);

void DestroyAsteroid(Asteroid& asteroid, Asteroid asteroids[], int& asteroidCount, int& score, Sound asteroidDestroySfx);

}

#endif