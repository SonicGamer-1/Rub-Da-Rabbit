#pragma once

#include <SDL2/SDL.h>

#include "player.h"
#include "consts.h"

class Enemy
{
    SDL_Point *center = new SDL_Point{PLAYER_SIZE / 2, PLAYER_SIZE / 2};
    SDL_Rect src = {0, 0, 32, 32};
    float animTime = 0;

public:
    float x, y;
    float vx, vy;
    SDL_Rect rect;

    SDL_Texture *texture = nullptr;

    void update(float dt, Player *p, bool &running);
    void render(SDL_Renderer *renderer);
};