#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <memory>
#include <vector>

class GameOverScreen {
private:
    std::shared_ptr<sf::Font> font;
    bool fontLoaded; // ESCUDO ANTI-CRASHEOS
    
    sf::Text titleText;
    sf::Text winnerText;
    
    std::optional<sf::Sprite> winnerSprite;
    
    sf::Texture bgTexture;
    std::optional<sf::Sprite> bgSprite; 
    sf::RectangleShape overlay;

    sf::Text p1StatsText; 
    sf::Text p2StatsText;
    sf::Text playAgainText;
    sf::Text exitText;

    int selectedOption;
    bool optionConfirmed;

    bool loadFont();
    void updateDisplay();

public:
    GameOverScreen();
    ~GameOverScreen();

    void setWinner(const std::string& winnerName); 
    void setWinnerImage(const sf::Texture& texture); 
    void setPlayerStats(const std::vector<float>& p1Laps, const std::vector<float>& p2Laps);
    
    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void reset();

    bool isConfirmed() const { return optionConfirmed; }
    int getSelectedOption() const { return selectedOption; }
};

#endif // GAMEOVERSCREEN_H