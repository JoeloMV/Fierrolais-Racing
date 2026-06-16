#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class OptionsMenu {
private:
    sf::Font font; // Ahora es un objeto directo
    sf::Text titleText;
    std::vector<sf::Text> menuOptions;
    
    int selectedItemIndex;
    float musicVolume;
    float sfxVolume;
    
    bool loadFont();
    void updateTextDisplays();

public:
    OptionsMenu();
    ~OptionsMenu();
    
    void moveUp();
    void moveDown();
    void increaseVolume();
    void decreaseVolume();
    
    float getMusicVolume() const;
    float getSfxVolume() const;
    
    void render(sf::RenderWindow& window);
};

#endif // OPTIONSMENU_H