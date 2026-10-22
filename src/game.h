#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "enemy.h"
#include "player.h"
#include "ui.h"

class Game {
  void enemyUpdate();
  void saveHighScore();
  void loadHighScore();

  SDL_Rect *carrot;

  bool running = true;
  bool win = false;
  float deltaTime = 0.0f;

  SDL_Texture *carrotTexture = nullptr;
  SDL_Texture *enemyTexture = nullptr;

  Mix_Chunk *jumpSound = nullptr;
  Mix_Chunk *eatSound = nullptr;
  Mix_Chunk *gameOverSound = nullptr;

  Mix_Music *bgMusic = nullptr;

  int score = 0;
  int highScore;

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
