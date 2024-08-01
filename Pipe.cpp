#include "Pipe.h"
#include "Windows.h"

Pipe::Pipe(int y_pos)
{
    sprite_down.setTexture(texture_down);
    sprite_up.setTexture(texture_up);

    sprite_down.setScale(SCALE_FACTOR, SCALE_FACTOR);
    sprite_up.setScale(SCALE_FACTOR, SCALE_FACTOR);

    sprite_up.setPosition(WIN_WIDTH, y_pos);
    sprite_down.setPosition(WIN_WIDTH, y_pos - pipe_distance - sprite_down.getGlobalBounds().height);
}
void Pipe::loadTextures()
{
    texture_down.loadFromFile("assets/pipedown.png");
    texture_up.loadFromFile("assets/pipe.png");
    
}
void Pipe::update(sf::Time &dt, int score)
{
    int current_move_speed = move_speed;
    if(score <3) {
        current_move_speed  = 400;
    }
    else if(score <7) {
        current_move_speed = 500;
    }
    else if(score <11) {
        current_move_speed = 800;
    }
    else {
        current_move_speed = 900;
    }
    sprite_up.move(-current_move_speed * dt.asSeconds(), 0.f);
    sprite_down.move(-current_move_speed * dt.asSeconds(), 0.f);
}

void Pipe::render(sf::RenderWindow& window) {
    window.draw(sprite_up);
    window.draw(sprite_down);
    
}
float Pipe::getRightBound()
{
    return sprite_down.getGlobalBounds().left + sprite_down.getGlobalBounds().width;
}
sf::Texture Pipe ::texture_down, Pipe::texture_up;
int Pipe::pipe_distance = 180, Pipe::move_speed = 400;
