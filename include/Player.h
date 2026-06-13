#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

class Player {
private:
    sf::Texture carTexture; 
    sf::Sprite car;
    std::string name;
    int playerID; // 1 para Player 1, 2 para Player 2

    float speed;
    float maxSpeed;
    float acceleration;
    float friction;
    float turnSpeed;
    float angle; 

    // En SFML 3 las teclas se guardan con sf::Keyboard::Key
    sf::Keyboard::Key keyUp;
    sf::Keyboard::Key keyDown;
    sf::Keyboard::Key keyLeft;
    sf::Keyboard::Key keyRight;

public:
    Player(int id, float x, float y, const std::string& playerName);
    
    void update();
    void render(sf::RenderWindow& window);
    
    sf::Sprite& getShape() { return car; }
    float getSpeed() const { return speed; }
    
    // CORRECCIÓN SFML 3: Usar llaves {x, y} para convertir a sf::Vector2f
    void setPosition(float x, float y) { car.setPosition({x, y}); }
    sf::Vector2f getPosition() const { return car.getPosition(); }
};

#endif // PLAYER_H