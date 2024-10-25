#include "bullet.h"
#include "raymath.h"

namespace pixeloids_luchelli
{


void InitializeBulletArray(Bullet bullets[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        bullets[i].active = false;
        bullets[i].radius = bulletDefaultRadius;
        bullets[i].speed = bulletDefaultSpeed;
    }
}

void FireBullet(Bullet bullets[], int arraySize, Vector2 position, Vector2 direction)
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

void UpdateBullet(Bullet& bullet) 
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

void DrawBullet(Bullet bullet) 
{
    if (bullet.active) 
    {
        DrawCircleV(bullet.position, bullet.radius, YELLOW);
    }
}



}