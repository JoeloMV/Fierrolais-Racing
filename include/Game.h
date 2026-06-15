#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <memory>
#include "Menu.h"
#include "SplashScreen.h"
#include "OptionsMenu.h"
#include "CreditsScreen.h"
#include "NameInputScreen.h"
#include "CharacterSelectionScreen.h"
#include "CarSelectionScreen.h"

enum class GameState {
    SPLASH,
    MENU,
    NAME_INPUT_P1,
    NAME_INPUT_P2,
    CHARACTER_SELECTION,
    CAR_SELECTION,
    PLAYING,
    COUNTDOWN,
    OPTIONS,
    GAME_OVER,
    CREDITS,
    QUIT
};

class Game {
private:
    sf::RenderWindow window;
    bool isRunning;
    GameState currentState;
    GameState previousState;
    Menu menu;
    SplashScreen splashScreen;
    OptionsMenu optionsMenu;
    CreditsScreen creditsScreen;
    NameInputScreen nameInputScreen;
    CharacterSelectionScreen characterSelectionScreen;
    CarSelectionScreen carSelectionScreen;
    std::shared_ptr<sf::Music> backgroundMusic;
    sf::Texture pistaTexture;
    sf::Sprite pistaSprite;
    sf::Image pistaImage;
    sf::Font fuente;
    sf::Text textoCuentaRegresiva;
    float tiempoCuentaRegresiva = 3.5f;
    sf::Texture carroTexture;
    sf::Sprite carroSprite;
    sf::Texture carro2Texture;
    sf::Sprite carro2Sprite;
    sf::Clock relojCuenta;
    // --- FÍSICAS JUGADOR 1 ---
    float speed = 0.0f;
    float angle = 180.0f; // Inicia en 180 para apuntar a la meta

    // --- FÍSICAS JUGADOR 2 ---
    float speed2 = 0.0f;
    float angle2 = 180.0f;

    // --- CONFIGURACIÓN DE LOS MOTORES ---
    const float maxSpeed = 300.0f;   // Velocidad máxima
    const float accel = 200.0f;      // Qué tan rápido acelera
    const float decel = 150.0f;      // Desaceleración / Fricción
    const float turnSpeed = 150.0f;  // Qué tan rápido da las vueltas
    std::string player1Name;
    std::string player2Name;
    
    void loadBackgroundMusic();
    
public:
    Game();
    ~Game();
    
    void run();
    void update();
    void render();
    void handleNameInput(const sf::Event& event);
    void handleCharacterSelectionInput(const sf::Event& event);
    void handleEvents();
    void handleMenuInput(const sf::Event& event);
    void handleSplashInput(const sf::Event& event);
    void handleOptionsInput(const sf::Event& event);
    void handleCreditsInput(const sf::Event& event);
};

#endif // GAME_H
