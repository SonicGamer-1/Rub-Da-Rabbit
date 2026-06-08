#pragma once

#include <SDL2/SDL.h>

#include "player.h"
#include "consts.h"

class Enemy
{
    SDL_Point *center = new SDL_Point{(int)(PLAYER_SIZE / 2), (int)(PLAYER_SIZE / 2)};
    SDL_Rect frame1 = {0, 0, 32, 32};
    SDL_Rect frame2 = {32, 0, 32, 32};
    float animTime = 0;
    SDL_Rect src = frame1;

public:
    float x, y;
    float vx, vy;
    SDL_Rect rect;

    SDL_Texture *texture = nullptr;

    void update(float dt, Player *p, bool &running);
    void render(SDL_Renderer *renderer);
};