#include "CreditsScreen.h"
#include <iostream>

bool CreditsScreen::loadFont() {
    font = std::make_shared<sf::Font>();
    std::vector<std::string> fontPaths = {
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\Arial.ttf",
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
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
    }
    
    // Título
    titleText = std::make_shared<sf::Text>(*font, "CREDITOS", 60);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setPosition(sf::Vector2f(450, 150));
    
    // Créditos
    std::vector<std::pair<std::string, std::string>> credits = {
        {"Joel Muñoz", "25110302"},
        {"Luis Hernandez", "22110256"}
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

void CreditsScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    if (titleText) {
        window.draw(*titleText);
    }
    
    for (const auto& credit : creditTexts) {
        window.draw(credit);
    }
    
    // Mostrar instrucciones
    sf::Text instructions(*font, "Presiona ESC para volver al menu", 20);
    instructions.setFillColor(sf::Color::Green);
    instructions.setPosition(sf::Vector2f(350, 650));
    window.draw(instructions);
    
    window.display();
}
