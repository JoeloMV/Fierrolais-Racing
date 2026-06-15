#include "NameInputScreen.h"
#include <iostream>

bool NameInputScreen::loadFont() {
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

NameInputScreen::NameInputScreen() : currentPlayer(1), enterPressed(false){
    if (!loadFont()) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
    }
    
    titleText = std::make_shared<sf::Text>(*font, "", 50);
    titleText->setPosition(sf::Vector2f(250, 150));
    
    inputText = std::make_shared<sf::Text>(*font, "", 40);
    inputText->setPosition(sf::Vector2f(300, 300));
    
    instructionText = std::make_shared<sf::Text>(*font, "Presiona ENTER para confirmar", 25);
    instructionText->setFillColor(sf::Color::Cyan);
    instructionText->setPosition(sf::Vector2f(250, 600));
}

NameInputScreen::~NameInputScreen() {
}

void NameInputScreen::reset(int player) {
    currentPlayer = player;
    playerName.clear();
    enterPressed = false;
    updateDisplay();
}

void NameInputScreen::updateDisplay() {
    titleText->setString("Jugador " + std::to_string(currentPlayer) + " - Ingresa tu nombre");
    if (currentPlayer == 1) {
        titleText->setFillColor(sf::Color::Blue);
    } else {
        titleText->setFillColor(sf::Color::Red);
    }
    
    inputText->setString(playerName);
    if (currentPlayer == 1) {
        inputText->setFillColor(sf::Color::Blue);
    } else {
        inputText->setFillColor(sf::Color::Red);
    }
}

void NameInputScreen::handleInput(const sf::Event& event) {
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        char c = static_cast<char>(textEvent->unicode);
        
        // Permitir solo caracteres alfanuméricos y espacios
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
            (c >= '0' && c <= '9') || c == ' ') {
            if (playerName.length() < MAX_NAME_LENGTH) {
                playerName += c;
                updateDisplay();
            }
        }
    } else if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Backspace) {
            if (!playerName.empty()) {
                playerName.pop_back();
                updateDisplay();
            }
        } 
        else if (keyEvent->code == sf::Keyboard::Key::Enter) {
            if (!playerName.empty()) {
                enterPressed = true; // Abrimos el candado
            }
        }
    }
}

bool NameInputScreen::isNameConfirmed() const {
    return enterPressed; // Retorna si presionaste Enter
}

void NameInputScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    if (titleText) window.draw(*titleText);
    if (inputText) window.draw(*inputText);
    if (instructionText) window.draw(*instructionText);
    
    window.display();
}