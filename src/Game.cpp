#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true), currentState(GameState::MENU) {
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
        
        if (currentState == GameState::MENU) {
            handleMenuInput(*event);
        }
    }
}

void Game::handleMenuInput(const sf::Event& event) {
    menu.handleInput(event);
    
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            int selected = menu.getSelectedOption();
            switch (selected) {
                case 0: // Iniciar juego
                    currentState = GameState::PLAYING;
                    std::cout << "Iniciando juego..." << std::endl;
                    break;
                case 1: // Opciones
                    currentState = GameState::OPTIONS;
                    std::cout << "Abriendo opciones..." << std::endl;
                    break;
                case 2: // Créditos
                    currentState = GameState::CREDITS;
                    std::cout << "Mostrando créditos..." << std::endl;
                    break;
                case 3: // Salir
                    window.close();
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }
        
        // ESC para volver al menú desde otros estados
        if (keyEvent->code == sf::Keyboard::Key::Escape && currentState != GameState::MENU) {
            currentState = GameState::MENU;
            menu.reset();
        }
    }
}

void Game::update() {
    switch (currentState) {
        case GameState::MENU:
            menu.update();
            break;
        case GameState::PLAYING:
            // Lógica del juego aquí
            break;
        case GameState::OPTIONS:
            // Lógica de opciones aquí
            break;
        case GameState::CREDITS:
            // Lógica de créditos aquí
            break;
        case GameState::QUIT:
            isRunning = false;
            break;
    }
}

void Game::render() {
    switch (currentState) {
        case GameState::MENU:
            menu.render(window);
            break;
        case GameState::PLAYING:
            window.clear(sf::Color::Green);
            // Aquí se renderizarán los objetos del juego
            window.display();
            break;
        case GameState::OPTIONS:
            window.clear(sf::Color::Blue);
            // Render de opciones
            window.display();
            break;
        case GameState::CREDITS:
            window.clear(sf::Color::Magenta);
            // Render de créditos
            window.display();
            break;
        case GameState::QUIT:
            break;
    }
}
