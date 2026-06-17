#include "Game.h"
#include "GameOverScreen.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/System/Clock.hpp>

// ==========================================
// VARIABLES GLOBALES
// ==========================================
int vueltasP1 = 0;
int vueltasP2 = 0;
bool p1PasoMitad = false;
bool p2PasoMitad = false;

std::vector<float> tiemposP1;
std::vector<float> tiemposP2;
sf::Clock relojVueltaP1;
sf::Clock relojVueltaP2;

sf::Font fuenteMarcador;
std::string nombreGuardadoJ1 = "Jugador 1";
std::string nombreGuardadoJ2 = "Jugador 2";
GameOverScreen gameOver; 

// Variables para las imágenes de los personajes en la partida
sf::Texture hudCharTexP1;
sf::Texture hudCharTexP2;
sf::Sprite hudCharSprP1(hudCharTexP1);
sf::Sprite hudCharSprP2(hudCharTexP2);

// ==========================================
// MÉTODOS DE LA CLASE GAME
// ==========================================

void Game::loadBackgroundMusic() {
    backgroundMusic = std::make_shared<sf::Music>();
    
    std::vector<std::string> musicPaths = {
        "assets/sounds/Musica1.mp3",
        "assets/sounds/Musica2.mp3",
        "assets/sounds/background.ogg",
        "assets/sounds/background.wav",
        "assets/sounds/background.flac",
        "assets/sounds/musica.ogg",
        "assets/sounds/musica.wav",
        "assets/sounds/musica.flac"
    };
    
    if (!fuenteMarcador.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        (void)fuenteMarcador.openFromFile("assets/arial.ttf");
    }
    textoCuentaRegresiva.setFont(fuenteMarcador);
    textoCuentaRegresiva.setCharacterSize(100); 
    textoCuentaRegresiva.setFillColor(sf::Color::Yellow); 
    textoCuentaRegresiva.setPosition(sf::Vector2f(500.0f, 300.0f)); 
    
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

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true), currentState(GameState::SPLASH), previousState(GameState::SPLASH), pistaSprite(pistaTexture), carroSprite(carroTexture), carro2Sprite(carro2Texture), textoCuentaRegresiva(fuente)
{
    window.setFramerateLimit(60);
    loadBackgroundMusic();
    

    if (!fuente.openFromFile("assets/arial.ttf")) {
        std::cerr << "Error al cargar la fuente arial.ttf" << std::endl;
    }
    
    textoCuentaRegresiva.setFont(fuente);
    textoCuentaRegresiva.setCharacterSize(100); 
    textoCuentaRegresiva.setFillColor(sf::Color::Yellow);
    textoCuentaRegresiva.setPosition(sf::Vector2f(550.0f, 300.0f));
    
    if (!pistaTexture.loadFromFile("assets/pista.png")) {
        std::cerr << "Error al cargar pista.png" << std::endl;
    }
    if (!pistaImage.loadFromFile("assets/pista.png")) {
        std::cerr << "Error al cargar pistaImage" << std::endl;
    }

    pistaSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(626, 417)));
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
            GameState estadoAnterior = currentState;
            handleNameInput(*event);

            if (estadoAnterior == GameState::NAME_INPUT_P1 && currentState == GameState::NAME_INPUT_P2) {
                nombreGuardadoJ1 = nameInputScreen.getPlayerName();
                if (nombreGuardadoJ1.empty()) nombreGuardadoJ1 = "Jugador 1";
                player1Name = nombreGuardadoJ1;
            }
            else if (estadoAnterior == GameState::NAME_INPUT_P2 && currentState == GameState::CHARACTER_SELECTION) {
                nombreGuardadoJ2 = nameInputScreen.getPlayerName();             
                if (nombreGuardadoJ2.empty()) nombreGuardadoJ2 = "Jugador 2";
                player2Name = nombreGuardadoJ2;
            }
        } else if (currentState == GameState::CHARACTER_SELECTION) {
            handleCharacterSelectionInput(*event);
        } else if (currentState == GameState::CAR_SELECTION) {
            carSelectionScreen.handleInput(*event);
            if (carSelectionScreen.areBothConfirmed()) {
                currentState = GameState::COUNTDOWN;
                relojCuenta.restart();
                std::cout << "Iniciando juego..." << std::endl;

                int p1 = carSelectionScreen.getPlayer2Selection();
                int p2 = carSelectionScreen.getPlayer1Selection();

                std::cout << "DEBUG - Controles Corregidos -> P1 maneja: " << p2 << " | P2 maneja: " << p1 << std::endl;

                std::vector<std::string> rutasCarros = {
                    "assets/carro1.png",
                    "assets/carro2.png",
                    "assets/carro3.png",
                    "assets/carro4.png",
                    "assets/carro5.png"
                };

                float anchoDeseado = 60.0f;
                float largoDeseado = 83.0f;

                if (p1 >= 0 && p1 < rutasCarros.size()) {
                    (void)carroTexture.loadFromFile(rutasCarros[p1]);
                    carroSprite.setTexture(carroTexture, true);
                    
                    sf::FloatRect bounds = carroSprite.getLocalBounds();
                    carroSprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
                    carroSprite.setRotation(sf::degrees(270.0f));

                    float escalaX = anchoDeseado / bounds.size.x;
                    float escalaY = largoDeseado / bounds.size.y;
                    carroSprite.setScale(sf::Vector2f(escalaX, escalaY));
                    carroSprite.setPosition(sf::Vector2f(472.0f, 124.0f));
                }

                if (p2 >= 0 && p2 < rutasCarros.size()) {
                    (void)carro2Texture.loadFromFile(rutasCarros[p2]);
                    carro2Sprite.setTexture(carro2Texture, true);
                    
                    sf::FloatRect bounds2 = carro2Sprite.getLocalBounds();
                    carro2Sprite.setOrigin(sf::Vector2f(bounds2.size.x / 2.0f, bounds2.size.y / 2.0f));
                    carro2Sprite.setRotation(sf::degrees(270.0f));

                    float escalaX2 = anchoDeseado / bounds2.size.x;
                    float escalaY2 = largoDeseado / bounds2.size.y;
                    carro2Sprite.setScale(sf::Vector2f(escalaX2, escalaY2));
                    carro2Sprite.setPosition(sf::Vector2f(472.0f, 161.0f));
                }
            }
        } 
        // Con esto el Game Over por fin escucha el teclado de tu GameOverScreen.cpp
        else if (currentState == GameState::CAR_SELECTION) {
            carSelectionScreen.handleInput(*event);
            if (carSelectionScreen.areBothConfirmed()) {
                // ... todo el código donde cargas los carritos ...
            }
        } 
        // =========================================================
        // ¡ESTO ES LO QUE SEGURAMENTE TE FALTA EN GAME.CPP!
        // =========================================================
        else if (currentState == GameState::GAME_OVER) {
            gameOver.handleInput(*event); 
        }
        else if (currentState == GameState::OPTIONS) {
            handleOptionsInput(*event);
        } 
        else if (currentState == GameState::CREDITS) {
            handleCreditsInput(*event);
        }
    }
}

void Game::handleSplashInput(const sf::Event& event) {
    if (event.getIf<sf::Event::KeyPressed>()) {
        
        // 1. Apagamos la música del splash screen antes de cambiar de estado
        splashScreen.stopMusic(); 
        
        // 2. Pasamos al menú principal
        currentState = GameState::MENU;
        
        // 3. Iniciamos la música de fondo del juego
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
                    break;
                case 1:
                    previousState = GameState::MENU;
                    currentState = GameState::OPTIONS;
                    break;
                case 2:
                    previousState = GameState::MENU;
                    currentState = GameState::CREDITS;
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
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        
        // Salir al menú con ESC
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            currentState = GameState::MENU;
        }
        
        // Flechas Arriba y Abajo para cambiar de opción
        else if (keyEvent->code == sf::Keyboard::Key::Up) {
            optionsMenu.moveUp();
        }
        else if (keyEvent->code == sf::Keyboard::Key::Down) {
            optionsMenu.moveDown();
        }
        
        // Flecha Izquierda para BAJAR volumen
        else if (keyEvent->code == sf::Keyboard::Key::Left) {
            optionsMenu.decreaseVolume();
            
            // Si la música está sonando, aplicamos el cambio instantáneamente
            if (backgroundMusic) {
                backgroundMusic->setVolume(optionsMenu.getMusicVolume());
            }
            
            // Nota: Aquí en el futuro podrás hacer lo mismo con tus efectos de sonido
            // ej. sonidoMotor.setVolume(optionsMenu.getSfxVolume());
        }
        
        // Flecha Derecha para SUBIR volumen
        else if (keyEvent->code == sf::Keyboard::Key::Right) {
            optionsMenu.increaseVolume();
            
            // Aplicamos el cambio instantáneamente
            if (backgroundMusic) {
                backgroundMusic->setVolume(optionsMenu.getMusicVolume());
            }
        }
    }
}

void Game::handleNameInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            if (nameInputScreen.isNameConfirmed()) {
                if (currentState == GameState::NAME_INPUT_P1) {
                    player1Name = nameInputScreen.getPlayerName();
                    currentState = GameState::NAME_INPUT_P2;
                    nameInputScreen.reset(2);
                } else if (currentState == GameState::NAME_INPUT_P2) {
                    player2Name = nameInputScreen.getPlayerName();
                    currentState = GameState::CHARACTER_SELECTION;
                    characterSelectionScreen.setPlayerNames(player1Name, player2Name);
                    characterSelectionScreen.reset();
                }
            }
        }
    }
    nameInputScreen.handleInput(event);
}

void Game::handleCharacterSelectionInput(const sf::Event& event) {
    characterSelectionScreen.handleInput(event);
    
    if (characterSelectionScreen.areBothConfirmed()) {
        currentState = GameState::CAR_SELECTION;
        carSelectionScreen.reset();
        
        // --- TRUCO: Intercambiamos las lecturas para romper la inversión ---
        int charP1 = characterSelectionScreen.getPlayer2Selection();
        int charP2 = characterSelectionScreen.getPlayer1Selection();

        std::vector<std::string> rutasPersonajes = {
            "assets/mecha corta.jpeg",         // 0
            "assets/alucin.png",         // 1
            "assets/checo perez.png" ,    // 2
            "assets/licenciado.png",        // 3
            "assets/poeta.png",   // 4
            "assets/programador.png", // 5
            "assets/Salta montes.jpeg",       // 6
            "assets/Vaquero.png",   // 7
            "assets/mamado.png",        // 8
            "assets/fierrolais.jpeg"   // 9
        };

        if (charP1 >= rutasPersonajes.size()) charP1 = rutasPersonajes.size() - 1;
        if (charP2 >= rutasPersonajes.size()) charP2 = rutasPersonajes.size() - 1;
        if (charP1 < 0) charP1 = 0;
        if (charP2 < 0) charP2 = 0;

        // Procesar foto de la izquierda (Ahora será el verdadero P1)
        std::string rutaFinalP1 = rutasPersonajes[charP1];
        if (!hudCharTexP1.loadFromFile(rutaFinalP1)) {
            (void)hudCharTexP1.loadFromFile("assets/poeta.png"); 
        }
        hudCharSprP1.setTexture(hudCharTexP1, true);
        sf::FloatRect boundsP1 = hudCharSprP1.getLocalBounds();
        if (boundsP1.size.x > 0 && boundsP1.size.y > 0) {
            hudCharSprP1.setScale(sf::Vector2f(50.0f / boundsP1.size.x, 50.0f / boundsP1.size.y));
        }

        // Procesar foto de la derecha (Ahora será el verdadero P2)
        std::string rutaFinalP2 = rutasPersonajes[charP2];
        if (!hudCharTexP2.loadFromFile(rutaFinalP2)) {
            (void)hudCharTexP2.loadFromFile("assets/poeta.png");
        }
        hudCharSprP2.setTexture(hudCharTexP2, true);
        sf::FloatRect boundsP2 = hudCharSprP2.getLocalBounds();
        if (boundsP2.size.x > 0 && boundsP2.size.y > 0) {
            hudCharSprP2.setScale(sf::Vector2f(50.0f / boundsP2.size.x, 50.0f / boundsP2.size.y));
        }
    }
}

void Game::handleCreditsInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            currentState = previousState;
            menu.reset();
        }
    }
}

sf::Clock relojCuenta;

void Game::update() {
    switch (currentState) {
        case GameState::SPLASH:
            splashScreen.update();
            break;
        case GameState::MENU:
            menu.update();
            break;
        case GameState::NAME_INPUT_P1:
            if (nameInputScreen.isNameConfirmed()) { 
                player1Name = nameInputScreen.getPlayerName(); 
                currentState = GameState::NAME_INPUT_P2;      
                nameInputScreen.reset(2);                      
            }
            break;
        case GameState::NAME_INPUT_P2:
            if (nameInputScreen.isNameConfirmed()) { 
                player2Name = nameInputScreen.getPlayerName(); 
                currentState = GameState::CHARACTER_SELECTION;     
            }
            break;
        case GameState::CHARACTER_SELECTION:
            characterSelectionScreen.update();
            break;
        case GameState::CAR_SELECTION:
            break;
        case GameState::OPTIONS:
            break;
        case GameState::COUNTDOWN: {
            float tiempoTranscurrido = relojCuenta.getElapsedTime().asSeconds();
            int cuenta = 3 - static_cast<int>(tiempoTranscurrido);

            textoCuentaRegresiva.setFont(fuenteMarcador);
            textoCuentaRegresiva.setCharacterSize(100);

            if (cuenta > 0) {
                textoCuentaRegresiva.setString(std::to_string(cuenta));
                textoCuentaRegresiva.setFillColor(sf::Color::White);
                textoCuentaRegresiva.setPosition(sf::Vector2f(600.f, 300.f));
            } 
            else if (cuenta == 0) {
                textoCuentaRegresiva.setString("Gooooooooooo");
                textoCuentaRegresiva.setFillColor(sf::Color::Green);
                textoCuentaRegresiva.setPosition(sf::Vector2f(300.f, 300.f));
            } 
            else if (tiempoTranscurrido > 4.5f) {
                currentState = GameState::PLAYING;
                textoCuentaRegresiva.setString(""); 
                relojVueltaP1.restart();
                relojVueltaP2.restart();
            }
            break;
        }
        case GameState::PLAYING: {
            float dt = 1.0f / 60.0f; 

            sf::Vector2f oldPos1 = carroSprite.getPosition();
            sf::Vector2f oldPos2 = carro2Sprite.getPosition();

            // CONTROL JUGADOR 1
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) angle -= turnSpeed * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) angle += turnSpeed * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                speed += accel * dt; if (speed > maxSpeed) speed = maxSpeed;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                speed -= accel * dt; if (speed < -maxSpeed / 2.0f) speed = -maxSpeed / 2.0f;
            } else {
                if (speed > 0) { speed -= decel * dt; if (speed < 0) speed = 0; } 
                else if (speed < 0) { speed += decel * dt; if (speed > 0) speed = 0; }
            }
            float radianes = angle * 3.14159265f / 180.0f;
            carroSprite.move(sf::Vector2f(std::cos(radianes) * speed * dt, std::sin(radianes) * speed * dt));
            carroSprite.setRotation(sf::degrees(angle + 90.0f));

            // CONTROL JUGADOR 2
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) angle2 -= turnSpeed * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) angle2 += turnSpeed * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                speed2 += accel * dt; if (speed2 > maxSpeed) speed2 = maxSpeed;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                speed2 -= accel * dt; if (speed2 < -maxSpeed / 2.0f) speed2 = -maxSpeed / 2.0f;
            } else {
                if (speed2 > 0) { speed2 -= decel * dt; if (speed2 < 0) speed2 = 0; } 
                else if (speed2 < 0) { speed2 += decel * dt; if (speed2 > 0) speed2 = 0; }
            }
            float radianes2 = angle2 * 3.14159265f / 180.0f;
            carro2Sprite.move(sf::Vector2f(std::cos(radianes2) * speed2 * dt, std::sin(radianes2) * speed2 * dt));
            carro2Sprite.setRotation(sf::degrees(angle2 + 90.0f));

            // COLISIONES J1
            sf::Vector2f pos1 = carroSprite.getPosition();
            int imgX1 = static_cast<int>(pos1.x * (626.0f / 1200.0f));
            int imgY1 = static_cast<int>(pos1.y * (417.0f / 700.0f));

            if (imgX1 >= 0 && imgX1 < 626 && imgY1 >= 0 && imgY1 < 417) {
                sf::Color p1 = pistaImage.getPixel(sf::Vector2u(imgX1, imgY1));
                bool esPasto1 = (p1.g > 150 && p1.r < 140 && p1.b < 120); 
                bool esAgua1 = (p1.b > 210 && p1.g > 150 && p1.r < 100);
                bool esArena1 = (p1.r > 190 && p1.g > 160 && p1.g < 230 && p1.b > 130 && p1.b < 180);

                if (esPasto1 || esAgua1 || esArena1) {
                    carroSprite.setPosition(oldPos1); 
                    speed = speed * -0.2f; 
                }
            } else {
                carroSprite.setPosition(oldPos1);
                speed = speed * -0.2f;
            }

            // COLISIONES J2
            sf::Vector2f pos2 = carro2Sprite.getPosition();
            int imgX2 = static_cast<int>(pos2.x * (626.0f / 1200.0f));
            int imgY2 = static_cast<int>(pos2.y * (417.0f / 700.0f));

            if (imgX2 >= 0 && imgX2 < 626 && imgY2 >= 0 && imgY2 < 417) {
                sf::Color p2 = pistaImage.getPixel(sf::Vector2u(imgX2, imgY2));
                bool esPasto2 = (p2.g > 150 && p2.r < 140 && p2.b < 120); 
                bool esAgua2 = (p2.b > 210 && p2.g > 150 && p2.r < 100);
                bool esArena2 = (p2.r > 190 && p2.g > 160 && p2.g < 230 && p2.b > 130 && p2.b < 180);

                if (esPasto2 || esAgua2 || esArena2) {
                    carro2Sprite.setPosition(oldPos2); 
                    speed2 = speed2 * -0.2f; 
                }
            } else {
                carro2Sprite.setPosition(oldPos2);
                speed2 = speed2 * -0.2f;
            }

            // VUELTAS Y CHECKPOINTS
            sf::FloatRect lineaMeta(sf::Vector2f(350.0f, 100.0f), sf::Vector2f(20.0f, 150.0f));   
            sf::FloatRect checkpoint(sf::Vector2f(500.0f, 600.0f), sf::Vector2f(200.0f, 100.0f));

            if (carroSprite.getGlobalBounds().findIntersection(checkpoint)) p1PasoMitad = true; 
            if (carroSprite.getGlobalBounds().findIntersection(lineaMeta)) {
                if (p1PasoMitad == true) {
                    vueltasP1++; 
                    p1PasoMitad = false; 
                    float tiempoVuelta = relojVueltaP1.restart().asSeconds();
                    tiemposP1.push_back(tiempoVuelta);
                }
            }

            if (carro2Sprite.getGlobalBounds().findIntersection(checkpoint)) p2PasoMitad = true; 
            if (carro2Sprite.getGlobalBounds().findIntersection(lineaMeta)) {
                if (p2PasoMitad == true) {
                    vueltasP2++; 
                    p2PasoMitad = false; 
                    float tiempoVuelta = relojVueltaP2.restart().asSeconds();
                    tiemposP2.push_back(tiempoVuelta);
                }
            }

            if (vueltasP1 >= 3) {
            // El código cree que ganó el Carro 1, pero en la vida real este es el Jugador 2
            gameOver.setWinner(player2Name);
            gameOver.setWinnerImage(hudCharTexP2);
            gameOver.setPlayerStats(tiemposP2, tiemposP1); // Mandamos los tiempos reales del J1 a la izquierda y los del J2 a la derecha
            currentState = GameState::GAME_OVER;
            
        } else if (vueltasP2 >= 3) {
            // El código cree que ganó el Carro 2, pero en la vida real este es el Jugador 1
            gameOver.setWinner(player1Name);
            gameOver.setWinnerImage(hudCharTexP1);
            gameOver.setPlayerStats(tiemposP2, tiemposP1); // Mandamos los tiempos reales del J1 a la izquierda y los del J2 a la derecha
            currentState = GameState::GAME_OVER;
        }
            break;
        }
        case GameState::GAME_OVER: { 
            if (gameOver.isConfirmed()) {
                if (gameOver.getSelectedOption() == 0) {
                    gameOver.reset();
                    carSelectionScreen.reset();
                    vueltasP1 = 0;
                    vueltasP2 = 0;
                    p1PasoMitad = false;
                    p2PasoMitad = false;
                    tiemposP1.clear();
                    tiemposP2.clear();
                    speed = 0.0f;
                    speed2 = 0.0f;
                    currentState = GameState::CAR_SELECTION;
                } else {
                    window.close();
                    isRunning = false;
                }
            }
            break;
        }
        case GameState::CREDITS:
            creditsScreen.update();
            break;
        case GameState::QUIT:
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
            window.clear(sf::Color::Black);
            carSelectionScreen.render(window);
            window.display();
            break;
        case GameState::OPTIONS:
            optionsMenu.render(window);
            break;
        case GameState::COUNTDOWN:
        case GameState::PLAYING: {
            window.clear(sf::Color::Black);
            window.draw(pistaSprite);
            window.draw(carroSprite);
            window.draw(carro2Sprite);
            window.draw(textoCuentaRegresiva);

            // DIBUJAR MARCADOR DEL JUGADOR 2 (Izquierda)
            hudCharSprP2.setPosition(sf::Vector2f(20.f, 10.f)); 
            window.draw(hudCharSprP2);

            sf::Text marcadorP2(fuenteMarcador);
            marcadorP2.setCharacterSize(30);
            marcadorP2.setFillColor(sf::Color::Blue);
            marcadorP2.setPosition(sf::Vector2f(80.f, 20.f)); 
            marcadorP2.setString(player1Name + " lleva: " + std::to_string(vueltasP2) + " / 3 vueltas");
            window.draw(marcadorP2);

            // DIBUJAR MARCADOR DEL JUGADOR 1 (Derecha)
            hudCharSprP1.setPosition(sf::Vector2f(780.f, 10.f)); 
            window.draw(hudCharSprP1);

            sf::Text marcadorP1(fuenteMarcador);
            marcadorP1.setCharacterSize(30);
            marcadorP1.setFillColor(sf::Color::Red);
            marcadorP1.setPosition(sf::Vector2f(840.f, 20.f)); 
            marcadorP1.setString(player2Name + " lleva: " + std::to_string(vueltasP1) + " / 3 vueltas");
            window.draw(marcadorP1);

            window.display();
            break;
        }
        case GameState::GAME_OVER: { 
            window.clear(sf::Color::Black);
            gameOver.render(window);
            window.display();
            break;
        }
        case GameState::CREDITS:
            creditsScreen.render(window);
            break;
        case GameState::QUIT:
            break;
    }
}