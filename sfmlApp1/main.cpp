#include <iostream>
#include "Game.h"

int main()
{
    // Init srand
    std::srand(static_cast<unsigned int>(time(0)));

    // Init game engine
    Game game;
    game.setDifficulty("hard");

    // Game loop
    while (game.running() && !game.getEndGame()) {
        

        // Update
        game.update();

        // Render
        game.render();
    }
}