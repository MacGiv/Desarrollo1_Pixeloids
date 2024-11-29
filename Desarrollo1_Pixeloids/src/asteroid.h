#include "raylib.h"

#ifndef ASTEROID_H
#define ASTEROID_H

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

const int maxLargeAsteroids = 15;
const int maxMediumAsteroids = 35;
const int maxSmallAsteroids = 50;

void InitializeAsteroid(Asteroid& asteroid, Vector2 position, Vector2 velocity, AsteroidSize size);

void UpdateAsteroid(Asteroid& asteroid);

void DrawAsteroid(Asteroid& asteroid, Texture2D& asteroidSprite);

void DestroyAsteroid(Asteroid& asteroid, Asteroid asteroids[], int& asteroidCount, int& score, Sound asteroidDestroySfx);

} // namespace pixeloids_luchelli 

#endif // ASTEROID_H