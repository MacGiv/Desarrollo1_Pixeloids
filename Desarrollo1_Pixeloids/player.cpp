#include "player.h"
#include "raymath.h"

namespace pixeloids_luchelli
{

void initializePlayer(Player& player) 
{
    player.position = { 400.0f, 300.0f }; // Start pos
    player.velocity = { 0.0f, 0.0f };
    player.rotation = 0.0f;
    player.radius = 20.0f;
    player.speed = 500.0f; 
    player.maxSpeed = 800.0f;
}

void updatePlayer(Player& player)
{
    // Get Mouse direction
    Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
    player.rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    // Right click movement
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) 
    {
        Vector2 acceleration = Vector2Scale(Vector2Normalize(direction), player.speed * GetFrameTime());

        player.velocity = Vector2Add(player.velocity, acceleration);
    }

    // Limit velocity
    if (Vector2Length(player.velocity) > player.maxSpeed) 
    {
        player.velocity = Vector2Scale(Vector2Normalize(player.velocity), player.maxSpeed);
    }

    // Update position
    player.position = Vector2Add(player.position, Vector2Scale(player.velocity, GetFrameTime()));

    // Screen borders check
    if (player.position.x < -player.radius) 
        player.position.x = static_cast<float>(GetScreenWidth()) + player.radius;

    if (player.position.x > GetScreenWidth() + player.radius) 
        player.position.x = -player.radius;

    if (player.position.y < -player.radius) 
        player.position.y = static_cast<float>(GetScreenHeight()) + player.radius;

    if (player.position.y > GetScreenHeight() + player.radius) 
        player.position.y = -player.radius;
}

void drawPlayer(const Player& player) 
{
    DrawCircleV(player.position, player.radius, RED);

    // Debug direction line start
    float lineLength = 50.0f;
    Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
    Vector2 lineEnd = Vector2Add(player.position, Vector2Scale(direction, lineLength));
    DrawLineV(player.position, lineEnd, RAYWHITE);
    // Debug direction line end
}

}