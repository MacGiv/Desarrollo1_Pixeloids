#include "bullet.h"
#include "raymath.h"

namespace pixeloids_luchelli
{


void fireBullet(Bullet bullets[], int arraySize, Vector2 position, Vector2 direction)
{
    for (int i = 0; i < arraySize; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].position = position;
            bullets[i].velocity = Vector2Scale(Vector2Normalize(direction), bullets[i].speed);
            bullets[i].active = true;
            break;
        }
    }
}

void updateBullet(Bullet& bullet) 
{
    if (bullet.active) 
    {
        bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, GetFrameTime()));

        // Deactivate bullet if it's not on screen
        if (bullet.position.x < 0 || bullet.position.x > GetScreenWidth() ||
            bullet.position.y < 0 || bullet.position.y > GetScreenHeight()) 
        {
            bullet.active = false;
        }
    }
}

void drawBullet(Bullet bullet) 
{
    if (bullet.active) 
    {
        DrawCircleV(bullet.position, bullet.radius, YELLOW);
    }
}



}