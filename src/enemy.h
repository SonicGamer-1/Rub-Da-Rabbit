#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "consts.h"
#include "player.h"

class Enemy {
  SDL_Point *center = new SDL_Point{ENEMY_SIZE / 2, ENEMY_SIZE / 2};
  SDL_Rect src = {0, 0, 32, 32};
  float animTime = 0;
  int chaserChannel = -1;

public:
  float x, y;
  float vx, vy;
  SDL_Rect rect;

  SDL_Texture *texture = nullptr;

  void update(float dt, Player *p, bool &running, Mix_Chunk *chaserSound,
              bool playAudio);
  void render(SDL_Renderer *renderer);
};