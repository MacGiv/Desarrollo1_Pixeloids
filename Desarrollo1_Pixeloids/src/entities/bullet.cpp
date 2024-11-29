#include "entities/bullet.h"

#include "raymath.h"

namespace pixeloids_luchelli
{


void fireBullet(Bullet bullets[], int arraySize, Vector2 position, Vector2 direction, Sound bulletSound)
{
    for (int i = 0; i < arraySize; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].position = position;
            bullets[i].velocity = Vector2Scale(Vector2Normalize(direction), bullets[i].speed);
            bullets[i].active = true;
            SetSoundVolume(bulletSound, 0.2f);
            PlaySound(bulletSound);

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

void drawBullet(Bullet bullet, Texture2D texture, float bulletRotation)
{
    if (bullet.active) 
    {
        //DrawCircleV(bullet.position, bullet.radius, YELLOW);

        Rectangle sourceRect = { 0.0f, 0.0f, 64.0f, 64.0f };
        Rectangle destRect = { bullet.position.x, bullet.position.y,
                               bullet.radius * 3, bullet.radius * 3 };
        Vector2 origin = { 20.0f, 20.0f };  // 20 for player radius

        DrawTexturePro(
            texture,
            sourceRect,
            destRect,
            origin,
            bulletRotation + 90,
            WHITE
        );

    }
}



} // namespace pixeloids_luchelli