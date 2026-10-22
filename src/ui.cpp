#include "ui.h"
#include "consts.h"

UI::UI(SDL_Renderer *renderer) : renderer(renderer) {}

void UI::update(const std::string &text, SDL_Color color, TTF_Font *font,
                SDL_Texture *&texture) {
  SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_DestroyTexture(texture);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
}

void UI::render(SDL_Texture *texture, int x, int y) {
  SDL_Rect dstRect = {x, y, 0, 0};
  SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
  SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void UI::fpsCounter(int &frameCount, float &timeAccumulator, float deltaTime,
                    TTF_Font *font, SDL_Texture *&texture) {
  frameCount++;
  timeAccumulator += deltaTime;

  if (timeAccumulator >= 1.0f) {
    std::string fpsText = "FPS: " + std::to_string(frameCount);
    update(fpsText, {0, 0, 0, 255}, font, texture);

    frameCount = 0;
    timeAccumulator -= 1.0f;
  }
}

void UI::staminaCounter(int stamina, SDL_Texture *&texture) {
  SDL_Rect dstRect = {WIDTH - 18, 10, 15, 30};

  for (int i = 0; i < stamina; ++i) {
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    dstRect.x -= 18; // Move left for the next stamina icon
  }
}