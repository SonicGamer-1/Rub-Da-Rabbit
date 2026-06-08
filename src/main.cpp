#include <SDL2/SDL.h>
#include <vector>

#include "consts.h"
#include "player.h"
#include "game.h"
#include "enemy.h"

int main(int argc, char *argv[])
{
    Game game;

    Player player = {10.0f, 10.0f, PLAYER_SIZE};
    std::vector<Enemy> enemies;

    game.init(&player, &enemies);

    game.loop();

    game.quit();
    return 0;
}