#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;

    width = texture.width / maxFrames;
    height = texture.height;
    speed = 3.5f;
}

void Enemy::tick(float deltaTime)
{
    // Check if enemy is alive 
    if (!getAlive()) return;

    // get toTarget vector (from Enemy to Character)
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());

    // If the vector from enemy to character (velocity at this point in function) is < radius, stop chasing and set velocity = 0
    if (Vector2Length(velocity) < radius) velocity = {};
    
    BaseCharacter::tick(deltaTime);

    // Enemy damages character if collision
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        target->takeDamage(damagePerSec * deltaTime);
    }
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}