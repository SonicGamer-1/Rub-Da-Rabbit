#include <cmath>

#include "enemy.h"

void Enemy::update(float dt, Player *p, bool &running) {
  float dx = p->x - x;
  float dy = p->y - y;
  float distance = sqrtf(dx * dx + dy * dy);

  float dz = p->s - PLAYER_SIZE;

  if (distance <= (PLAYER_SIZE + ENEMY_SIZE) / 5 && dz == 0) {
    running = false;
    return;
  }

  vx = (dx / distance) * ENEMY_SPEED;
  vy = (dy / distance) * ENEMY_SPEED;

  x += vx * dt;
  y += vy * dt;

  rect = {(int)(x - (ENEMY_SIZE / 2)), (int)(y - (ENEMY_SIZE / 2)), ENEMY_SIZE,
          ENEMY_SIZE};
  center->x = (int)(ENEMY_SIZE / 2);
  center->y = (int)(ENEMY_SIZE / 2);

  animTime += dt;
  if (animTime >= 0.25 / ENEMY_SPEED * 200) {
    animTime = 0;
    if (src.x == 32 * 5)
      src.x = 0;
    else
      src.x += 32;
  }
}

void Enemy::render(SDL_Renderer *renderer) {
  SDL_RenderCopyEx(renderer, texture, &src, &rect,
                   atan2f(vy, vx) * 180.0f / 3.14159265f + 90.0f, center,
                   SDL_FLIP_NONE);
}