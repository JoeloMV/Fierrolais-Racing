#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <memory>
#include <vector> // <-- Nueva libreria para listas de tiempos

class GameOverScreen {
private:
    std::shared_ptr<sf::Font> font;
    sf::Text titleText;
    sf::Text winnerText;
    
    // Nuevos textos para las estadísticas
    sf::Text p1StatsText; 
    sf::Text p2StatsText;

    sf::Text playAgainText;
    sf::Text exitText;

    int selectedOption; // 0 = Jugar de nuevo, 1 = Salir
    bool optionConfirmed;

    bool loadFont();
    void updateDisplay();

public:
    GameOverScreen();
    ~GameOverScreen();

    void setWinner(const std::string& winnerName); 
    
    // Nueva función para pasarle los tiempos de las vueltas
    void setPlayerStats(const std::vector<float>& p1Laps, const std::vector<float>& p2Laps);
    
    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void reset();

    bool isConfirmed() const { return optionConfirmed; }
    int getSelectedOption() const { return selectedOption; }
};

#endif // GAMEOVERSCREEN_H