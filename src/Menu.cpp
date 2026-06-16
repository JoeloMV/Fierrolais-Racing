#include "Menu.h"
#include <iostream>

bool Menu::loadFont() {
    font = std::make_shared<sf::Font>();
    // Intentar cargar una fuente del sistema Windows
    std::vector<std::string> fontPaths = {
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"  // Para Linux
    };
    
    for (const auto& path : fontPaths) {
        if (font->openFromFile(path)) {
            std::cout << "Fuente cargada desde: " << path << std::endl;
            return true;
        }
    }
    
    std::cerr << "No se pudo cargar ninguna fuente. El menú no mostrará texto." << std::endl;
    return false;
}

Menu::Menu() : selectedIndex(0), normalColor(sf::Color::White), selectedColor(sf::Color::Yellow), backgroundSprite(nullptr) {
    // Cargar la fuente
    if (!loadFont()) {
        std::cerr << "Advertencia: No se cargó la fuente correctamente" << std::endl;
    }
    
    // Crear y configurar el título del menú
    titleText = std::make_shared<sf::Text>(*font, "FIERROLAIS RACING", 60);
    titleText->setFillColor(selectedColor);
    titleText->setPosition(sf::Vector2f(150, 100));
    
    // Opciones del menú
    std::vector<std::string> options = {
        "INICIAR JUEGO",
        "OPCIONES",
        "CREDITOS",
        "SALIR"
    };
    
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(*font, options[i], 40);
        text.setPosition(sf::Vector2f(250, 250 + i * 80));
        text.setFillColor((i == 0) ? selectedColor : normalColor);
        menuItems.push_back(text);
    }

// === CARGAR EL FONDO DEL MENÚ PRINCIPAL ===
        if (!backgroundTexture.loadFromFile("assets/fondo_menu.png")) {
         std::cerr << "Error: No se pudo cargar assets/fondo_menu.png" << std::endl;
    } else {
        backgroundSprite = new sf::Sprite(backgroundTexture);
    }
}
Menu::~Menu() {
}
void Menu::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Up) {
            if (selectedIndex > 0) {
                menuItems[selectedIndex].setFillColor(normalColor);
                selectedIndex--;
                menuItems[selectedIndex].setFillColor(selectedColor);
            }
        } else if (keyEvent->code == sf::Keyboard::Key::Down) {
            if (selectedIndex < static_cast<int>(menuItems.size()) - 1) {
                menuItems[selectedIndex].setFillColor(normalColor);
                selectedIndex++;
                menuItems[selectedIndex].setFillColor(selectedColor);
            }
        }
    }
}

void Menu::update() {
    // Lógica de actualización del menú si es necesaria
}

void Menu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    // Dibujar el fondo espectacular del Mustang
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    }
    if (titleText) {
        window.draw(*titleText);
    }
    
    for (const auto& item : menuItems) {
        window.draw(item);
    }
    
    window.display();
}

int Menu::getSelectedOption() const {
    return selectedIndex;
}

void Menu::reset() {
    selectedIndex = 0;
    for (size_t i = 0; i < menuItems.size(); ++i) {
        menuItems[i].setFillColor((i == 0) ? selectedColor : normalColor);
    }
}
