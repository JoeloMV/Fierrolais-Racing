#include "CharacterSelectionScreen.h"
#include <iostream>
#include <sstream>

bool CharacterSelectionScreen::loadFont() {
    std::vector<std::string> fontPaths = {
        "assets/arial.ttf", // Siempre es bueno buscar en tu carpeta primero
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };
    
    for (const auto& path : fontPaths) {
        if (font.openFromFile(path)) {
            return true;
        }
    }
    
    return false;
}

void CharacterSelectionScreen::initializeCharacters() {
    characters.clear();
    std::vector<std::string> names = {"Mecha\nCorta", "Alucin", "Checo\nPerez", "Licenciado", "Poeta", "Programador", "Salta\nmontes", "Vaquero", "Chenms\nMamado", "Fierrolais"};
    
    for (int i = 0; i < 10; ++i) { 
        Character c;
        c.id = i;
        c.name = names[i];
        characters.push_back(c);
    }
}

// ¡Magia aquí! Le pasamos la 'font' a los textos justo antes de que el constructor arranque
CharacterSelectionScreen::CharacterSelectionScreen() 
    : player1Selection(0), player2Selection(0), 
      player1Confirmed(false), player2Confirmed(false),
      titleText(font), player1StatusText(font), player2StatusText(font) {
    
    if (!loadFont()) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
    }
    
    initializeCharacters();
    
    titleText.setString("SELECCIONA TU PERSONAJE");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(250, 50));
    
    // Crear textos para cada personaje acomodados en la cuadrícula
    for (int i = 0; i < 10; ++i) {
        characterTexts.emplace_back(font); // Nace pegado a la fuente
        characterTexts[i].setString(characters[i].name);
        characterTexts[i].setCharacterSize(18); 
        float posX = 100.0f + (i % 5) * 140.0f; 
        float posY = 150.0f + (i / 5) * 180.0f; 
        
        characterTexts[i].setPosition(sf::Vector2f(posX, posY + 110.0f)); 
        characterTexts[i].setFillColor(sf::Color::White);
    }
    
    // Textos de estado
    player1StatusText.setString("");
    player1StatusText.setCharacterSize(20);
    player1StatusText.setFillColor(sf::Color::Blue);
    player1StatusText.setPosition(sf::Vector2f(50, 550)); 
    
    player2StatusText.setString("");
    player2StatusText.setCharacterSize(20);
    player2StatusText.setFillColor(sf::Color::Red);
    player2StatusText.setPosition(sf::Vector2f(800, 550)); 
    
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
    for (int i = 0; i < 10; ++i) { 
        if (i == player1Selection || i == player2Selection) {
            if (i == player1Selection && i == player2Selection) {
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
    
    std::string p1Status = player1Name + (player1Confirmed ? " (Confirmado)" : "");
    player1StatusText.setString(p1Status);
    
    std::string p2Status = player2Name + (player2Confirmed ? " (Confirmado)" : "");
    player2StatusText.setString(p2Status);
}

void CharacterSelectionScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        
        // Controles Jugador 1 (WASD) 
        if (!player1Confirmed) {
            if (keyEvent->code == sf::Keyboard::Key::W && player1Selection >= 5) player1Selection -= 5; 
            if (keyEvent->code == sf::Keyboard::Key::S && player1Selection < 5) player1Selection += 5;  
            if (keyEvent->code == sf::Keyboard::Key::A && (player1Selection % 5) > 0) player1Selection--; 
            if (keyEvent->code == sf::Keyboard::Key::D && (player1Selection % 5) < 4) player1Selection++; 
            updateDisplay();
        }
        
        // Controles Jugador 2 (Flechas) 
        if (!player2Confirmed) {
            if (keyEvent->code == sf::Keyboard::Key::Up && player2Selection >= 5) player2Selection -= 5;
            if (keyEvent->code == sf::Keyboard::Key::Down && player2Selection < 5) player2Selection += 5;
            if (keyEvent->code == sf::Keyboard::Key::Left && (player2Selection % 5) > 0) player2Selection--;
            if (keyEvent->code == sf::Keyboard::Key::Right && (player2Selection % 5) < 4) player2Selection++;
            updateDisplay();
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
    static sf::Texture alucinTexture; 
    static sf::Texture mechaTexture; 
    static sf::Texture checoTexture; 
    static sf::Texture licenciadoTexture; 
    static sf::Texture poetaTexture; 
    static sf::Texture programadorTexture; 
    static sf::Texture saltamontesTexture; 
    static sf::Texture vaqueroTexture; 
    static sf::Texture mamadoTexture; 
    static sf::Texture fierrolaisTexture; 
    static bool textureLoaded = false;

    if (!textureLoaded) {
        alucinTexture.loadFromFile("assets/alucin.png");
        mechaTexture.loadFromFile("assets/mecha corta.jpeg");  
        checoTexture.loadFromFile("assets/checo perez.png"); 
        licenciadoTexture.loadFromFile("assets/licenciado.jpeg"); 
        poetaTexture.loadFromFile("assets/poeta.png");
        programadorTexture.loadFromFile("assets/programador.png");
        saltamontesTexture.loadFromFile("assets/Salta Montes.jpeg");
        vaqueroTexture.loadFromFile("assets/Vaquero.png");
        mamadoTexture.loadFromFile("assets/mamado.png");
        fierrolaisTexture.loadFromFile("assets/fierrolais.jpeg");
        textureLoaded = true;
    }
    
    window.clear(sf::Color::Black);
    
    // Ahora los textos se dibujan directo, sin asterisco (*)
    window.draw(titleText);
    
    for (int i = 0; i < 10; ++i) { 
        float posX = 100.0f + (i % 5) * 140.0f; 
        float posY = 150.0f + (i / 5) * 180.0f; 
        
        sf::RectangleShape box(sf::Vector2f(100.0f, 100.0f)); 
        box.setPosition(sf::Vector2f(posX, posY));
        
        if (i == player1Selection && i == player2Selection) {
            box.setFillColor(sf::Color::Cyan);  
        } else if (i == player1Selection) {
            box.setFillColor(sf::Color::Blue);  
        } else if (i == player2Selection) {
            box.setFillColor(sf::Color::Red);   
        } else {
            box.setFillColor(sf::Color(50, 50, 50));  
        }
        
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);
        
        window.draw(box);
        
        if (i == 0 && textureLoaded) {
            sf::Sprite mechaSprite(mechaTexture); 
            mechaSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            mechaSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(mechaSprite);
        }
        if (i == 1 && textureLoaded) {
            sf::Sprite alucinSprite(alucinTexture); 
            alucinSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            alucinSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(alucinSprite);
        }
        if (i == 2 && textureLoaded) {
            sf::Sprite checoSprite(checoTexture); 
            checoSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            checoSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(checoSprite);
        }
        if (i == 3 && textureLoaded) {
            sf::Sprite licenciadoSprite(licenciadoTexture); 
            licenciadoSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            licenciadoSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(licenciadoSprite);
        }
        if (i == 4 && textureLoaded) {
            sf::Sprite poetaSprite(poetaTexture); 
            poetaSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            poetaSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(poetaSprite);
        }
        if (i == 5 && textureLoaded) {
            sf::Sprite programadorSprite(programadorTexture); 
            programadorSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            programadorSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(programadorSprite);
        }
        if (i == 6 && textureLoaded) {
            sf::Sprite saltamontesSprite(saltamontesTexture); 
            saltamontesSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            saltamontesSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(saltamontesSprite);
        }
        if (i == 7 && textureLoaded) {
            sf::Sprite vaqueroSprite(vaqueroTexture); 
            vaqueroSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            vaqueroSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(vaqueroSprite);
        }
        if (i == 8 && textureLoaded) {
            sf::Sprite mamadoSprite(mamadoTexture); 
            mamadoSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            mamadoSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(mamadoSprite);
        }
        if (i == 9 && textureLoaded) {
            sf::Sprite fierrolaisSprite(fierrolaisTexture); 
            fierrolaisSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
            fierrolaisSprite.setPosition(sf::Vector2f(posX, posY)); 
            window.draw(fierrolaisSprite);
        }  
    }
    
    for (const auto& charText : characterTexts) {
        window.draw(charText);
    }
    
    window.draw(player1StatusText);
    window.draw(player2StatusText);
    
    // Instrucciones
    sf::Text instructions(font);
    instructions.setString("P1: WASD/Enter | P2: Flechas/Enter");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Green);
    instructions.setPosition(sf::Vector2f(250, 650));
    window.draw(instructions);
    
    window.display();
}