#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>

class Game {
private:
    sf::RenderWindow window;
    bool isRunning;
    
public:
    Game();
    ~Game();
    
    void run();
    void update();
    void render();
    void handleEvents();
};

#endif // GAME_H
