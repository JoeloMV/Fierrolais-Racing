#ifndef CREDITSSCREEN_H
#define CREDITSSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

class CreditsScreen {
private:
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Text> titleText;
    std::vector<sf::Text> creditTexts;
    
    bool loadFont();
    
public:
    CreditsScreen();
    ~CreditsScreen();
    
    void update();
    void render(sf::RenderWindow& window);
};

#endif // CREDITSSCREEN_H
