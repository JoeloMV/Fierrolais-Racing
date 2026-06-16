#include "SplashScreen.h"
#include <iostream>

bool SplashScreen::loadFont() {
    font = std::make_shared<sf::Font>();
    
    // Buscar la fuente directamente en tu carpeta de assets
    if (font->openFromFile("assets/arial.ttf")) {
        std::cout << "Fuente cargada exitosamente desde: assets/arial.ttf" << std::endl;
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
    if (!texture.loadFromFile("assets/images/pantalla_de_carga.png")) {
        std::cerr << "Error: No se pudo cargar la imagen de splash" << std::endl;
    }
    
    // Configurar el sprite
    sprite = std::make_shared<sf::Sprite>(texture);
    
    // Ajustar el sprite para que llene la ventana (1200x700)
    float scaleX = 1200.0f / texture.getSize().x;
    float scaleY = 700.0f / texture.getSize().y;
    sprite->setScale(sf::Vector2f(scaleX, scaleY));
    
    // Configurar el texto
    continueText = std::make_shared<sf::Text>(*font, "Presiona cualquier tecla para continuar", 30);
    continueText->setFillColor(sf::Color::White);
    
    // Centrar el texto horizontalmente y posicionarlo en la parte inferior
    float textWidth = continueText->getLocalBounds().size.x;
    continueText->setPosition(sf::Vector2f((1200 - textWidth) / 2, 600));
}

SplashScreen::~SplashScreen() {
}

void SplashScreen::update() {
    // Animar la opacidad del texto (fade in/out)
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
    
    // Actualizar el color del texto con la nueva opacidad
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