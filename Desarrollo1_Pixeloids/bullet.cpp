#include "bullet.h"
#include "raymath.h"

namespace pixeloids_luchelli
{

void InitializeBullet(Bullet& bullet) 
{
    bullet.position = { 0, 0 };
    bullet.velocity = { 0, 0 };
    bullet.active = false;
    bullet.radius = 5.0f; 
}

void ShootBullet(Bullet& bullet, Vector2 startPos, Vector2 direction, float speed) 
{
    if (!bullet.active) 
    {
        bullet.position = startPos;
        bullet.velocity = Vector2Scale(Vector2Normalize(direction), speed);
        bullet.active = true;
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

void DrawBullet(const Bullet& bullet) 
{
    if (bullet.active) 
    {
        DrawCircleV(bullet.position, bullet.radius, YELLOW);
    }
}

}