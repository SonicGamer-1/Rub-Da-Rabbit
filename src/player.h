#pragma once
#include <SDL2/SDL.h>

class Player
{
    SDL_Point *center = new SDL_Point{(int)(s / 2), (int)(s / 2)};
    SDL_Rect frame1 = {0, 0, 32, 32};
    SDL_Rect frame2 = {32, 0, 32, 32};
    float animTime = 0;
    SDL_Rect src = frame1;

public:
    float x, y;
    float vx, vy;
    float s;
    float vz, az;
    bool onGround = true;
    SDL_Rect rect;

    SDL_Texture *texture = nullptr;

    Player(float x_, float y_, float s_);

    void update(bool left, bool right, bool up, bool down, bool jump, float dt);
    void render(SDL_Renderer *renderer);
};