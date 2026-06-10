#include "Player.h"
#include <cmath>

const float PI = 3.14159265f;

Player::Player(int id, float x, float y, const std::string& playerName) 
    : playerID(id), name(playerName), speed(0.0f), angle(0.0f) {
    
    maxSpeed = 7.0f;
    acceleration = 0.2f;
    friction = 0.05f;
    turnSpeed = 3.5f;

    // CORRECCIÓN SFML 3: Usar llaves {ancho, alto}
    car.setSize({40.0f, 20.0f});
    
    // CORRECCIÓN SFML 3: Usar llaves para el origen y la posición
    car.setOrigin({20.0f, 10.0f}); 
    car.setPosition({x, y});

    if (playerID == 1) {
        car.setFillColor(sf::Color::Blue);
        // CORRECCIÓN SFML 3: Agregar "Key::" a las teclas
        keyUp = sf::Keyboard::Key::W;
        keyDown = sf::Keyboard::Key::S;
        keyLeft = sf::Keyboard::Key::A;
        keyRight = sf::Keyboard::Key::D;
    } else {
        car.setFillColor(sf::Color::Red);
        keyUp = sf::Keyboard::Key::Up;
        keyDown = sf::Keyboard::Key::Down;
        keyLeft = sf::Keyboard::Key::Left;
        keyRight = sf::Keyboard::Key::Right;
    }
}

void Player::update() {
    if (sf::Keyboard::isKeyPressed(keyUp)) {
        speed += acceleration;
        if (speed > maxSpeed) speed = maxSpeed;
    } 
    else if (sf::Keyboard::isKeyPressed(keyDown)) {
        speed -= acceleration;
        if (speed < -maxSpeed / 2.0f) speed = -maxSpeed / 2.0f;
    } 
    else {
        if (speed > 0) {
            speed -= friction;
            if (speed < 0) speed = 0;
        } else if (speed < 0) {
            speed += friction;
            if (speed > 0) speed = 0;
        }
    }

    if (speed != 0) {
        float turnMultiplier = (speed > 0) ? 1.0f : -1.0f; 
        
        if (sf::Keyboard::isKeyPressed(keyLeft)) {
            angle -= turnSpeed * turnMultiplier;
        }
        if (sf::Keyboard::isKeyPressed(keyRight)) {
            angle += turnSpeed * turnMultiplier;
        }
    }

    // CORRECCIÓN SFML 3: Convertir el flotante a sf::Angle usando sf::degrees()
    car.setRotation(sf::degrees(angle));

    float radians = angle * PI / 180.0f;
    float moveX = std::cos(radians) * speed;
    float moveY = std::sin(radians) * speed;

    // CORRECCIÓN SFML 3: Usar llaves para mover {x, y}
    car.move({moveX, moveY});
}

void Player::render(sf::RenderWindow& window) {
    window.draw(car);
}