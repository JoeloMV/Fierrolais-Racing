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

enum class GameState {
    SPLASH,
    MENU,
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
    std::shared_ptr<sf::Music> backgroundMusic;
    
    void loadBackgroundMusic();
    
public:
    Game();
    ~Game();
    
    void run();
    void update();
    void render();
    void handleEvents();
    void handleMenuInput(const sf::Event& event);
    void handleSplashInput(const sf::Event& event);
    void handleOptionsInput(const sf::Event& event);
    void handleCreditsInput(const sf::Event& event);
};

#endif // GAME_H
