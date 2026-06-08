#include "CharacterSelectionScreen.h"
#include <iostream>
#include <sstream>

bool CharacterSelectionScreen::loadFont() {
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

void CharacterSelectionScreen::initializeCharacters() {
    characters.clear();
    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        Character c;
        c.id = i;
        c.name = "Personaje " + std::to_string(i + 1);
        characters.push_back(c);
    }
}

CharacterSelectionScreen::CharacterSelectionScreen() 
    : player1Selection(0), player2Selection(0), 
      player1Confirmed(false), player2Confirmed(false) {
    
    if (!loadFont()) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
    }
    
    initializeCharacters();
    
    titleText = std::make_shared<sf::Text>(*font, "SELECCIONA TU PERSONAJE", 40);
    titleText->setFillColor(sf::Color::White);
    titleText->setPosition(sf::Vector2f(250, 50));
    
    // Crear textos para cada personaje
    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        sf::Text charText(*font, characters[i].name, 25);
        charText.setPosition(sf::Vector2f(500, 150 + i * (CHARACTER_BOX_SIZE + CHARACTER_SPACING)));
        charText.setFillColor(sf::Color::White);
        characterTexts.push_back(charText);
    }
    
    // Textos de estado
    player1StatusText = std::make_shared<sf::Text>(*font, "", 20);
    player1StatusText->setFillColor(sf::Color::Blue);
    player1StatusText->setPosition(sf::Vector2f(50, 300));
    
    player2StatusText = std::make_shared<sf::Text>(*font, "", 20);
    player2StatusText->setFillColor(sf::Color::Red);
    player2StatusText->setPosition(sf::Vector2f(1000, 300));
    
    updateDisplay();
}

CharacterSelectionScreen::~CharacterSelectionScreen() {
}

void CharacterSelectionScreen::setPlayerNames(const std::string& p1, const std::string& p2) {
    player1Name = p1;
    player2Name = p2;
    updateDisplay();
}

void CharacterSelectionScreen::reset() {
    player1Selection = 0;
    player2Selection = 0;
    player1Confirmed = false;
    player2Confirmed = false;
    updateDisplay();
}

void CharacterSelectionScreen::updateDisplay() {
    // Actualizar colores de personajes basado en selección
    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        if (i == player1Selection || i == player2Selection) {
            // Este personaje está siendo apuntado
            if (i == player1Selection && i == player2Selection) {
                // Ambos lo apuntan - mostrar ambos colores alternando o algo (por ahora solo el del primero)
                characterTexts[i].setFillColor(sf::Color::Cyan);
            } else if (i == player1Selection) {
                characterTexts[i].setFillColor(sf::Color::Blue);
            } else {
                characterTexts[i].setFillColor(sf::Color::Red);
            }
        } else {
            characterTexts[i].setFillColor(sf::Color::White);
        }
    }
    
    // Actualizar estado de jugadores
    std::string p1Status = player1Name + (player1Confirmed ? " (Confirmado)" : "");
    player1StatusText->setString(p1Status);
    
    std::string p2Status = player2Name + (player2Confirmed ? " (Confirmado)" : "");
    player2StatusText->setString(p2Status);
}

void CharacterSelectionScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Controles Jugador 1 (WASD)
        if (keyEvent->code == sf::Keyboard::Key::W) {
            if (!player1Confirmed && player1Selection > 0) {
                player1Selection--;
                updateDisplay();
            }
        } else if (keyEvent->code == sf::Keyboard::Key::S) {
            if (!player1Confirmed && player1Selection < NUM_CHARACTERS - 1) {
                player1Selection++;
                updateDisplay();
            }
        }
        
        // Controles Jugador 2 (Flechas)
        if (keyEvent->code == sf::Keyboard::Key::Up) {
            if (!player2Confirmed && player2Selection > 0) {
                player2Selection--;
                updateDisplay();
            }
        } else if (keyEvent->code == sf::Keyboard::Key::Down) {
            if (!player2Confirmed && player2Selection < NUM_CHARACTERS - 1) {
                player2Selection++;
                updateDisplay();
            }
        }
        
        // Confirmar (Enter o Space para ambos)
        if (keyEvent->code == sf::Keyboard::Key::Enter || keyEvent->code == sf::Keyboard::Key::Space) {
            if (!player1Confirmed) {
                player1Confirmed = true;
                updateDisplay();
            } else if (!player2Confirmed) {
                player2Confirmed = true;
                updateDisplay();
            }
        }
    }
}

bool CharacterSelectionScreen::areBothConfirmed() const {
    return player1Confirmed && player2Confirmed;
}

void CharacterSelectionScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    if (titleText) {
        window.draw(*titleText);
    }
    
    // Dibujar personajes en el centro
    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        // Dibujar rectángulo
        sf::RectangleShape box(sf::Vector2f(CHARACTER_BOX_SIZE, CHARACTER_BOX_SIZE));
        box.setPosition(sf::Vector2f(450, 150 + i * (CHARACTER_BOX_SIZE + CHARACTER_SPACING)));
        
        // Color según selección
        if (i == player1Selection && i == player2Selection) {
            box.setFillColor(sf::Color::Cyan);  // Ambos apuntan
        } else if (i == player1Selection) {
            box.setFillColor(sf::Color::Blue);  // Solo P1
        } else if (i == player2Selection) {
            box.setFillColor(sf::Color::Red);   // Solo P2
        } else {
            box.setFillColor(sf::Color(50, 50, 50));  // Gris oscuro
        }
        
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);
        
        window.draw(box);
    }
    
    // Dibujar texto de personajes
    for (const auto& charText : characterTexts) {
        window.draw(charText);
    }
    
    // Preview P1 (izquierda)
    if (player1StatusText) {
        window.draw(*player1StatusText);
    }
    
    // Preview P2 (derecha)
    if (player2StatusText) {
        window.draw(*player2StatusText);
    }
    
    // Instrucciones
    sf::Text instructions(*font, "P1: WASD/Enter | P2: Flechas/Enter", 20);
    instructions.setFillColor(sf::Color::Green);
    instructions.setPosition(sf::Vector2f(250, 650));
    window.draw(instructions);
    
    window.display();
}
