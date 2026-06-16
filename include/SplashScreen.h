#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // <-- NUEVA LIBRERÍA DE AUDIO
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
    
    sf::Music splashMusic; // <-- NUEVO REPRODUCTOR DE MÚSICA
    
    bool loadFont();
    
public:
    SplashScreen();
    ~SplashScreen();
    
    void update();
    void render(sf::RenderWindow& window);
    
    void stopMusic(); // <-- NUEVA FUNCIÓN PARA APAGARLA
};

#endif // SPLASHSCREEN_H