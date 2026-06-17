#include "Menu.h"
#include <iostream>

bool Menu::loadFont() {
    font = std::make_shared<sf::Font>();
    
    // Ahora busca la fuente directamente en tu carpeta de assets
    if (font->openFromFile("assets/fonts/arial.ttf")) {
        std::cout << "Fuente cargada exitosamente desde: assets/fonts/arial.ttf" << std::endl;
        return true;
    }
    
    std::cerr << "No se pudo cargar la fuente desde assets. El menú no mostrará texto." << std::endl;
    return false;
}

Menu::Menu() : selectedIndex(0), normalColor(sf::Color::White), selectedColor(sf::Color::Yellow), backgroundSprite(nullptr) {
    // Cargar la fuente
    if (!loadFont()) {
        std::cerr << "Advertencia: No se cargó la fuente correctamente" << std::endl;
    }
    
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
/// === CARGAR LOS FONDOS DEL JUEGO ===
    // 1. Intentamos cargar primero el fondo para los nombres
    if (backgroundTexture.loadFromFile("assets/fondo_nombre.png")) {
        backgroundSprite = new sf::Sprite(backgroundTexture);
    } 
    // 2. Si no existe, usamos el fondo del Mustang para el menú principal
    else if (backgroundTexture.loadFromFile("assets/images/fondos/fondo_menu.png")) {
        backgroundSprite = new sf::Sprite(backgroundTexture);
    } 
    // 3. Si no encuentra ninguno, avisa en la consola
    else {
        std::cerr << "Error: No se pudo cargar ningún fondo desde assets." << std::endl;
    }
}
Menu::~Menu() {
}
void Menu::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Mover hacia arriba (Flecha Arriba o tecla W)
        if (keyEvent->code == sf::Keyboard::Key::Up || keyEvent->code == sf::Keyboard::Key::W) {
            if (selectedIndex > 0) {
                menuItems[selectedIndex].setFillColor(normalColor);
                selectedIndex--;
                menuItems[selectedIndex].setFillColor(selectedColor);
            }
        } 
        // Mover hacia abajo (Flecha Abajo o tecla S)
        else if (keyEvent->code == sf::Keyboard::Key::Down || keyEvent->code == sf::Keyboard::Key::S) {
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