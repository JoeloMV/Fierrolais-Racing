#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <memory>

class GameOverScreen {
private:
    std::shared_ptr<sf::Font> font;
    sf::Text titleText;
    sf::Text winnerText;
    sf::Text playAgainText;
    sf::Text exitText;

    int selectedOption; // 0 = Jugar de nuevo, 1 = Salir
    bool optionConfirmed;

    bool loadFont();
    void updateDisplay();

public:
    GameOverScreen();
    ~GameOverScreen();

    // Llama a esta función cuando acabe la carrera para poner quién ganó
    void setWinner(const std::string& winnerName); 
    
    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void reset();

    bool isConfirmed() const { return optionConfirmed; }
    int getSelectedOption() const { return selectedOption; }
};

#endif // GAMEOVERSCREEN_H