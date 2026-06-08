#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

class SplashScreen {
private:
    sf::Texture texture;
    std::shared_ptr<sf::Sprite> sprite;
    std::shared_ptr<sf::Text> continueText;
    std::shared_ptr<sf::Font> font;
    float textAlpha;
    float alphaDelta;
    bool fadeIn;
    
    bool loadFont();
    
public:
    SplashScreen();
    ~SplashScreen();
    
    void update();
    void render(sf::RenderWindow& window);
};

#endif // SPLASHSCREEN_H
