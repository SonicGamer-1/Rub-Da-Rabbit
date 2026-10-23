#pragma once
#include <SDL2/SDL.h>

class Player {
  SDL_Point center;
  SDL_Rect src = {0, 0, 32, 32};
  float animTime = 0;
  float facingVx = 0.0f;
  float facingVy = 1.0f;

public:
  float x, y;
  float vx, vy;
  float s;
  float vz, az;

  int stamina = 0;

  bool onGround = true;
  SDL_Rect rect;

  SDL_Texture *texture = nullptr;

  Player(float x_, float y_, float s_);

  void update(bool left, bool right, bool up, bool down, bool jump, float dt);
  void render(SDL_Renderer *renderer);
};