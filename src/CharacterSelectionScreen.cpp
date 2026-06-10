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
    // 1. Aquí están los 10 nombres de los personajes
    std::vector<std::string> names = {"Mecha\nCorta", "Alucin", "Checo\nPerez", "Licenciado", "Poeta", "Programador", "Salta\nmontes", "Vaquero", "Chenms\nMamado", "Fierrolais"};
    
    for (int i = 0; i < 10; ++i) { // Cambiado a 10
        Character c;
        c.id = i;
        c.name = names[i];
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
    
    // Crear textos para cada personaje acomodados en la cuadrícula
    for (int i = 0; i < 10; ++i) {
        sf::Text charText(*font, characters[i].name, 18); // Letra un poco más chica para que quepa
        float posX = 100.0f + (i % 5) * 140.0f; // 5 columnas
        float posY = 150.0f + (i / 5) * 180.0f; // 2 filas
        
        charText.setPosition(sf::Vector2f(posX, posY + 110.0f)); // Texto abajito del cuadro
        charText.setFillColor(sf::Color::White);
        characterTexts.push_back(charText);
    }
    
    // Textos de estado
    player1StatusText = std::make_shared<sf::Text>(*font, "", 20);
    player1StatusText->setFillColor(sf::Color::Blue);
    player1StatusText->setPosition(sf::Vector2f(50, 550)); // Movido más abajo para que no estorbe
    
    player2StatusText = std::make_shared<sf::Text>(*font, "", 20);
    player2StatusText->setFillColor(sf::Color::Red);
    player2StatusText->setPosition(sf::Vector2f(800, 550)); // Movido más abajo
    
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
    for (int i = 0; i < 10; ++i) { // Cambiado a 10
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
    player1StatusText->setString(p1Status);
    
    std::string p2Status = player2Name + (player2Confirmed ? " (Confirmado)" : "");
    player2StatusText->setString(p2Status);
}

void CharacterSelectionScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        
        // Controles Jugador 1 (WASD) - Ahora en cuadrícula
        if (!player1Confirmed) {
            if (keyEvent->code == sf::Keyboard::Key::W && player1Selection >= 5) player1Selection -= 5; // Sube fila
            if (keyEvent->code == sf::Keyboard::Key::S && player1Selection < 5) player1Selection += 5;  // Baja fila
            if (keyEvent->code == sf::Keyboard::Key::A && (player1Selection % 5) > 0) player1Selection--; // Izquierda
            if (keyEvent->code == sf::Keyboard::Key::D && (player1Selection % 5) < 4) player1Selection++; // Derecha
            updateDisplay();
        }
        
        // Controles Jugador 2 (Flechas) - Ahora en cuadrícula
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
   static sf::Texture alucinTexture; // Eliminamos el static sf::Sprite de aquí
   static sf::Texture mechaTexture; // Nueva textura para Mecha Corta
    static bool textureLoaded = false;

    if (!textureLoaded) {
     alucinTexture.loadFromFile("assets/alucin.png");
        mechaTexture.loadFromFile("assets/mecha corta.jpeg");  
            textureLoaded = true;
        
    }
    window.clear(sf::Color::Black);
    
    if (titleText) {
        window.draw(*titleText);
    }
    
    // Dibujar los 10 personajes en cuadrícula
    for (int i = 0; i < 10; ++i) { // Cambiado a 10
        float posX = 100.0f + (i % 5) * 140.0f; // Matemáticas de las columnas
        float posY = 150.0f + (i / 5) * 180.0f; // Matemáticas de las filas
        
        sf::RectangleShape box(sf::Vector2f(100.0f, 100.0f)); // Tamaño fijo del cuadro
        box.setPosition(sf::Vector2f(posX, posY));
        
        // Color según selección
        if (i == player1Selection && i == player2Selection) {
            box.setFillColor(sf::Color::Cyan);  // Ambos apuntan
        } else if (i == player1Selection) {
            box.setFillColor(sf::Color::Blue);  // Solo P1
        } else if (i == player2Selection) {
            box.setFillColor(sf::Color::Red);   // Solo P2
        } else {
            box.setFillColor(sf::Color(50, 50, 50));  // Gris oscuro para los no seleccionados
        }
        
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);
        
        window.draw(box);
    // Si es el cuadro 0 (Mecha Corta) y la textura cargó bien
    if (i == 0 && textureLoaded) {
        sf::Sprite mechaSprite(mechaTexture); 
        mechaSprite.setScale(sf::Vector2f(0.5f, 0.5f)); 
        mechaSprite.setPosition(sf::Vector2f(posX, posY)); 
        window.draw(mechaSprite);
    }
    // Si es el cuadro 1 (Alucin) y la textura cargó bien, lo dibujamos
    if (i == 1 && textureLoaded) {
        sf::Sprite alucinSprite(alucinTexture); // Creamos el sprite directamente con su textura
        alucinSprite.setScale(sf::Vector2f(0.5f, 0.5f)); // Usamos sf::Vector2f para la escala
        alucinSprite.setPosition(sf::Vector2f(posX, posY)); // Usamos sf::Vector2f para la posición
        window.draw(alucinSprite);
    }
    }
    
    // Dibujar texto de personajes
    for (const auto& charText : characterTexts) {
        window.draw(charText);
    }
    
    if (player1StatusText) window.draw(*player1StatusText);
    if (player2StatusText) window.draw(*player2StatusText);
    
    // Instrucciones
    sf::Text instructions(*font, "P1: WASD/Enter | P2: Flechas/Enter", 20);
    instructions.setFillColor(sf::Color::Green);
    instructions.setPosition(sf::Vector2f(250, 650));
    window.draw(instructions);
    
    window.display();
}