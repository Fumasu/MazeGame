#include <cstdlib>

#include "Game.hpp"

int main(int argc, char **argv)
{
    Game game(1024, 768, "MazeGame");
    
    if (!game.Init())
        return EXIT_FAILURE;
        
	return game.Run();
}
