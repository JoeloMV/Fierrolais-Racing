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

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true), currentState(GameState::SPLASH), previousState(GameState::SPLASH), pistaSprite(pistaTexture), carroSprite(carroTexture) {
    window.setFramerateLimit(60);
    loadBackgroundMusic();
    optionsMenu.setBackgroundMusic(backgroundMusic);
   // Cargamos la imagen de la pista
    if (!pistaTexture.loadFromFile("assets/pista.png")) {
        std::cerr << "Error al cargar pista.png" << std::endl;
    }

    // Le decimos al sprite el tamaño de la imagen (626 x 417)
    pistaSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(626, 417)));

    // Ajustamos la escala al formato de SFML 3 usando sf::Vector2f
    pistaSprite.setScale(sf::Vector2f(1200.0f / 626.0f, 700.0f / 417.0f));

// --- Cargar el Carro ---
    if (!carroTexture.loadFromFile("assets/images/fierrarif8.png")) {
        std::cerr << "Error al cargar el carro" << std::endl;
    }
    carroSprite = sf::Sprite(carroTexture);
    
    carroSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(100, 150)));
    
   // Lo hacemos pequeño para que quepa en la pista
    carroSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 

    // Lo regresamos a la línea de meta
    carroSprite.setPosition(sf::Vector2f(600.0f, 500.0f));
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
        } else if (currentState == GameState::NAME_INPUT_P1 || currentState == GameState::NAME_INPUT_P2) {
            handleNameInputInput(*event);
        } else if (currentState == GameState::CHARACTER_SELECTION) {
            handleCharacterSelectionInput(*event);
        } else if (currentState == GameState::CAR_SELECTION) {
            // Pasamos los controles a la selección de carros
            carSelectionScreen.handleInput(*event);
            if (carSelectionScreen.areBothConfirmed()) {
                currentState = GameState::PLAYING;
                std::cout << "Iniciando juego..." << std::endl;
            }
        } else if (currentState == GameState::OPTIONS) {
            handleOptionsInput(*event);
        } else if (currentState == GameState::CREDITS) {
            handleCreditsInput(*event);
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
                    currentState = GameState::NAME_INPUT_P1;
                    nameInputScreen.reset(1);
                    std::cout << "Ingresando nombre Jugador 1..." << std::endl;
                    break;
                case 1:
                    previousState = GameState::MENU;
                    currentState = GameState::OPTIONS;
                    optionsMenu.reset();
                    std::cout << "Abriendo opciones..." << std::endl;
                    break;
                case 2:
                    previousState = GameState::MENU;
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

void Game::handleNameInputInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            if (nameInputScreen.isNameConfirmed()) {
                if (currentState == GameState::NAME_INPUT_P1) {
                    player1Name = nameInputScreen.getPlayerName();
                    currentState = GameState::NAME_INPUT_P2;
                    nameInputScreen.reset(2);
                    std::cout << "Jugador 1: " << player1Name << std::endl;
                    std::cout << "Ingresando nombre Jugador 2..." << std::endl;
                } else if (currentState == GameState::NAME_INPUT_P2) {
                    player2Name = nameInputScreen.getPlayerName();
                    currentState = GameState::CHARACTER_SELECTION;
                    characterSelectionScreen.setPlayerNames(player1Name, player2Name);
                    characterSelectionScreen.reset();
                    std::cout << "Jugador 2: " << player2Name << std::endl;
                    std::cout << "Seleccionando personajes..." << std::endl;
                }
            }
        }
    }
    
    // Pasar eventos al input screen
    nameInputScreen.handleInput(event);
}

void Game::handleCharacterSelectionInput(const sf::Event& event) {
    characterSelectionScreen.handleInput(event);
    
    if (characterSelectionScreen.areBothConfirmed()) {
        // Ambos jugadores confirmaron - pasamos a seleccionar carros
        currentState = GameState::CAR_SELECTION;
        carSelectionScreen.reset();
        std::cout << "Seleccionando carros..." << std::endl;
    }
}

void Game::handleCreditsInput(const sf::Event& event) {
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
        case GameState::NAME_INPUT_P1:
        case GameState::NAME_INPUT_P2:
            nameInputScreen.update();
            break;
        case GameState::CHARACTER_SELECTION:
            characterSelectionScreen.update();
            break;
        case GameState::CAR_SELECTION:
            // La pantalla de selección de carros se actualiza mediante inputs, así que aquí no hace falta nada
            break;
        case GameState::OPTIONS:
            optionsMenu.update();
            break;
        case GameState::PLAYING:
            break;
        case GameState::CREDITS:
            creditsScreen.update();
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
        case GameState::NAME_INPUT_P1:
        case GameState::NAME_INPUT_P2:
            nameInputScreen.render(window);
            break;
        case GameState::CHARACTER_SELECTION:
            characterSelectionScreen.render(window);
            break;
        case GameState::CAR_SELECTION:
            // Limpiamos, dibujamos la pantalla de carros y mostramos
            window.clear(sf::Color::Black);
            carSelectionScreen.render(window);
            window.display();
            break;
        case GameState::OPTIONS:
            optionsMenu.render(window);
            break;
        case GameState::PLAYING:
            window.clear(sf::Color::Black); // Limpiamos el fondo en negro
            window.draw(pistaSprite);
            window.draw(carroSprite);       
            window.display();               // Mostramos los cambios en la pantalla
            break;
        case GameState::CREDITS:
            creditsScreen.render(window);
            break;
        case GameState::QUIT:
            break;
    }
}