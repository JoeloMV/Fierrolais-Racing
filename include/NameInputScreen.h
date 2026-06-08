#ifndef NAMEINPUTSCREEN_H
#define NAMEINPUTSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <memory>

class NameInputScreen {
private:
    std::shared_ptr<sf::Font> font;
    std::string playerName;
    int currentPlayer;  // 1 o 2
    std::shared_ptr<sf::Text> titleText;
    std::shared_ptr<sf::Text> inputText;
    std::shared_ptr<sf::Text> instructionText;
    
    static const int MAX_NAME_LENGTH = 15;
    
    bool loadFont();
    void updateDisplay();
    
public:
    NameInputScreen();
    ~NameInputScreen();
    
    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void reset(int player);
    void update() {}  // No-op, solo para compatibilidad con Game
    
    const std::string& getPlayerName() const { return playerName; }
    bool isNameConfirmed() const;
};

#endif // NAMEINPUTSCREEN_H
