#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <string>
#include <iostream>

enum GameState
{
    MENU,
    PLAYING
};

int main()
{
    GameState gamestate = MENU;
    int rightScore = 0;
    int leftScore = 0;
    sf::Font font;
    if (!font.openFromFile("/home/abdur/C++game/fonts/pixel_operator/PixelOperator.ttf"))
    {
        // Handle error - font file not found
        return -1; // or use a fallback font
    }

    //*Initializing all objects here scoreboard, paddles, ball and Start button
    sf::RectangleShape startOutline({150.f, 30.f});
    startOutline.setFillColor(sf::Color::Black);
    startOutline.setOutlineColor(sf::Color::White);
    startOutline.setOutlineThickness(2.f);
    startOutline.setPosition({320.f - startOutline.getSize().x / 2, 240.f - startOutline.getSize().y / 2});
    sf::Text scoreboard(font);
    sf::Text startText(font);
    startText.setString("START");
    sf::FloatRect textBounds = startText.getLocalBounds();
    startText.setPosition({startOutline.getPosition().x + (startOutline.getSize().x - textBounds.size.x)/2,(startOutline.getPosition().y - (startOutline.getSize().y - textBounds.size.y)/2) });
    startText.setFillColor(sf::Color::White);

    scoreboard.setString("0 | 0");
    scoreboard.setCharacterSize(24);
    scoreboard.setFillColor(sf::Color::White);
    scoreboard.setPosition({320.f, 20.f});

    sf::RectangleShape paddleLeft({10.f, 60.f});
    paddleLeft.setFillColor(sf::Color::White);
    paddleLeft.setPosition({20.f, 240.f});

    sf::RectangleShape paddleRight({10.f, 60.f});
    paddleRight.setFillColor(sf::Color::White);
    paddleRight.setPosition({610.f, 240.f});

    sf::RenderWindow window(sf::VideoMode({640, 480}), "Pong Game");
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition({320.f, 240.f});

    sf::Vector2f velocity(400.f, 0.f);
    sf::Clock clock;
    float paddleSpeed = 400.f;
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                //*Now checking if it is within the Start button
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (startOutline.getGlobalBounds().contains({mousePos.x, mousePos.y}))
                {
                    gamestate = PLAYING;
                }
            }
        }
        if (gamestate == MENU)
        {
            window.clear();
            window.draw(startOutline);
            window.draw(startText);
            window.display();
        }
        else
        {

            //^Controlling the rightPaddle and the leftPaddle
            sf::Vector2f pos = paddleRight.getPosition();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                pos.y -= paddleSpeed * deltaTime;
                if (pos.y >= 0.f)
                    paddleRight.setPosition(pos);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                pos.y += paddleSpeed * deltaTime;
                if (pos.y + paddleRight.getSize().y <= 480.f)
                    paddleRight.setPosition(pos);
            }
            sf::Vector2f posLeft = paddleLeft.getPosition();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                posLeft.y -= paddleSpeed * deltaTime;
                if (posLeft.y >= 0.f)
                    paddleLeft.setPosition(posLeft);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                posLeft.y += paddleSpeed * deltaTime;
                if (posLeft.y + paddleLeft.getSize().y <= 480.f)
                    paddleLeft.setPosition(posLeft);
            }
            sf::Vector2f position = ball.getPosition();

            //^Updating position of ball based on velocity and time typical distance formula
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;

            //^Bounce off top and bottom walls
            if (position.y <= 0 || position.y >= 460)
            {                             // 480 - 20 (ball diameter)
                velocity.y = -velocity.y; // Reverse vertical direction
            }

            if ((position.x + 20 >= paddleRight.getPosition().x) && (position.y + ball.getRadius() >= paddleRight.getPosition().y && position.y + ball.getRadius() <= paddleRight.getPosition().y + paddleRight.getSize().y))
            {
                velocity.x = -velocity.x; //*Reverse horizontal velocity if it hits right paddle
            }

            if ((position.x <= paddleLeft.getPosition().x + paddleLeft.getSize().x) && (position.y + ball.getRadius() >= paddleLeft.getPosition().y && position.y + ball.getRadius() <= paddleLeft.getPosition().y + paddleLeft.getSize().y))
            {
                velocity.x = -velocity.x; //*Reverse horizontal velocity if it hits right paddle
                float ballCenter = position.y + ball.getRadius();
                float paddleCenter = paddleRight.getPosition().y + paddleRight.getSize().y / 2;
                float hitOffset = ballCenter - paddleCenter;
                velocity.y = hitOffset * 2.f;
                if (velocity.y > 300.f)
                    velocity.y = 300.f;
                if (velocity.y < -300.f)
                    velocity.y = -300.f;
            }
            //^reset ball position if it goes beyond the left or right edges
            if (position.x > window.getSize().x || position.x < 0)
            {

                if (position.x < 0)
                {
                    rightScore++;
                }
                else if (position.x > window.getSize().x)
                {
                    leftScore++;
                }
                scoreboard.setString(std::to_string(leftScore) + " | " + std::to_string(rightScore));
                position = {320.f, 240.f};
                velocity.x *= -1;
                velocity.y = 0.f;
            }

            // Apply the new position
            ball.setPosition(position);

            window.clear();

            window.draw(ball);

            window.draw(paddleRight);

            window.draw(paddleLeft);

            window.draw(scoreboard);

            window.display();
        }
    }
    return 0;
}