#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

class Player {
private:
    sf::RectangleShape car;
    float speed;
    float velocityX;
    float velocityY;
    std::string name;
    int playerID; // 1 para Player 1, 2 para Player 2

public:
    Player(int id, float x, float y, const std::string& playerName);
    
    void handleInput(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);
    
    sf::RectangleShape& getShape();
    float getSpeed() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
};

#endif // PLAYER_H
