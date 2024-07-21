#include <SFML/Graphics.hpp>
#include "Bird.h"
#include <vector>
#include <random>
#include"Pipe.h"

class Game
{
public:
    Game(sf::RenderWindow &window); // It is a reference
    sf::RenderWindow &win;          // to store reference created a member variable win
    void startGameLoop();

private:
    sf::Texture bg_texture, ground_texture; // Not making constant only because we have to give here value
    sf::Sprite bg_sprite, ground_sprite1, ground_sprite2;
    Bird bird;
    bool is_enter_pressed, run_game;
    sf::Clock clock;
    const int move_speed = 270;
    void draw();
    void moveGround(sf::Time &);
    void doProcessing(sf::Time &dt);
    void checkCollisions();
    int pipe_counter, pipe_spawn_time;
    std::vector<Pipe> pipes;
    std::random_device rd; // for generating random numbers
    std::uniform_int_distribution<int> dist{250, 550};
};