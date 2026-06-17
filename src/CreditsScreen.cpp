#include "CreditsScreen.h"
#include <iostream>

bool CreditsScreen::loadFont() {
    font = std::make_shared<sf::Font>();
    
    // Prioridad 1: Tu carpeta assets. Prioridad 2 y 3: Fuentes del sistema operativo.
    std::vector<std::string> fontPaths = {
        "assets/fonts/arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };
    
    for (const auto& path : fontPaths) {
        if (font->openFromFile(path)) {
            return true;
        }
    }
    
    return false;
}

CreditsScreen::CreditsScreen() {
    // Cargar la fuente
    if (!loadFont()) {
        std::cerr << "Error: No se pudo cargar la fuente en la pantalla de créditos" << std::endl;
    }
    
    // Título
    titleText = std::make_shared<sf::Text>(*font, "CREDITOS", 60);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setPosition(sf::Vector2f(450, 150));
    
    // Créditos
    std::vector<std::pair<std::string, std::string>> credits = {
        {"Joel Munoz", "25110302"},
        {"Luis Hernandez", "25110256"}
    };
    
    for (size_t i = 0; i < credits.size(); ++i) {
        // Nombre
        sf::Text nameText(*font, credits[i].first, 40);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(sf::Vector2f(300, 300 + i * 100));
        creditTexts.push_back(nameText);
        
        // Número
        sf::Text idText(*font, credits[i].second, 35);
        idText.setFillColor(sf::Color::Cyan);
        idText.setPosition(sf::Vector2f(700, 310 + i * 100));
        creditTexts.push_back(idText);
    }
}

CreditsScreen::~CreditsScreen() {
}

void CreditsScreen::update() {
    // Lógica de actualización si es necesaria
}

#include <optional>

void CreditsScreen::render(sf::RenderWindow& window) {
    // 1. CARGA DE RECURSOS
    static sf::Texture bgTexture;
    static std::optional<sf::Sprite> bgSprite;
    static bool bgLoaded = false;

    if (!bgLoaded) {
        // Intenta cargar la imagen
        if (bgTexture.loadFromFile("assets/images/fondos/bg_credits.jpg")) { 
            bgSprite.emplace(bgTexture);
            sf::FloatRect bounds = bgSprite->getLocalBounds();
            bgSprite->setScale(sf::Vector2f({1200.0f / bounds.size.x, 700.0f / bounds.size.y}));
        } else {
            // 🚨 SI FALLA, TE AVISARÁ AQUÍ EN LA TERMINAL
            std::cout << "ERROR: No se encontro 'assets/bg_credits.jpeg'. Revisa el nombre y extension." << std::endl;
        }
        bgLoaded = true;
    }

    // 2. DIBUJO DE FONDO
    if (bgSprite.has_value()) {
        window.clear(sf::Color::Black);
        window.draw(bgSprite.value());
    } else {
        // Si no cargo la imagen, pintamos un fondo gris oscuro para poder ver el recuadro
        window.clear(sf::Color(50, 50, 50)); 
    }

    // 3. DIBUJAR RECUADRO OSCURO
    sf::RectangleShape creditsBox;
    creditsBox.setSize(sf::Vector2f({800.0f, 500.0f}));
    creditsBox.setPosition(sf::Vector2f({200.0f, 100.0f})); 
    creditsBox.setFillColor(sf::Color(0, 0, 0, 180)); 
    window.draw(creditsBox);
    
    // 4. DIBUJAR TEXTOS DE CRÉDITOS
    window.draw(*titleText); 
    
    for (const auto& credit : creditTexts) {
        window.draw(credit);
    }
    
    window.display();
}