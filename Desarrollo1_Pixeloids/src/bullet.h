#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"
#include "player.h"

namespace pixeloids_luchelli
{

const float bulletDefaultSpeed = 1000.0f;
const float bulletDefaultRadius = 10.0f;

struct Bullet 
{
    Vector2 position;
    Vector2 velocity;
    bool active;
    float radius;
    float speed;
};

void fireBullet(Bullet bullets[], int size, Vector2 position, Vector2 direction, Sound bulletSound);

void updateBullet(Bullet& bullet);

void drawBullet(Bullet bullet, Texture2D texture, float bulletRotation);


} // namespace pixeloids_luchelli

#endif // BULLET_H