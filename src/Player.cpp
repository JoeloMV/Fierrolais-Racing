#include "Player.h"
#include <cmath>
#include <iostream>

const float PI = 3.14159265f;

// CORRECCIÓN PARA SFML 3: car(carTexture) inicializa el Sprite enlazado correctamente
Player::Player(int id, float x, float y, const std::string& playerName)
    : playerID(id), name(playerName), speed(0.0f), angle(0.0f), car(carTexture) {

    maxSpeed = 7.0f;
    acceleration = 0.2f;
    friction = 0.05f;
    turnSpeed = 3.5f;

    // --- CARGA DINÁMICA DE SPRITES ---
    // Si es el Jugador 1 carga carro1, de lo contrario carga carro2
    std::string archivoCarro = (playerID == 1) ? "assets/carro1.png" : "assets/carro2.png";

    if (!carTexture.loadFromFile(archivoCarro)) {
        std::cerr << "Error al cargar " << archivoCarro << std::endl;
    }
    car.setTexture(carTexture);

    // Centramos el eje del carro usando sus límites reales
    sf::FloatRect bounds = car.getLocalBounds();
    car.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    
    // Escala pequeña para que quepa bien en la pista
    car.setScale({0.14f, 0.14f});
    
    // Colocamos el carro en su posición inicial
    car.setPosition({x, y});

    // Configuración de controles independientes para cada jugador
    if (playerID == 1) {
        keyUp    = sf::Keyboard::Key::W;
        keyDown  = sf::Keyboard::Key::S;
        keyLeft  = sf::Keyboard::Key::A;
        keyRight = sf::Keyboard::Key::D;
    } else {
        keyUp    = sf::Keyboard::Key::Up;
        keyDown  = sf::Keyboard::Key::Down;
        keyLeft  = sf::Keyboard::Key::Left;
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

    // Ajuste de rotación para SFML 3 usando sf::degrees
    car.setRotation(sf::degrees(angle));

    float radians = angle * PI / 180.0f;
    car.move({speed * std::sin(radians), -speed * std::cos(radians)});
}

void Player::render(sf::RenderWindow& window) {
    window.draw(car);
}