#include "Game.h"
#include <iostream>
#include <vector>
#include <cmath>
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

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true), currentState(GameState::SPLASH), previousState(GameState::SPLASH), pistaSprite(pistaTexture), carroSprite(carroTexture), carro2Sprite(carro2Texture) {
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
            carSelectionScreen.handleInput(*event);
            if (carSelectionScreen.areBothConfirmed()) {
                currentState = GameState::PLAYING;
                std::cout << "Iniciando juego..." << std::endl;

                int p1 = carSelectionScreen.getPlayer1Selection();
                int p2 = carSelectionScreen.getPlayer2Selection();

                std::vector<std::string> rutasCarros = {
                    "assets/carro1.png",
                    "assets/carro2.png",
                    "assets/carro3.png",
                    "assets/carro4.png",
                    "assets/carro5.png"
                };

                // ========================================================
                // 🛠️ EL MOLDE: AQUÍ SE CONTROLA EL TAMANO UNIFICADO 🛠️
                // ========================================================
                // No importa el archivo, todos se forzarán a estas medidas exactas.
                float anchoDeseado = 61.0f; 
                float largoDeseado = 83.0f;

                // --- PROCESAMIENTO JUGADOR 1 ---
                if (p1 >= 0 && p1 < rutasCarros.size()) {
                    carroTexture.loadFromFile(rutasCarros[p1]);
                    carroSprite.setTexture(carroTexture, true);
                    
                    // Encontramos el centro de la imagen para que gire derecho
                    sf::FloatRect bounds = carroSprite.getLocalBounds();
                    carroSprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
                    carroSprite.setRotation(sf::degrees(270.0f));

                    // FÓRMULA DE ESCALA AUTOMÁTICA (Tamaño Deseado / Tamaño Real)
                    float escalaX = anchoDeseado / bounds.size.x;
                    float escalaY = largoDeseado / bounds.size.y;
                    carroSprite.setScale(sf::Vector2f(escalaX, escalaY));

                    // Posición base (Modifícala tú después a tu gusto)
                    carroSprite.setPosition(sf::Vector2f(472.0f, 125.0f));
                }

                // --- PROCESAMIENTO JUGADOR 2 ---
                if (p2 >= 0 && p2 < rutasCarros.size()) {
                    carro2Texture.loadFromFile(rutasCarros[p2]);
                    carro2Sprite.setTexture(carro2Texture, true);
                    
                    // Encontramos el centro de la imagen para que gire derecho
                    sf::FloatRect bounds2 = carro2Sprite.getLocalBounds();
                    carro2Sprite.setOrigin(sf::Vector2f(bounds2.size.x / 2.0f, bounds2.size.y / 2.0f));
                    carro2Sprite.setRotation(sf::degrees(270.0f));

                    // FÓRMULA DE ESCALA AUTOMÁTICA (Mismos tamaños exactos del molde)
                    float escalaX2 = anchoDeseado / bounds2.size.x;
                    float escalaY2 = largoDeseado / bounds2.size.y;
                    carro2Sprite.setScale(sf::Vector2f(escalaX2, escalaY2));

                    // Posición base (Modifícala tú después a tu gusto)
                    carro2Sprite.setPosition(sf::Vector2f(472.0f, 160.0f));
                }
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
        case GameState::PLAYING: {
            // Como el juego corre limitado a 60 FPS fijos, definimos un tiempo por cuadro constante
            float dt = 1.0f / 60.0f; 

            // ==========================================
            // 🔥 CONTROL JUGADOR 1 (Flechas de dirección)
            // ==========================================
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                angle -= turnSpeed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                angle += turnSpeed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                speed += accel * dt;
                if (speed > maxSpeed) speed = maxSpeed;
            } 
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                speed -= accel * dt;
                if (speed < -maxSpeed / 2.0f) speed = -maxSpeed / 2.0f; // Reversa
            } 
            else {
                if (speed > 0) {
                    speed -= decel * dt;
                    if (speed < 0) speed = 0;
                } else if (speed < 0) {
                    speed += decel * dt;
                    if (speed > 0) speed = 0;
                }
            }

            // Mover el carro libremente usando el ángulo matemático
            float radianes = angle * 3.14159265f / 180.0f;
            carroSprite.move(sf::Vector2f(std::cos(radianes) * speed * dt, std::sin(radianes) * speed * dt));
            carroSprite.setRotation(sf::degrees(angle + 90.0f));


            // ==========================================
            // 🔥 CONTROL JUGADOR 2 (Teclas W, A, S, D)
            // ==========================================
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                angle2 -= turnSpeed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                angle2 += turnSpeed * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                speed2 += accel * dt;
                if (speed2 > maxSpeed) speed2 = maxSpeed;
            } 
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                speed2 -= accel * dt;
                if (speed2 < -maxSpeed / 2.0f) speed2 = -maxSpeed / 2.0f;
            } 
            else {
                if (speed2 > 0) {
                    speed2 -= decel * dt;
                    if (speed2 < 0) speed2 = 0;
                } else if (speed2 < 0) {
                    speed2 += decel * dt;
                    if (speed2 > 0) speed2 = 0;
                }
            }

            // Mover el carro 2 libremente usando el ángulo matemático
            float radianes2 = angle2 * 3.14159265f / 180.0f;
            carro2Sprite.move(sf::Vector2f(std::cos(radianes2) * speed2 * dt, std::sin(radianes2) * speed2 * dt));
            carro2Sprite.setRotation(sf::degrees(angle2 + 90.0f));

            break;
        }
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
            window.draw(carro2Sprite);      
            window.display();               // Mostramos los cambios en la pantalla
            break;
        case GameState::CREDITS:
            creditsScreen.render(window);
            break;
        case GameState::QUIT:
            break;
    }
}