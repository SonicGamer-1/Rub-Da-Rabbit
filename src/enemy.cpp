#include <cmath>

#include "enemy.h"

void Enemy::update(float dt, Player *p, bool &running)
{
    float dx = p->x - x;
    float dy = p->y - y;
    float distance = sqrtf(dx * dx + dy * dy);

    float dz = p->s - PLAYER_SIZE;

    if (distance <= PLAYER_SIZE / 2.5 && dz == 0)
    {
        running = false;
        return;
    }

    vx = (dx / distance) * SPEED;
    vy = (dy / distance) * SPEED;

    x += vx * dt;
    y += vy * dt;

    rect = {(int)(x - (PLAYER_SIZE / 2)), (int)(y - (PLAYER_SIZE / 2)), PLAYER_SIZE, PLAYER_SIZE};
    center->x = (int)(PLAYER_SIZE / 2);
    center->y = (int)(PLAYER_SIZE / 2);

    animTime += dt;
    if (animTime >= 0.25 / 3)
    {
        animTime = 0;
        if (src.x == 32 * 5)
            src.x = 0;
        else
            src.x += 32;
    }
}

void Enemy::render(SDL_Renderer *renderer)
{
    SDL_RenderCopyEx(renderer, texture, &src, &rect, atan2f(vy, vx) * 180.0f / 3.14159265f + 90.0f, center, SDL_FLIP_NONE);
}