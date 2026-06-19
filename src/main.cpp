#include <SDL2/SDL.h>
#include <vector>

#include "consts.h"
#include "player.h"
#include "game.h"
#include "enemy.h"

int main(int argc, char *argv[])
{
    Game game;

    game.init();

    game.loop();

    game.quit();
    return 0;
}