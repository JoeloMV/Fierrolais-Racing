#include "Player.h"

Player::Player(int id, float x, float y, const std::string& playerName)
    : playerID(id), name(playerName), speed(0.0f), velocityX(0.0f), velocityY(0.0f)
{
    // Como 'car' es un rectángulo, le damos tamaño y color
    car.setSize(sf::Vector2f(50.0f, 100.0f)); // Ancho 50, Alto 100
    car.setFillColor(sf::Color::Blue);        // Carro de color azul
    car.setPosition(sf::Vector2f(x, y));      // Posición inicial
}

void Player::handleInput(const sf::Event& event) {
    // Espacio para los controles futuros
}

void Player::update() {
    // Espacio para las físicas futuras
}

void Player::render(sf::RenderWindow& window) {
    // Dibujamos el rectángulo directamente (sin asteriscos)
    window.draw(car);
}