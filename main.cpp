#include <SFML/Graphics.hpp>
#include "Windows.h" 
#include "Game.h"

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "FLAPPY BIRD");
    Game game(window);
    game.startGameLoop();
    return 0;
}
