#include <SDL2/SDL_image.h>

#include "consts.h"
#include "game.h"
#include <ctime>

void Game::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Rub da Rabbits", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    uiFont = TTF_OpenFont("assets/font.ttf", 36);

    srand((unsigned int)time(nullptr));

    this->enemies = new std::vector<Enemy>;
    this->p = new Player{10.0f, 10.0f, PLAYER_SIZE};
    carrot = new SDL_Rect{WIDTH - 30, HEIGHT - 50, 20, 40};

    p->texture = IMG_LoadTexture(renderer, "assets/player.png");
    carrotTexture = IMG_LoadTexture(renderer, "assets/carrot.png");
    enemyTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
    tempE.texture = enemyTexture;
}

void Game::enemyUpdate()
{
    for (Enemy &e : *enemies)
    {
        e.update(deltaTime, p, running);
    }
    for (int i = 0; i < enemies->size(); i++)
    {
        for (int j = i + 1; j < enemies->size(); j++)
        {
            float dx = (*enemies)[j].x - (*enemies)[i].x;
            float dy = (*enemies)[j].y - (*enemies)[i].y;

            float dist = sqrtf(dx * dx + dy * dy);
            float minDist = PLAYER_SIZE;

            if (dist > 0.0f && dist < minDist)
            {
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

void Game::textUpdate(const std::string &text, SDL_Color color, TTF_Font *font, SDL_Texture *&texture)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Game::textRender(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dstRect = {x, y, 0, 0};
    SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

static void fpsCounter(Uint64 &lastTime, float &deltaTime, int &frameCount, float &timeAccumulator)
{
    Uint64 currentTime = SDL_GetPerformanceCounter();
    deltaTime = (currentTime - lastTime) / (float)SDL_GetPerformanceFrequency();
    lastTime = currentTime;

    timeAccumulator += deltaTime;
    frameCount++;
    if (timeAccumulator >= 1.0f)
    {
        printf("FPS: %d\n", frameCount);
        timeAccumulator = 0.0f;
        frameCount = 0;
    }
}

void Game::loop()
{
    Uint64 lastTime = SDL_GetPerformanceCounter();

    int frameCount = 0;
    float timeAccumulator = 0.0f;

    textUpdate("Score: " + std::to_string(score), {0, 0, 0, 255}, uiFont, scoreTexture);

    while (running)
    {
        fpsCounter(lastTime, deltaTime, frameCount, timeAccumulator);

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);
        p->update(keys[SDL_SCANCODE_A], keys[SDL_SCANCODE_D], keys[SDL_SCANCODE_W], keys[SDL_SCANCODE_S], keys[SDL_SCANCODE_SPACE], deltaTime);
        enemyUpdate();

        if (SDL_HasIntersection(&p->rect, carrot) && p->s == PLAYER_SIZE)
        {
            score++;
            textUpdate("Score: " + std::to_string(score), {0, 0, 0, 255}, uiFont, scoreTexture);

            carrot->x = rand() % (WIDTH - 20);
            carrot->y = rand() % (HEIGHT - 40);

            tempE.x = (float)(rand() % (WIDTH - PLAYER_SIZE));
            tempE.y = (float)(rand() % (HEIGHT - PLAYER_SIZE));
            if (abs(tempE.x - p->x) < 4 * PLAYER_SIZE && abs(tempE.y - p->y) < 4 * PLAYER_SIZE)
            {
                tempE.x += ((tempE.x - p->x) > 0 ? +1 : -1) * PLAYER_SIZE * 3;
                tempE.y += ((tempE.y - p->y) > 0 ? +1 : -1) * PLAYER_SIZE * 3;
            }
            enemies->push_back(tempE);
        }

        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
        SDL_RenderClear(renderer);

        for (Enemy &e : *enemies)
        {
            e.render(renderer);
        }
        p->render(renderer);

        SDL_RenderCopy(renderer, carrotTexture, nullptr, carrot);

        textRender(scoreTexture, 10, 10);

        SDL_RenderPresent(renderer);
    };
}

void Game::quit()
{
    SDL_DestroyTexture(carrotTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(p->texture);
    SDL_DestroyTexture(scoreTexture);
    TTF_CloseFont(uiFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
