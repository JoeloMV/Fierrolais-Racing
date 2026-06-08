#include "OptionsMenu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

bool OptionsMenu::loadFont() {
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



OptionsMenu::OptionsMenu() 
    : selectedIndex(0), normalColor(sf::Color::White), selectedColor(sf::Color::Yellow),
      masterVolume(80.0f), musicVolume(70.0f), sharedBackgroundMusic(nullptr) {
    
    // Cargar la fuente
    if (!loadFont()) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
    }
    
    // Título
    titleText = std::make_shared<sf::Text>(*font, "OPCIONES", 60);
    titleText->setFillColor(selectedColor);
    titleText->setPosition(sf::Vector2f(450, 100));
    
    // Etiquetas de opciones
    std::vector<std::string> labels = {
        "Volumen General",
        "Volumen Musica"
    };
    
    for (size_t i = 0; i < labels.size(); ++i) {
        sf::Text labelText(*font, labels[i], 35);
        labelText.setPosition(sf::Vector2f(250, 250 + i * 100));
        labelText.setFillColor((i == 0) ? selectedColor : normalColor);
        optionLabels.push_back(labelText);
        
        // Valor correspondiente
        sf::Text valueText(*font, "", 35);
        valueText.setPosition(sf::Vector2f(800, 250 + i * 100));
        valueText.setFillColor((i == 0) ? selectedColor : normalColor);
        optionValues.push_back(valueText);
    }
    
    updateDisplay();
}

OptionsMenu::~OptionsMenu() {
}

void OptionsMenu::updateDisplay() {
    // Actualizar valores mostrados
    std::ostringstream oss;
    
    oss << std::fixed << std::setprecision(0) << masterVolume << "%";
    optionValues[0].setString(oss.str());
    
    oss.str("");
    oss << std::fixed << std::setprecision(0) << musicVolume << "%";
    optionValues[1].setString(oss.str());
}

void OptionsMenu::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Up) {
            if (selectedIndex > 0) {
                optionLabels[selectedIndex].setFillColor(normalColor);
                optionValues[selectedIndex].setFillColor(normalColor);
                selectedIndex--;
                optionLabels[selectedIndex].setFillColor(selectedColor);
                optionValues[selectedIndex].setFillColor(selectedColor);
            }
        } else if (keyEvent->code == sf::Keyboard::Key::Down) {
            if (selectedIndex < static_cast<int>(optionLabels.size()) - 1) {
                optionLabels[selectedIndex].setFillColor(normalColor);
                optionValues[selectedIndex].setFillColor(normalColor);
                selectedIndex++;
                optionLabels[selectedIndex].setFillColor(selectedColor);
                optionValues[selectedIndex].setFillColor(selectedColor);
            }
        } else if (keyEvent->code == sf::Keyboard::Key::Left) {
            // Bajar volumen
            if (selectedIndex == 0) {
                masterVolume = std::max(0.0f, masterVolume - 5.0f);
            } else if (selectedIndex == 1) {
                musicVolume = std::max(0.0f, musicVolume - 5.0f);
                if (sharedBackgroundMusic) {
                    sharedBackgroundMusic->setVolume(musicVolume);
                }
            }
            updateDisplay();
        } else if (keyEvent->code == sf::Keyboard::Key::Right) {
            // Subir volumen
            if (selectedIndex == 0) {
                masterVolume = std::min(100.0f, masterVolume + 5.0f);
            } else if (selectedIndex == 1) {
                musicVolume = std::min(100.0f, musicVolume + 5.0f);
                if (sharedBackgroundMusic) {
                    sharedBackgroundMusic->setVolume(musicVolume);
                }
            }
            updateDisplay();
        }
    }
}

void OptionsMenu::update() {
    // Lógica de actualización si es necesaria
}

void OptionsMenu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    if (titleText) {
        window.draw(*titleText);
    }
    
    for (const auto& label : optionLabels) {
        window.draw(label);
    }
    
    for (const auto& value : optionValues) {
        window.draw(value);
    }
    
    // Mostrar instrucciones
    sf::Text instructions(*font, "Usa <- -> para cambiar | Usa ^v para navegar | ESC para volver", 20);
    instructions.setFillColor(sf::Color::Cyan);
    instructions.setPosition(sf::Vector2f(100, 650));
    window.draw(instructions);
    
    window.display();
}

void OptionsMenu::reset() {
    selectedIndex = 0;
    for (size_t i = 0; i < optionLabels.size(); ++i) {
        optionLabels[i].setFillColor((i == 0) ? selectedColor : normalColor);
        optionValues[i].setFillColor((i == 0) ? selectedColor : normalColor);
    }
}

void OptionsMenu::setBackgroundMusic(std::shared_ptr<sf::Music> music) {
    sharedBackgroundMusic = music;
    if (sharedBackgroundMusic) {
        sharedBackgroundMusic->setVolume(musicVolume);
    }
}

void OptionsMenu::setMasterVolume(float volume) {
    masterVolume = std::max(0.0f, std::min(100.0f, volume));
    updateDisplay();
}

void OptionsMenu::setMusicVolume(float volume) {
    musicVolume = std::max(0.0f, std::min(100.0f, volume));
    if (sharedBackgroundMusic) {
        sharedBackgroundMusic->setVolume(musicVolume);
    }
    updateDisplay();
}
