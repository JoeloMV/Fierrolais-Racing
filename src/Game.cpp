#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true) {
    window.setFramerateLimit(60);
}

Game::~Game() {
    window.close();
}

void Game::run() {
    while (window.isOpen() && isRunning) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            isRunning = false;
        }
    }
}

void Game::update() {
    // Lógica del juego aquí
}

void Game::render() {
    window.clear(sf::Color::Green);
    
    // Aquí se renderizarán los objetos del juego
    
    window.display();
}
