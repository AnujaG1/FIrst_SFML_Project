#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Windows.h"
#include "Game.h"

Game::Game(sf::RenderWindow &window) : win(window), is_enter_pressed(false), is_mute_pressed(false),
                                       run_game(true),
                                       pipe_counter(71),
                                       pipe_spawn_time(70),
                                       score(0)
{
    win.setFramerateLimit(60);
    if (!bg_texture.loadFromFile("assets/bg.png"))
    {
        std::cerr << "Error: Could not load bg.png" << std::endl;
    }
    if (!bg_textureb.loadFromFile("assets/bg_night.jpg"))
    {
        std::cerr << "Error: Could not load bg_night.png" << std::endl;
    }
    if (!bg_texturea.loadFromFile("assets/home.jpg"))
    {
        std::cerr << "Error: Could not load home.png" << std::endl;
    }
    bg_sprite.setTexture(bg_texture);
    bg_spritea.setTexture(bg_texturea);
    bg_spriteb.setTexture(bg_textureb);
    bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
    bg_spritea.setScale(SCALE_FACTOR, SCALE_FACTOR);
    bg_spriteb.setScale(SCALE_FACTOR, SCALE_FACTOR);

    bg_sprite.setPosition(0.f, 0.f);
    bg_spritea.setPosition(bg_sprite.getGlobalBounds().width, 0.f);
    bg_spriteb.setPosition(bg_spritea.getGlobalBounds().width, 0.f);

    if (!volume_on_texture.loadFromFile("assets/volume_on.png"))
    {
        std::cerr << "Error: Could not load volume_on.png" << std::endl;
    }
    volume_on_sprite.setTexture(volume_on_texture);
    volume_on_sprite.setScale(0.1f, 0.1f);
    volume_on_sprite.setPosition(520, 8);

    volume_off_texture.loadFromFile("assets/volume_off.png");
    volume_off_sprite.setTexture(volume_off_texture);
    volume_off_sprite.setScale(0.1f, 0.1f);
    volume_off_sprite.setPosition(520, 8);

    ground_texture.loadFromFile("assets/ground.png");
    ground_sprite1.setTexture(ground_texture);
    ground_sprite2.setTexture(ground_texture);
    ground_sprite1.setScale(SCALE_FACTOR, SCALE_FACTOR);
    ground_sprite2.setScale(SCALE_FACTOR, SCALE_FACTOR);
    ground_sprite1.setPosition(0.f, 575);
    ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width, 575);

    font.loadFromFile("assets/pixel.ttf");
    start_text.setFont(font);
    start_text.setCharacterSize(30);
    start_text.setFillColor(sf::Color::White);
    start_text.setPosition(180, 280);
    start_text.setString("Start Game");

    restart_text.setFont(font);
    restart_text.setCharacterSize(30);
    restart_text.setFillColor(sf::Color::White);
    restart_text.setPosition(200, 200);
    restart_text.setString("Play Again");

    scoreboard_text.setFont(font);
    scoreboard_text.setCharacterSize(30);
    scoreboard_text.setFillColor(sf::Color::White);
    scoreboard_text.setPosition(220, 240);
    scoreboard_text.setString("Score:");

    score_hud_text.setFont(font);
    score_hud_text.setCharacterSize(20);
    score_hud_text.setFillColor(sf::Color::White);
    score_hud_text.setPosition(15, 15);
    score_hud_text.setString("Score: 0");

    exit_text.setFont(font);
    exit_text.setCharacterSize(25);
    exit_text.setFillColor(sf::Color::White);
    exit_text.setPosition(500, 700);
    exit_text.setString("Exit");

    score_buffer.loadFromFile("assets/sfx/flap.wav");
    score_sound.setBuffer(score_buffer);

    dead_buffer.loadFromFile("assets/sfx/dead.wav");
    dead_sound.setBuffer(dead_buffer);

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

        for (auto it = pipes.begin(); it != pipes.end();)
        {
            it->update(dt, score);
            if (it->getRightBound() < 0)
            {
                it = pipes.erase(it);
            }
            else
            {
                ++it;
            }
        }
        checkCollisions();
        checkScore();
    }
    bird.update(dt);
}

void Game::startGameLoop()
{
    sf::Clock clock;

    while (win.isOpen())
    {
        sf::Time dt = clock.restart();
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                win.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (exit_text.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    win.close();
                }
            }
            if (event.type == sf::Event::KeyPressed && run_game)
            {
                if (event.key.code == sf::Keyboard::Enter && !is_enter_pressed)
                {
                    is_enter_pressed = true;
                    bird.setShouldFly(true);
                }
                else if (event.key.code == sf::Keyboard::Space && is_enter_pressed)
                {
                    bird.flapBird(dt);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (!run_game)
                {
                    if (restart_text.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        restartGame();
                    }
                }
                else if (!is_enter_pressed)
                {
                    if (start_text.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        is_enter_pressed = true;
                        bird.setShouldFly(true);
                        start_text.setString("");
                    }
                }
                if (volume_on_sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !is_mute_pressed)
                {
                    is_mute_pressed = true;
                }
                else if (volume_off_sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && is_mute_pressed)
                {
                    is_mute_pressed = false;
                }
            }
        }

        if (is_enter_pressed)
        {
            bird.update(dt);
            doProcessing(dt);
        }
        draw();
        win.display();
    }
}

void Game::checkCollisions()
{
    if (!pipes.empty())
    {
        if (pipes[0].sprite_down.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
            pipes[0].sprite_up.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
            bird.bird_sprite.getGlobalBounds().top >= 540)
        {
            if (!is_mute_pressed)
            {
                dead_sound.play();
            }
            is_enter_pressed = false;
            run_game = false;
        }
    }
}

void Game::checkScore()
{
    if (!pipes.empty())
    {
        if (!start_monitoring)
        {
            if (bird.bird_sprite.getGlobalBounds().left > pipes[0].sprite_down.getGlobalBounds().left &&
                bird.getRightBound() < pipes[0].getRightBound())
            {
                start_monitoring = true;
            }
        }
        else
        {
            if (bird.bird_sprite.getGlobalBounds().left > pipes[0].getRightBound())
            {
                score++;
                score_hud_text.setString("Score: " + toString(score));
                if (!is_mute_pressed)
                {
                    score_sound.play();
                }
                start_monitoring = false;
            }
        }
    }
}

void Game::draw()
{
    win.draw(exit_text);
    if(score >=2 && score <=4)
    {
        bg_sprite.setTexture(bg_texturea);
        win.draw(bg_sprite);
    } 
    else if(score >4)
    {
        bg_sprite.setTexture(bg_textureb);
        win.draw(bg_sprite);
        
    }
    else {
        bg_sprite.setTexture(bg_texture);
        win.draw(bg_sprite);
    }
    for (Pipe &pipe : pipes)
    {
        win.draw(pipe.sprite_down);
        win.draw(pipe.sprite_up);
    }
    if (is_mute_pressed)
    {
        win.draw(volume_off_sprite);
    }
    else
    {
        win.draw(volume_on_sprite);
    }
    win.draw(start_text);
    win.draw(ground_sprite1);
    win.draw(ground_sprite2);
    win.draw(bird.bird_sprite);
    win.draw(score_hud_text);

    if (!run_game)
    {
        win.draw(restart_text);
        scoreboard_text.setString("Score: " + toString(score));
        win.draw(scoreboard_text);
    }
    if (!is_enter_pressed && !run_game)
    {
        win.draw(start_text);
    }
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

void Game::restartGame()
{
    bird.resetBirdPosition();
    bird.setShouldFly(true);
    run_game = true;
    is_enter_pressed = true;
    pipe_counter = 71;
    pipes.clear();
    score = 0;
    score_hud_text.setString("Score: 0");
}

std::string Game::toString(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}