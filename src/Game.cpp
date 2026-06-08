#include "Game.h"
#include <iostream>
#include <vector>

void Game::loadBackgroundMusic() {
    backgroundMusic = std::make_shared<sf::Music>();
    
    std::vector<std::string> musicPaths = {
        "assets/music/Musica1.mp3",
        "assets/music/Musica2.mp3",
        "assets/music/background.ogg",
        "assets/music/background.wav",
        "assets/music/background.flac",
        "assets/music/musica.ogg",
        "assets/music/musica.wav",
        "assets/music/musica.flac"
    };
    
    for (const auto& path : musicPaths) {
        if (backgroundMusic->openFromFile(path)) {
            std::cout << "Música cargada desde: " << path << std::endl;
            backgroundMusic->setLooping(true);
            backgroundMusic->setVolume(70.0f);
            return;
        }
    }
    
    std::cerr << "Advertencia: No se pudo cargar el archivo de música" << std::endl;
}

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true), currentState(GameState::SPLASH), previousState(GameState::SPLASH) {
    window.setFramerateLimit(60);
    loadBackgroundMusic();
    optionsMenu.setBackgroundMusic(backgroundMusic);
}

Game::~Game() {
    if (backgroundMusic) {
        backgroundMusic->stop();
    }
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
        
        if (currentState == GameState::SPLASH) {
            handleSplashInput(*event);
        } else if (currentState == GameState::MENU) {
            handleMenuInput(*event);
        } else if (currentState == GameState::OPTIONS) {
            handleOptionsInput(*event);
        }
    }
}

void Game::handleSplashInput(const sf::Event& event) {
    if (event.getIf<sf::Event::KeyPressed>()) {
        currentState = GameState::MENU;
        if (backgroundMusic) {
            backgroundMusic->play();
        }
        std::cout << "Pasando al menú..." << std::endl;
    }
}

void Game::handleMenuInput(const sf::Event& event) {
    menu.handleInput(event);
    
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            int selected = menu.getSelectedOption();
            switch (selected) {
                case 0:
                    currentState = GameState::PLAYING;
                    std::cout << "Iniciando juego..." << std::endl;
                    break;
                case 1:
                    previousState = GameState::MENU;
                    currentState = GameState::OPTIONS;
                    optionsMenu.reset();
                    std::cout << "Abriendo opciones..." << std::endl;
                    break;
                case 2:
                    currentState = GameState::CREDITS;
                    std::cout << "Mostrando créditos..." << std::endl;
                    break;
                case 3:
                    window.close();
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }
        
        if (keyEvent->code == sf::Keyboard::Key::Escape && currentState != GameState::MENU) {
            currentState = GameState::MENU;
            menu.reset();
        }
    }
}

void Game::handleOptionsInput(const sf::Event& event) {
    optionsMenu.handleInput(event);
    
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            currentState = previousState;
            menu.reset();
            std::cout << "Volviendo al menú..." << std::endl;
        }
    }
}

void Game::update() {
    switch (currentState) {
        case GameState::SPLASH:
            splashScreen.update();
            break;
        case GameState::MENU:
            menu.update();
            break;
        case GameState::OPTIONS:
            optionsMenu.update();
            break;
        case GameState::PLAYING:
            break;
        case GameState::CREDITS:
            break;
        case GameState::QUIT:
            isRunning = false;
            break;
    }
}

void Game::render() {
    switch (currentState) {
        case GameState::SPLASH:
            splashScreen.render(window);
            break;
        case GameState::MENU:
            menu.render(window);
            break;
        case GameState::OPTIONS:
            optionsMenu.render(window);
            break;
        case GameState::PLAYING:
            window.clear(sf::Color::Green);
            window.display();
            break;
        case GameState::CREDITS:
            window.clear(sf::Color::Magenta);
            window.display();
            break;
        case GameState::QUIT:
            break;
    }
}
