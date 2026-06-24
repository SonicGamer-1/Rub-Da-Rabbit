#pragma once

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "player.h"
#include "enemy.h"
#include "ui.h"

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

    TTF_Font *uiFont = nullptr;
    SDL_Texture *scoreTexture = nullptr;
    SDL_Texture *fpsTexture = nullptr;
    SDL_Texture *gameOverTexture = nullptr;
    UI ui = nullptr;

public:
    void init();
    void loop();
    void quit();

    Player *p;
    std::vector<Enemy> *enemies;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};
