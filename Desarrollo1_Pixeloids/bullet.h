#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"

namespace pixeloids_luchelli
{

const float bulletDefaultSpeed = 800.0f;
const float bulletDefaultRadius = 5.0f;

struct Bullet 
{
    Vector2 position;
    Vector2 velocity;
    bool active;
    float radius;
    float speed;
};

void InitializeBulletArray(Bullet bullets[], int size);

void FireBullet(Bullet bullets[], int size, Vector2 position, Vector2 direction);

void UpdateBullet(Bullet& bullet);

void DrawBullet(Bullet bullet);

#endif

}