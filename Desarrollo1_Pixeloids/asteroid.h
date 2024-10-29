#pragma once
#include "raylib.h"

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

void DrawAsteroid(Asteroid asteroid);

void DestroyAsteroid(Asteroid& asteroid, Asteroid asteroids[], int& asteroidCount);

}  