#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"

namespace pixeloids_luchelli
{

struct Bullet 
{
    Vector2 position;
    Vector2 velocity;
    bool active;
    float radius;
};

void InitializeBullet(Bullet& bullet);

void ShootBullet(Bullet& bullet, Vector2 startPos, Vector2 direction, float speed);

void UpdateBullet(Bullet& bullet);

void DrawBullet(const Bullet& bullet);

#endif

}