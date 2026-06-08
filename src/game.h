#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "player.h"

class Enemy;

class Game
{
    void enemyUpdate();
    SDL_Rect *winPoint;

    bool running = true;
    bool win = false;
    float deltaTime = 0.0f;

public:
    void init(Player *p, std::vector<Enemy> *enemies);
    void loop();
    void quit();

    Player *p;
    std::vector<Enemy> *enemies;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};
