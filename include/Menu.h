#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <memory>

class Menu {
private:
    std::shared_ptr<sf::Font> font;
    std::vector<sf::Text> menuItems;
    int selectedIndex;
    sf::Color normalColor;
    sf::Color selectedColor;
    std::shared_ptr<sf::Text> titleText;
    bool loadFont();
    
    protected:
    sf::Texture backgroundTexture;
    sf::Sprite* backgroundSprite;

public:
    Menu();
    ~Menu();
    
    void handleInput(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);
    int getSelectedOption() const;
    void reset();
};

#endif // MENU_H