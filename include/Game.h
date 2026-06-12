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
    OPTIONS,
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
    sf::Texture carroTexture;
    sf::Sprite carroSprite;
    std::string player1Name;
    std::string player2Name;
    
    void loadBackgroundMusic();
    
public:
    Game();
    ~Game();
    
    void run();
    void update();
    void render();
    void handleNameInputInput(const sf::Event& event);
    void handleCharacterSelectionInput(const sf::Event& event);
    void handleEvents();
    void handleMenuInput(const sf::Event& event);
    void handleSplashInput(const sf::Event& event);
    void handleOptionsInput(const sf::Event& event);
    void handleCreditsInput(const sf::Event& event);
};

#endif // GAME_H
