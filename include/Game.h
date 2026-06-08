#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include "Menu.h"

enum class GameState {
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
    Menu menu;
    
public:
    Game();
    ~Game();
    
    void run();
    void update();
    void render();
    void handleEvents();
    void handleMenuInput(const sf::Event& event);
};

#endif // GAME_H
