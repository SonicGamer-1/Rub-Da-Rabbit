#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "consts.h"
#include "game.h"

void Game::saveHighScore() {
  std::ofstream file("save.dat", std::ios::binary);

  if (file) {
    file.write(reinterpret_cast<char *>(&highScore), sizeof(highScore));
  }
}
void Game::loadHighScore() {
  std::ifstream file("save.dat", std::ios::binary);

  if (file) {
    file.read(reinterpret_cast<char *>(&highScore), sizeof(highScore));
  } else {
    highScore = 0;
  }
}

void Game::init() {

  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow("Rub da Rabbits", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
  Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  ui = UI(renderer);
  uiFont = TTF_OpenFont("assets/font.ttf", 36);

  srand((unsigned int)time(nullptr));
  loadHighScore();

  this->enemies = new std::vector<Enemy>;
  this->p = new Player{10.0f, 10.0f, PLAYER_SIZE};
  carrot = new SDL_Rect{WIDTH - 30, HEIGHT - 50, 20, 40};

  bgMusic = Mix_LoadMUS("assets/audio/bg_music.mp3");
  Mix_PlayMusic(bgMusic, -1);

  jumpSound = Mix_LoadWAV("assets/audio/jump.wav");
  staminaSound = Mix_LoadWAV("assets/audio/stamina.wav");
  gameOverSound = Mix_LoadWAV("assets/audio/game_over.wav");
  chaseSound = Mix_LoadWAV("assets/audio/chase.wav");

  p->texture = IMG_LoadTexture(renderer, "assets/image/player.png");
  carrotTexture = IMG_LoadTexture(renderer, "assets/image/carrot.png");
  enemyTexture = IMG_LoadTexture(renderer, "assets/image/enemy.png");
  tempE.texture = enemyTexture;
}

void Game::enemyUpdate() {
  for (Enemy &e : *enemies) {
    e.update(deltaTime, p, running);
  }
  for (std::size_t i = 0; i < enemies->size(); i++) {
    for (std::size_t j = i + 1; j < enemies->size(); j++) {
      float dx = (*enemies)[j].x - (*enemies)[i].x;
      float dy = (*enemies)[j].y - (*enemies)[i].y;

      float dist = sqrtf(dx * dx + dy * dy);
      float minDist = PLAYER_SIZE;

      if (dist > 0.0f && dist < minDist) {
        float overlap = minDist - dist;

        float nx = dx / dist;
        float ny = dy / dist;

        (*enemies)[i].x -= nx * overlap * 0.5f;
        (*enemies)[i].y -= ny * overlap * 0.5f;

        (*enemies)[j].x += nx * overlap * 0.5f;
        (*enemies)[j].y += ny * overlap * 0.5f;
      }
    }
  }
}

void Game::loop() {
  Uint64 lastTime = SDL_GetPerformanceCounter();

  int frameCount = 0;
  float timeAccumulator = 0.0f;

  ui.update("Score: " + std::to_string(score), {0, 0, 0, 255}, uiFont,
            scoreTexture);
  ui.update("Game Over! Press Enter to Restart", {0, 0, 0, 255}, uiFont,
            gameOverTexture);

  while (running) {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    deltaTime = (float)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
    lastTime = currentTime;

    ui.fpsCounter(frameCount, timeAccumulator, deltaTime, uiFont, fpsTexture);

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        return;
    }

    const Uint8 *keys = SDL_GetKeyboardState(nullptr);
    p->update(keys[SDL_SCANCODE_A], keys[SDL_SCANCODE_D], keys[SDL_SCANCODE_W],
              keys[SDL_SCANCODE_S], keys[SDL_SCANCODE_SPACE], deltaTime);
    enemyUpdate();

    if (SDL_HasIntersection(&p->rect, carrot) && p->s == PLAYER_SIZE) {
      score++;
      p->stamina += (p->stamina < 10) ? 2 : 0;
      Mix_PlayChannel(-1, staminaSound, 0);
      ui.update("Score: " + std::to_string(score), {0, 0, 0, 255}, uiFont,
                scoreTexture);

      carrot->x = rand() % (WIDTH - 20);
      carrot->y = rand() % (HEIGHT - 40);

      tempE.x = (float)(rand() % (WIDTH - PLAYER_SIZE));
      tempE.y = (float)(rand() % (HEIGHT - PLAYER_SIZE));
      if (abs(tempE.x - p->x) < 4 * PLAYER_SIZE &&
          abs(tempE.y - p->y) < 4 * PLAYER_SIZE) {
        tempE.x += ((tempE.x - p->x) > 0 ? +1 : -1) * PLAYER_SIZE * 3;
        tempE.y += ((tempE.y - p->y) > 0 ? +1 : -1) * PLAYER_SIZE * 3;
      }
      enemies->push_back(tempE);
    }

    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, carrotTexture, nullptr, carrot);

    for (Enemy &e : *enemies)
      e.render(renderer);
    p->render(renderer);

    ui.render(scoreTexture, 10, 10);
    ui.render(fpsTexture, 15, HEIGHT - 50);
    ui.staminaCounter(p->stamina, carrotTexture);

    SDL_RenderPresent(renderer);

    if (score > highScore) {
      saveHighScore();
      highScore = score;
    }

    while (!running) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
          return;

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
          SDL_Delay(500);
          running = 1, score = 0, p->stamina = 0;
          enemies->clear();
          ui.update("Score: " + std::to_string(score), {0, 0, 0, 255}, uiFont,
                    scoreTexture);
        }
      }

      SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
      SDL_RenderClear(renderer);

      int w;
      int h;
      SDL_QueryTexture(gameOverTexture, nullptr, nullptr, &w, &h);
      ui.render(gameOverTexture, WIDTH / 2 - w / 2, HEIGHT / 2 - h / 2);
      ui.render(scoreTexture, 10, 10);

      SDL_RenderPresent(renderer);
    }
  };
}

void Game::quit() {
  SDL_DestroyTexture(carrotTexture);
  SDL_DestroyTexture(enemyTexture);
  SDL_DestroyTexture(p->texture);
  SDL_DestroyTexture(scoreTexture);
  SDL_DestroyTexture(fpsTexture);
  SDL_DestroyTexture(gameOverTexture);
  TTF_CloseFont(uiFont);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
