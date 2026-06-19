#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "player.h"
#include "enemy.h"

class Game
{
    void enemyUpdate();
    SDL_Rect *carrot;

    bool running = true;
    bool win = false;
    float deltaTime = 0.0f;

    SDL_Texture *carrotTexture = nullptr;
    SDL_Texture *enemyTexture = nullptr;

    int score = 0;
    Enemy tempE;

public:
    void init();
    void loop();
    void quit();

    Player *p;
    std::vector<Enemy> *enemies;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};
