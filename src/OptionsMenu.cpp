#include "OptionsMenu.h"
#include <iostream>

bool OptionsMenu::loadFont() {
    std::vector<std::string> fontPaths = {
        "assets/arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };
    for (const auto& path : fontPaths) {
        // Al ya no ser puntero, usamos punto (.) en vez de flecha (->)
        if (font.openFromFile(path)) return true;
    }
    return false;
}

// ¡Aquí está la magia! Le pasamos la fuente a titleText desde que nace
OptionsMenu::OptionsMenu() : titleText(font) {
    if (!loadFont()) {
        std::cerr << "Error cargando fuente en OptionsMenu" << std::endl;
    }
    
    musicVolume = 50.f; 
    sfxVolume = 50.f;
    selectedItemIndex = 0;
    
    // Título
    titleText.setString("OPCIONES");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(sf::Vector2f(450, 100));
    
    // Como no podemos crear textos vacíos, los creamos 1 por 1 pasándoles la fuente
    for(int i = 0; i < 3; ++i) {
        menuOptions.emplace_back(font);
        menuOptions[i].setCharacterSize(40);
        menuOptions[i].setPosition(sf::Vector2f(350, 250 + (i * 100)));
    }
    
    updateTextDisplays();
}

OptionsMenu::~OptionsMenu() {}

void OptionsMenu::updateTextDisplays() {
    menuOptions[0].setString("Volumen Musica: < " + std::to_string((int)musicVolume) + "% >");
    menuOptions[1].setString("Volumen Efectos: < " + std::to_string((int)sfxVolume) + "% >");
    menuOptions[2].setString("Volver al Menu (ESC)");
    
    for (int i = 0; i < 3; ++i) {
        if (i == selectedItemIndex) {
            menuOptions[i].setFillColor(sf::Color::Red); 
        } else {
            menuOptions[i].setFillColor(sf::Color::White); 
        }
    }
}

void OptionsMenu::moveUp() {
    if (selectedItemIndex > 0) {
        selectedItemIndex--;
        updateTextDisplays();
    }
}

void OptionsMenu::moveDown() {
    if (selectedItemIndex < 2) {
        selectedItemIndex++;
        updateTextDisplays();
    }
}

void OptionsMenu::increaseVolume() {
    if (selectedItemIndex == 0 && musicVolume < 100.f) {
        musicVolume += 5.f;
    } else if (selectedItemIndex == 1 && sfxVolume < 100.f) {
        sfxVolume += 5.f;
    }
    updateTextDisplays();
}

void OptionsMenu::decreaseVolume() {
    if (selectedItemIndex == 0 && musicVolume > 0.f) {
        musicVolume -= 5.f;
    } else if (selectedItemIndex == 1 && sfxVolume > 0.f) {
        sfxVolume -= 5.f;
    }
    updateTextDisplays();
}

float OptionsMenu::getMusicVolume() const { return musicVolume; }
float OptionsMenu::getSfxVolume() const { return sfxVolume; }

#include <optional> // Asegúrate de incluir esto al principio de tu archivo

void OptionsMenu::render(sf::RenderWindow& window) {
    // 1. CARGA DE RECURSOS
    static sf::Texture bgTexture;
    static std::optional<sf::Sprite> bgSprite;
    static bool bgLoaded = false;

    if (!bgLoaded) {
        if (bgTexture.loadFromFile("assets/bg_options.jpg")) {
            bgSprite.emplace(bgTexture);
            sf::FloatRect bounds = bgSprite->getLocalBounds();
            // Ajustar a 1200x700
            bgSprite->setScale(sf::Vector2f({1200.0f / bounds.size.x, 700.0f / bounds.size.y}));
        }
        bgLoaded = true;
    }

    // 2. DIBUJO
    window.clear(sf::Color::Black);
    
    // Dibujamos el fondo primero
    if (bgSprite.has_value()) {
        window.draw(bgSprite.value());
    }

    // 3. DIBUJAR RECUADRO OSCURO
    sf::RectangleShape optionsBox;
    // Tamaño del recuadro (puedes ajustar estos valores si tus textos son muy anchos/altos)
    optionsBox.setSize(sf::Vector2f({800.0f, 500.0f}));
    // Posición centrada (1200 - 800)/2 = 200 en X, y (700 - 500)/2 = 100 en Y
    optionsBox.setPosition(sf::Vector2f({200.0f, 100.0f})); 
    optionsBox.setFillColor(sf::Color(0, 0, 0, 180)); // Negro con 180 de opacidad (transparente)
    window.draw(optionsBox);
    
    // 4. DIBUJAR TEXTOS (Van encima del recuadro oscuro)
    window.draw(titleText);
    for (const auto& option : menuOptions) {
        window.draw(option);
    }
    
    window.display();
}