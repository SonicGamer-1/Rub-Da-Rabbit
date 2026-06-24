#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class UI
{
public:
    UI(SDL_Renderer *renderer);
    void update(const std::string &text, SDL_Color color, TTF_Font *font, SDL_Texture *&texture);
    void render(SDL_Texture *texture, int x, int y);
    void fpsCounter(int &frameCount, float &timeAccumulator, float deltaTime, TTF_Font *font, SDL_Texture *&texture);

private:
    SDL_Renderer *renderer;
};