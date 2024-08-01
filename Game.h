#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bird.h"
#include <vector>
#include <random>
#include "Pipe.h"
#include <sstream>


class Game
{
public:
    Game(sf::RenderWindow &window); // It is a reference
    sf::RenderWindow &win;          // to store reference created a member variable win
    void startGameLoop();
 
private:
   
    sf::Texture bg_texture,bg_texturea, bg_textureb, ground_texture, start_texture, loading_texture, volume_on_texture, volume_off_texture; // Not making constant only because we have to give here value
    sf::Sprite bg_sprite, bg_spritea,bg_spriteb, ground_sprite1, ground_sprite2, start_sprite, loading_sprite, volume_on_sprite, volume_off_sprite ;
    Bird bird;
    bool is_enter_pressed, run_game, start_monitoring, is_mute_pressed;
    const int move_speed = 270;

    void draw();
    void moveGround(sf::Time &dt);
    void doProcessing(sf::Time &dt);
    void checkCollisions();
    void restartGame();
    void checkScore();
    std::string toString(int);

    int pipe_counter, pipe_spawn_time, score;
    std::vector<Pipe> pipes;
    std::random_device rd; // for generating random numbers
    std::uniform_int_distribution<int> dist{250, 550};
    sf::Font font;
    sf::Text start_text,restart_text, score_hud_text, scoreboard_text;
    sf::SoundBuffer score_buffer, dead_buffer;
    sf::Sound score_sound, dead_sound;
   
    int current_score =0;
};
#endif