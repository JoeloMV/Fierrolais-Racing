#include "Game.h"
#include <iostream>
#include <vector>
#include <cmath>
// ==========================================
// VARIABLES GLOBALES DE VUELTAS
// ==========================================
int vueltasP1 = 0;
int vueltasP2 = 0;
bool p1PasoMitad = false;
bool p2PasoMitad = false;
sf::Font fuenteMarcador;
std::string nombreGuardadoJ1 = "Jugador 1";
std::string nombreGuardadoJ2 = "Jugador 2";

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
    // Cargar fuente para el marcador en pantalla
    if (!fuenteMarcador.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Si no la encuentra ahí, intenta en la carpeta del proyecto
        fuenteMarcador.openFromFile("assets/arial.ttf");
    }
    
  for (const auto& path : musicPaths) {
        if (backgroundMusic->openFromFile(path)) {
            std::cout << "Música cargada desde: " << path << std::endl;
            backgroundMusic->setLooping(true);
            backgroundMusic->setVolume(70.0f);
            return;
        }
    }

    std::cerr << "Advertencia: No se pudo cargar el archivo de música" << std::endl;
} // <--- ESTA LLAVE CIERRA DEFINITIVAMENTE void Game::loadBackgroundMusic()

Game::Game() : window(sf::VideoMode(sf::Vector2u(1200, 700)), "Fierrolais Racing"), isRunning(true), currentState(GameState::SPLASH), previousState(GameState::SPLASH), pistaSprite(pistaTexture), carroSprite(carroTexture), carro2Sprite(carro2Texture), textoCuentaRegresiva(fuente)
{
    window.setFramerateLimit(60);
    loadBackgroundMusic();
    optionsMenu.setBackgroundMusic(backgroundMusic);

    // --- CONFIGURACIÓN DE LA CUENTA REGRESIVA ---
    if (!fuente.openFromFile("assets/arial.ttf")) {
        std::cerr << "Error al cargar la fuente arial.ttf" << std::endl;
    }
    
    textoCuentaRegresiva.setFont(fuente);
    textoCuentaRegresiva.setCharacterSize(100); // Tamaño grande
    textoCuentaRegresiva.setFillColor(sf::Color::Yellow);
    
    // Posición aproximada al centro de la pantalla
    // (Ajusta los números si tu ventana tiene otro tamaño)
    textoCuentaRegresiva.setPosition(sf::Vector2f(550.0f, 300.0f));
   // Cargamos la imagen de la pista
    if (!pistaTexture.loadFromFile("assets/pista.png")) {
        std::cerr << "Error al cargar pista.png" << std::endl;
    }
    // 🔥 NUEVO: Cargamos la pista en la memoria para leer el color del pasto
    if (!pistaImage.loadFromFile("assets/pista.png")) {
        std::cerr << "Error al cargar pistaImage" << std::endl;
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
            // 1. Memorizamos el estado actual antes de procesar
            GameState estadoAnterior = currentState;

            // 2. Tu función original intacta
            handleNameInput(*event);

            // 3. Captura dinámica del jugador 1
            if (estadoAnterior == GameState::NAME_INPUT_P1 && currentState == GameState::NAME_INPUT_P2) {
                nombreGuardadoJ1 = nameInputScreen.getPlayerName();
                if (nombreGuardadoJ1.empty()) nombreGuardadoJ1 = "Jugador 1";
            } 
            // 4. Captura dinámica del jugador 2
            else if (estadoAnterior == GameState::NAME_INPUT_P2 && currentState == GameState::CHARACTER_SELECTION) {
                nombreGuardadoJ2 = nameInputScreen.getPlayerName();
                if (nombreGuardadoJ2.empty()) nombreGuardadoJ2 = "Jugador 2";
            }
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
                float anchoDeseado = 60.0f; 
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
                    carroSprite.setPosition(sf::Vector2f(472.0f, 124.0f));
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
                    carro2Sprite.setPosition(sf::Vector2f(472.0f, 161.0f));
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

void Game::handleNameInput(const sf::Event& event) {
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
    // ... tu código anterior ...
    if (nameInputScreen.isNameConfirmed()) { // 👈 Cambiado a isNameConfirmed()
        player1Name = nameInputScreen.getPlayerName(); 
        currentState = GameState::NAME_INPUT_P2;      
        nameInputScreen.reset(2); // 👈 Le enviamos el '2' como argumento para el siguiente jugador                     
    }
    break;

case GameState::NAME_INPUT_P2:
    // ... tu código anterior ...
    if (nameInputScreen.isNameConfirmed()) { // 👈 Cambiado a isNameConfirmed()
        player2Name = nameInputScreen.getPlayerName(); 
        currentState = GameState::CAR_SELECTION;      
    }
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
            case GameState::COUNTDOWN: {
            // Restamos el tiempo (asumiendo que tu juego va a 60 frames por segundo)
            float dt = 1.0f / 60.0f; 
            tiempoCuentaRegresiva -= dt;

            // Cambiamos el texto y color dependiendo del tiempo
            if (tiempoCuentaRegresiva > 2.0f) {
                textoCuentaRegresiva.setString("3");
            } else if (tiempoCuentaRegresiva > 1.0f) {
                textoCuentaRegresiva.setString("2");
                textoCuentaRegresiva.setFillColor(sf::Color(255, 165, 0)); // Naranja
            } else if (tiempoCuentaRegresiva > 0.0f) {
                textoCuentaRegresiva.setString("1");
                textoCuentaRegresiva.setFillColor(sf::Color::Red);
            } else if (tiempoCuentaRegresiva > -1.0f) {
                textoCuentaRegresiva.setString("!YA!");
                textoCuentaRegresiva.setFillColor(sf::Color::Green);
            } else {
                // Cuando el tiempo se acaba, pasamos al estado de juego normal
                currentState = GameState::PLAYING; 
            }
            break;
        }
        case GameState::PLAYING: {
            float dt = 1.0f / 60.0f; 

            // 1. 📍 GUARDAR LAS POSICIONES VIEJAS ANTES DE MOVER
            sf::Vector2f oldPos1 = carroSprite.getPosition();
            sf::Vector2f oldPos2 = carro2Sprite.getPosition();

            // ==========================================
            // 🔥 CONTROL Y MOVIMIENTO JUGADOR 1 (Como ya lo tenías)
            // ==========================================
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

            // ==========================================
            // 🔥 CONTROL Y MOVIMIENTO JUGADOR 2 (Como ya lo tenías)
            // ==========================================
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

         // =======================================================
            // 🚧 SISTEMA DE COLISIONES GEMELAS (P1 y P2 IDÉNTICOS) 🚧
            // =======================================================
            
            // --- JUGADOR 1 (Carro Rojo) ---
            sf::Vector2f pos1 = carroSprite.getPosition();
            int imgX1 = static_cast<int>(pos1.x * (626.0f / 1200.0f));
            int imgY1 = static_cast<int>(pos1.y * (417.0f / 700.0f));

            if (imgX1 >= 0 && imgX1 < 626 && imgY1 >= 0 && imgY1 < 417) {
                sf::Color p1 = pistaImage.getPixel(sf::Vector2u(imgX1, imgY1));
                
                // REGLAS SÚPER ESTRICTAS DE FUERA DE PISTA:
                // 1. El pasto real es muy verde, ignoramos cualquier verde mezclado.
                bool esPasto1 = (p1.g > 150 && p1.r < 140 && p1.b < 120); 
                // 2. El lago es de un azul muy brillante, ignoramos los bordes azules oscuros.
                bool esAgua1 = (p1.b > 210 && p1.g > 150 && p1.r < 100);
                // 3. La arena es beige (tiene azul). Ignoramos el amarillo puro de la pista.
                bool esArena1 = (p1.r > 190 && p1.g > 160 && p1.g < 230 && p1.b > 130 && p1.b < 180);

                if (esPasto1 || esAgua1 || esArena1) {
                    carroSprite.setPosition(oldPos1); 
                    speed = speed * -0.2f; 
                }
            } else {
                carroSprite.setPosition(oldPos1);
                speed = speed * -0.2f;
            }

            // --- JUGADOR 2 (Carro Gris) ---
            sf::Vector2f pos2 = carro2Sprite.getPosition();
            int imgX2 = static_cast<int>(pos2.x * (626.0f / 1200.0f));
            int imgY2 = static_cast<int>(pos2.y * (417.0f / 700.0f));

            if (imgX2 >= 0 && imgX2 < 626 && imgY2 >= 0 && imgY2 < 417) {
                sf::Color p2 = pistaImage.getPixel(sf::Vector2u(imgX2, imgY2));
                
                // REGLAS IDÉNTICAS PARA QUE EL JUGADOR 2 TENGA LA MISMA VENTAJA:
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
            // ==========================================
        // 🏁 SISTEMA DE VUELTAS Y ANTI-TRAMPAS 🏁
        // ==========================================
        // Coordenadas actualizadas para SFML 3: (Posición X, Y), (Ancho, Alto)
      sf::FloatRect lineaMeta(sf::Vector2f(350.0f, 100.0f), sf::Vector2f(20.0f, 150.0f));   
        sf::FloatRect checkpoint(sf::Vector2f(500.0f, 600.0f), sf::Vector2f(200.0f, 100.0f));

        // --- JUGADOR 1 (ROJO) ---
        if (carroSprite.getGlobalBounds().findIntersection(checkpoint)) {
            p1PasoMitad = true; 
        }
        if (carroSprite.getGlobalBounds().findIntersection(lineaMeta)) {
            if (p1PasoMitad == true) {
                vueltasP1++; 
                p1PasoMitad = false; 
                std::cout << "¡Jugador 1 completó la vuelta! Llevas: " << vueltasP1 << std::endl;
            }
        }

        // --- JUGADOR 2 (GRIS) ---
        if (carro2Sprite.getGlobalBounds().findIntersection(checkpoint)) {
            p2PasoMitad = true; 
        }
        if (carro2Sprite.getGlobalBounds().findIntersection(lineaMeta)) {
            if (p2PasoMitad == true) {
                vueltasP2++; 
                p2PasoMitad = false; 
                std::cout << "¡Jugador 2 completó la vuelta! Llevas: " << vueltasP2 << std::endl;
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
       case GameState::COUNTDOWN:
     case GameState::PLAYING:
    {
        window.clear(sf::Color::Black);
        window.draw(pistaSprite);
        window.draw(carroSprite);
        window.draw(carro2Sprite);
        window.draw(textoCuentaRegresiva);

        // --- Marcador Jugador 1 ---
        sf::Text marcadorP1(fuenteMarcador);
        if (vueltasP1 >= 3) {
            marcadorP1.setString(player1Name + " HA GANADO EL JUEGO!");
            marcadorP1.setFillColor(sf::Color::Yellow);
        } else {
            marcadorP1.setString(player1Name + " lleva: " + std::to_string(vueltasP1) + " / 3 vueltas");
            marcadorP1.setFillColor(sf::Color::Red);
        }
        marcadorP1.setCharacterSize(24);
        marcadorP1.setPosition(sf::Vector2f(30.0f, 20.0f));
        window.draw(marcadorP1);

        // --- Marcador Jugador 2 ---
        sf::Text marcadorP2(fuenteMarcador);
        if (vueltasP2 >= 3) {
            marcadorP2.setString(player2Name + " HA GANADO EL JUEGO!");
            marcadorP2.setFillColor(sf::Color::Yellow);
        } else {
            marcadorP2.setString(player2Name + " lleva: " + std::to_string(vueltasP2) + " / 3 vueltas");
            marcadorP2.setFillColor(sf::Color::White);
        }
        marcadorP2.setCharacterSize(24);
        marcadorP2.setPosition(sf::Vector2f(30.0f, 55.0f));
        window.draw(marcadorP2);

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