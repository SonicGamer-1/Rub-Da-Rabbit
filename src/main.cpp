#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <vector>

#include "consts.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "ui.h"

int main(int, char *[]) {

#ifdef _WIN32
  SetProcessDPIAware();
#endif

  Game game;

  game.init();

  game.loop();

  game.quit();
  return 0;
}