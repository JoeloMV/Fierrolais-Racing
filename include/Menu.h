#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <memory>

class Menu {
private:
    sf::RectangleShape darkPanel;    
    std::shared_ptr<sf::Font> font;
    std::vector<sf::Text> menuItems;
    int selectedIndex;
    sf::Color normalColor;
    sf::Color selectedColor;
    
    // Cambiado a un puntero inteligente o un objeto directo para evitar problemas de memoria
    std::shared_ptr<sf::Text> titleText; 

    sf::Texture backgroundTexture;
    sf::Sprite* backgroundSprite; // Se gestionará en el constructor/destructor

    bool loadFont();
    
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