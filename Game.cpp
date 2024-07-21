#include <SFML/Graphics.hpp>
#include "Windows.h"
#include "Game.h"
Game::Game(sf::RenderWindow &window) : win(window), is_enter_pressed(false),
                                       run_game(true),
                                       pipe_counter(71),
                                       pipe_spawn_time(70)
{
    win.setFramerateLimit(60);

    bg_texture.loadFromFile("assests/bg.png"); // loading file from hardrive
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
    bg_sprite.setPosition(0.f, -300.f);

    ground_texture.loadFromFile("assests/ground.png");
    ground_sprite1.setTexture(ground_texture);
    ground_sprite2.setTexture(ground_texture);

    ground_sprite1.setScale(SCALE_FACTOR, SCALE_FACTOR);
    ground_sprite2.setScale(SCALE_FACTOR, SCALE_FACTOR);

    ground_sprite1.setPosition(0.f, 575);
    ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width, 575);

    Pipe::loadTextures();
}
void Game::doProcessing(sf::Time &dt)
{
    if (is_enter_pressed)
    {
        moveGround(dt);

        if (pipe_counter > pipe_spawn_time)
        {
            pipes.push_back(Pipe(dist(rd)));
            pipe_counter = 0;
        }
        pipe_counter++;

        for (int i = 0; i < pipes.size(); i++)
        {
            pipes[i].update(dt);
            if (pipes[i].getRightBound() < 0)
            {
                pipes.erase(pipes.begin() + i);
            }
        }
        checkCollisions();
    }
    bird.update(dt);
}

void Game::startGameLoop()
{
    sf::Clock clock;

    while (win.isOpen()) // Game will run until the window is open.
    {
        sf::Time dt = clock.restart();
        sf::Event event;             // created a event class to handle the event.
        while (win.pollEvent(event)) // to watch the event happened
        {
            if (event.type == sf::Event::Closed)
            {
                win.close();
            }
            if (event.type == sf::Event::KeyPressed && run_game)
            {
                if (event.key.code == sf::Keyboard::Enter && !is_enter_pressed) // to watch the event happened
                {
                    is_enter_pressed = true;
                    bird.setShouldFly(true);
                }
                if (event.key.code == sf::Keyboard::Space && is_enter_pressed)
                {
                    bird.flapBird(dt);
                }
            }
        }
        moveGround(dt);
        bird.update(dt);
        doProcessing(dt);
        draw();
        win.display();
    }
}
void Game::checkCollisions()
{
    if (pipes.size() > 0)
    {
        if (pipes[0].sprite_down.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
            pipes[0].sprite_up.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) || // checks collision of bird with pipes
            bird.bird_sprite.getGlobalBounds().top >= 540)
        {
            is_enter_pressed = false;
            run_game = false;
        }
    }
}

void Game::draw()
{
    win.draw(bg_sprite);
    for (Pipe &pipe : pipes)
    {
        win.draw(pipe.sprite_down);
        win.draw(pipe.sprite_up);
    }
    win.draw(ground_sprite1);
    win.draw(ground_sprite2);
    win.draw(bird.bird_sprite);
}

void Game::moveGround(sf::Time &dt)
{
    ground_sprite1.move(-move_speed * dt.asSeconds(), 0.f);
    ground_sprite2.move(-move_speed * dt.asSeconds(), 0.f);

    if (ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width < 0)
    {
        ground_sprite1.setPosition(ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width, 575);
    }

    if (ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width < 0)
    {
        ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width, 575);
    }
}
