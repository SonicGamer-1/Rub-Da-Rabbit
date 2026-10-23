#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "consts.h"
#include "game.h"

void Game::saveHighScore() {
  std::ofstream file(SAVE_FILE, std::ios::binary);

  if (file) {
    file.write(reinterpret_cast<char *>(&highScore), sizeof(highScore));
  }
}
void Game::loadHighScore() {
  std::ifstream file(SAVE_FILE, std::ios::binary);

  if (file) {
    file.read(reinterpret_cast<char *>(&highScore), sizeof(highScore));
  } else {
    highScore = 0;
  }
}

void Game::init() {

  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

  Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, 2048);

  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  ui = UI(renderer);
  uiFont = TTF_OpenFont(FONT_PATH, 36);

  srand((unsigned int)time(nullptr));
  loadHighScore();

  this->enemies = new std::vector<Enemy>;
  this->p = new Player{10.0f, 10.0f, PLAYER_SIZE};

  carrot = new SDL_Rect{WIDTH - 30, HEIGHT - 50, CARROT_WIDTH, CARROT_HEIGHT};

  bgMusic = Mix_LoadMUS(BACKGROUND_MUSIC_PATH);
  Mix_VolumeMusic(BGM_VOLUME);
  Mix_PlayMusic(bgMusic, -1);

  jumpSound = Mix_LoadWAV(JUMP_SOUND_PATH);
  eatSound = Mix_LoadWAV(STAMINA_SOUND_PATH);
  runSound = Mix_LoadWAV(RUN_SOUND_PATH);
  gameOverSound = Mix_LoadWAV(GAME_OVER_SOUND_PATH);

  p->texture = IMG_LoadTexture(renderer, PLAYER_TEXTURE_PATH);
  carrotTexture = IMG_LoadTexture(renderer, CARROT_TEXTURE_PATH);
  enemyTexture = IMG_LoadTexture(renderer, ENEMY_TEXTURE_PATH);

  tempE.texture = enemyTexture;
  tempE.vx = 0.0f;
  tempE.vy = 0.0f;

  Mix_VolumeChunk(jumpSound, JUMP_SOUND_VOLUME);
  Mix_VolumeChunk(runSound, RUN_SOUND_VOLUME);
  Mix_VolumeChunk(eatSound, STAMINA_SOUND_VOLUME);
  Mix_VolumeChunk(gameOverSound, GAME_OVER_SOUND_VOLUME);
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
      float minDist = ENEMY_SIZE;

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

void Game::playerUpdate(const Uint8 *keys) {
  if (keys[SDL_SCANCODE_SPACE] && p->s == PLAYER_SIZE && p->stamina > 0)
    Mix_PlayChannel(-1, jumpSound, 0);

  if ((keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_W] ||
       keys[SDL_SCANCODE_S]) &&
      p->s == PLAYER_SIZE && !Mix_Playing(RUN_AUDIO_CHANNEL))
    Mix_PlayChannel(RUN_AUDIO_CHANNEL, runSound, 0);
  else
    Mix_HaltChannel(RUN_AUDIO_CHANNEL);

  p->update(keys[SDL_SCANCODE_A], keys[SDL_SCANCODE_D], keys[SDL_SCANCODE_W],
            keys[SDL_SCANCODE_S], keys[SDL_SCANCODE_SPACE], deltaTime);
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

    playerUpdate(keys);

    enemyUpdate();

    if (!running)
      Mix_PlayChannel(-1, gameOverSound, 0);

    if (SDL_HasIntersection(&p->rect, carrot) && p->s == PLAYER_SIZE) {
      score++;
      p->stamina += (p->stamina < MAX_STAMINA) ? STAMINA_RESTORE : 0;
      Mix_PlayChannel(-1, eatSound, 0);
      ui.update("Score: " + std::to_string(score), {0, 0, 0, 255}, uiFont,
                scoreTexture);

      carrot->x = rand() % (WIDTH - 20);
      carrot->y = rand() % (HEIGHT - 40);

      tempE.x = (float)(rand() % (WIDTH - ENEMY_SIZE));
      tempE.y = (float)(rand() % (HEIGHT - ENEMY_SIZE));
      if (abs(tempE.x - p->x) < ENEMY_MIN_DISTANCE * PLAYER_SIZE &&
          abs(tempE.y - p->y) < ENEMY_MIN_DISTANCE * PLAYER_SIZE) {
        tempE.x +=
            ((tempE.x - p->x) > 0 ? +1 : -1) * PLAYER_SIZE * ENEMY_MIN_DISTANCE;
        tempE.y +=
            ((tempE.y - p->y) > 0 ? +1 : -1) * PLAYER_SIZE * ENEMY_MIN_DISTANCE;
      }
      tempE.rect = {(int)(tempE.x - (ENEMY_SIZE / 2)),
                    (int)(tempE.y - (ENEMY_SIZE / 2)), ENEMY_SIZE, ENEMY_SIZE};
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
