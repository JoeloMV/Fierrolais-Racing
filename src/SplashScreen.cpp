#include "SplashScreen.h"
#include <iostream>

bool SplashScreen::loadFont() {
    font = std::make_shared<sf::Font>();
    
    if (font->openFromFile("assets/fonts/arial.ttf")) {
        return true;
    }
    
    std::cerr << "Error: No se pudo cargar la fuente desde assets." << std::endl;
    return false;
}

SplashScreen::SplashScreen() : textAlpha(0), alphaDelta(3), fadeIn(true) {
    // Cargar la fuente
    if (!loadFont()) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
    }
    
    // Cargar la textura
    if (!texture.loadFromFile("assets/images/fondos/pantalla_de_carga.png")) {
        std::cerr << "Error: No se pudo cargar la imagen de splash" << std::endl;
    }
    
    // Configurar el sprite
    sprite = std::make_shared<sf::Sprite>(texture);
    float scaleX = 1200.0f / texture.getSize().x;
    float scaleY = 700.0f / texture.getSize().y;
    sprite->setScale(sf::Vector2f(scaleX, scaleY));
    
    // Configurar el texto
    continueText = std::make_shared<sf::Text>(*font, "Presiona enter para continuar", 30);
    continueText->setFillColor(sf::Color::White);
    float textWidth = continueText->getLocalBounds().size.x;
    continueText->setPosition(sf::Vector2f((1200 - textWidth) / 2, 600));
    
    // === CARGAR Y REPRODUCIR LA MÚSICA ===
    // IMPORTANTE: Pon tu canción en la carpeta assets
    if (!splashMusic.openFromFile("assets/sounds/splash_music.ogg")) {
        std::cerr << "Error: No se pudo cargar assets/sounds/splash_music.ogg" << std::endl;
    } else {
        splashMusic.setLooping(true);         // Para que se repita infinitamente
        splashMusic.setVolume(70.0f);      // Volumen del 0 al 100
        splashMusic.play();                // Inicia la música inmediatamente
    }
}

SplashScreen::~SplashScreen() {
    splashMusic.stop(); // Por seguridad, la apaga si destruyes la pantalla
}

void SplashScreen::update() {
    if (fadeIn) {
        textAlpha += alphaDelta;
        if (textAlpha >= 255) {
            textAlpha = 255;
            fadeIn = false;
        }
    } else {
        textAlpha -= alphaDelta;
        if (textAlpha <= 100) {
            textAlpha = 100;
            fadeIn = true;
        }
    }
    
    sf::Color textColor = sf::Color::White;
    textColor.a = static_cast<unsigned char>(textAlpha);
    continueText->setFillColor(textColor);
}

void SplashScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    if (sprite) {
        window.draw(*sprite);
    }
    if (continueText) {
        window.draw(*continueText);
    }
    
    window.display();
}

void SplashScreen::stopMusic() {
    splashMusic.stop(); // Llama a esto desde Game.cpp cuando presiones la tecla
}