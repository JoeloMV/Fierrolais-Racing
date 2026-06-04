#include "Player.h"

Player::Player(int id, float x, float y, const std::string& playerName)
    : playerID(id), name(playerName), speed(0.0f), velocityX(0.0f), velocityY(0.0f) {
    
    car.setSize(sf::Vector2f(40, 60));
    car.setPosition(sf::Vector2f(x, y));
    
    // Colores diferentes para cada jugador
    if (playerID == 1) {
        car.setFillColor(sf::Color::Red);
    } else {
        car.setFillColor(sf::Color::Blue);
    }
}

void Player::handleInput(const sf::Event& event) {
    // Será implementado en la próxima fase
}

void Player::update() {
    // Será implementado en la próxima fase
}

void Player::render(sf::RenderWindow& window) {
    window.draw(car);
}

sf::RectangleShape& Player::getShape() {
    return car;
}

float Player::getSpeed() const {
    return speed;
}

void Player::setPosition(float x, float y) {
    car.setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Player::getPosition() const {
    return car.getPosition();
}
