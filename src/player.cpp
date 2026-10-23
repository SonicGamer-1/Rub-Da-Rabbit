#include <algorithm>
#include <cmath>

#include "consts.h"
#include "player.h"

Player::Player(float x_, float y_, float s_) : x(x_), y(y_), s(s_) {
  center = {(int)(s / 2), (int)(s / 2)};
  rect = {(int)x, (int)y, (int)s, (int)s};
  vx = 0, vy = 0;
  vz = 0, az = 0;
}

void Player::update(bool left, bool right, bool up, bool down, bool jump,
                    float dt) {
  if (jump && onGround && stamina > 0) {
    vz = PLAYER_SPEED * 2;
    az = -PLAYER_SPEED * 4; // makes time of flight 1 second
    onGround = false;
    src.x = 0;
    stamina--;
  }
  if (!onGround && vz < 0 && s <= PLAYER_SIZE) {
    s = PLAYER_SIZE;
    vz = 0;
    az = 0;
    onGround = true;
  }

  if ((left + right) && (up + down)) {
    vx =
        (right - left) * PLAYER_SPEED * 0.70710678f * (!onGround ? 1.5f : 1.0f);
    vy = -(up - down) * PLAYER_SPEED * 0.70710678f * (!onGround ? 1.5f : 1.0f);
  } else {
    vx = (right - left) * PLAYER_SPEED * (!onGround ? 1.5f : 1.0f);
    vy = -(up - down) * PLAYER_SPEED * (!onGround ? 1.5f : 1.0f);
  }

  x += vx * dt;
  y += vy * dt;

  vz += az * dt;
  s += vz * dt;

  x = std::clamp(x, 0.0f + (s / 2), WIDTH - (s / 2));
  y = std::clamp(y, 0.0f + (s / 2), HEIGHT - (s / 2));

  rect = {(int)(x - (s / 2)), (int)(y - (s / 2)), (int)s, (int)s};
  center.x = (int)(s / 2);
  center.y = (int)(s / 2);

  animTime += dt;
  if (animTime >= 0.25 / PLAYER_SPEED * 200 && (vx != 0 || vy != 0) &&
      onGround) {
    animTime = 0;
    if (src.x == 32 * 5)
      src.x = 0;
    else
      src.x += 32;
  }
}

void Player::render(SDL_Renderer *renderer) {
  SDL_RenderCopyEx(renderer, texture, &src, &rect,
                   atan2f(vy, vx) * 180.0f / 3.14159265f + 90.0f, &center,
                   SDL_FLIP_NONE);
}