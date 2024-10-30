#include "asteroid.h"
#include "raymath.h"

namespace pixeloids_luchelli
{

const float smallAsteroidSize = 10.0f;
const float medAsteroidSize = 20.0f;
const float largeAsteroidSize = 40.0f;


void InitializeAsteroid(Asteroid& asteroid, Vector2 position, Vector2 velocity, AsteroidSize size)
{
    asteroid.position = position;
    asteroid.velocity = velocity;
    asteroid.size = size;
    asteroid.active = true;

    switch (size) 
    {
    case AsteroidSize::LARGE: 
        asteroid.radius = largeAsteroidSize;
        break;
    case AsteroidSize::MEDIUM: 
        asteroid.radius = medAsteroidSize;
        break;
    case AsteroidSize::SMALL: 
        asteroid.radius = smallAsteroidSize;
        break;
    }
}

void UpdateAsteroid(Asteroid& asteroid) 
{
    if (asteroid.active) 
    {
        asteroid.position = Vector2Add(asteroid.position, Vector2Scale(asteroid.velocity, GetFrameTime()));

        if (asteroid.position.x < 0) 
            asteroid.position.x = static_cast<float>(GetScreenWidth());
        else if (asteroid.position.x > GetScreenWidth()) 
            asteroid.position.x = 0;

        if (asteroid.position.y < 0) 
            asteroid.position.y = static_cast<float>(GetScreenHeight());

        else if (asteroid.position.y > GetScreenHeight()) 
            asteroid.position.y = 0;
    }
}

void DrawAsteroid(Asteroid& asteroid, Texture2D& asteroidSprite)
{
    if (asteroid.active)
    {
        float scaleFactor = asteroid.radius / 32.0f;  

        Rectangle sourceRect = { 0.0f, 0.0f, 64.0f, 64.0f }; 
        Rectangle destRect = { asteroid.position.x, asteroid.position.y, 64.0f * scaleFactor, 64.0f * scaleFactor };
        Vector2 origin = { 32.0f, 32.0f }; 

        DrawTexturePro(
            asteroidSprite,            
            sourceRect,                
            destRect,                  
            origin,                    
            0.0f,                      
            WHITE                      
        );
    }
}

void DestroyAsteroid(Asteroid& asteroid, Asteroid asteroids[], int& asteroidCount, int& score) 
{

    asteroid.active = false;
    asteroidCount--;
    int maxAsteroids = maxLargeAsteroids + maxMediumAsteroids + maxSmallAsteroids;
    float rotationAngle = 30.0f;

    // Create new asteroids if the one destroyed is big or med
    if (asteroid.size == AsteroidSize::LARGE || asteroid.size == AsteroidSize::MEDIUM) 
    {
        AsteroidSize newSize = (asteroid.size == AsteroidSize::LARGE) ? AsteroidSize::MEDIUM : AsteroidSize::SMALL;
        Vector2 newVelocity1 = Vector2Rotate(asteroid.velocity, rotationAngle);
        Vector2 newVelocity2 = Vector2Rotate(asteroid.velocity, -rotationAngle);

        for (int i = 0; i < maxAsteroids; i++)
        {
            if (!asteroids[i].active) 
            {
                InitializeAsteroid(asteroids[i], asteroid.position, newVelocity1, newSize);
                asteroidCount++;
                i++;
                if (i < maxAsteroids)
                {
                    InitializeAsteroid(asteroids[i], asteroid.position, newVelocity2, newSize);
                    asteroidCount++;
                    score += 10;
                }
                break;
            }
        }
    }
}



}