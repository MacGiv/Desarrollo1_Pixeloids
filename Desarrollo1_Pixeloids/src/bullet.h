#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"

namespace pixeloids_luchelli
{

const float bulletDefaultSpeed = 1000.0f;
const float bulletDefaultRadius = 5.0f;

struct Bullet 
{
    Vector2 position;
    Vector2 velocity;
    bool active;
    float radius;
    float speed;
};

void fireBullet(Bullet bullets[], int size, Vector2 position, Vector2 direction);

void updateBullet(Bullet& bullet);

void drawBullet(Bullet bullet);

#endif

}