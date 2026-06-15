#include "GameOverScreen.h"
#include <iostream>

GameOverScreen::GameOverScreen() 
    : font(std::make_shared<sf::Font>()), // 1. Inicializamos la fuente en memoria
      titleText(*font),                   // 2. Pasamos la fuente a cada texto
      winnerText(*font), 
      playAgainText(*font), 
      exitText(*font) 
{
    selectedOption = 0;
    optionConfirmed = false;
    
    // Cargamos el archivo de la fuente
    loadFont();

    // Configuración del título principal
    titleText.setString("FIN DE LA CARRERA!!");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({200.0f, 100.0f});

    // Configuración del texto del ganador
    winnerText.setCharacterSize(40);
    winnerText.setFillColor(sf::Color::White);
    winnerText.setPosition({250.0f, 200.0f});

    // Opciones del menú
    playAgainText.setString("Jugar de nuevo");
    playAgainText.setCharacterSize(30);
    playAgainText.setPosition({350.0f, 350.0f});

    exitText.setString("Salir del juego");
    exitText.setCharacterSize(30);
    exitText.setPosition({350.0f, 420.0f});

    updateDisplay();
}

GameOverScreen::~GameOverScreen() {}

bool GameOverScreen::loadFont() {
    if (!font->openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error al cargar la fuente en GameOverScreen\n";
        return false;
    }
    return true;
}

void GameOverScreen::setWinner(const std::string& winnerName) {
    winnerText.setString("Ganador: " + winnerName);
}

void GameOverScreen::updateDisplay() {
    // Reseteamos el color a blanco
    playAgainText.setFillColor(sf::Color::White);
    exitText.setFillColor(sf::Color::White);

    // Pintamos de verde la opción seleccionada
    if (selectedOption == 0) {
        playAgainText.setFillColor(sf::Color::Green);
    } else if (selectedOption == 1) {
        exitText.setFillColor(sf::Color::Green);
    }
}

void GameOverScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (!optionConfirmed) {
            // Flecha Abajo o S
            if (keyEvent->code == sf::Keyboard::Key::Down || keyEvent->code == sf::Keyboard::Key::S) {
                selectedOption = 1;
            }
            // Flecha Arriba o W
            else if (keyEvent->code == sf::Keyboard::Key::Up || keyEvent->code == sf::Keyboard::Key::W) {
                selectedOption = 0;
            }
            // Confirmar con Enter o Espacio
            else if (keyEvent->code == sf::Keyboard::Key::Enter || keyEvent->code == sf::Keyboard::Key::Space) {
                optionConfirmed = true;
            }
        }
    }
    updateDisplay();
}

void GameOverScreen::render(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(winnerText);
    window.draw(playAgainText);
    window.draw(exitText);
}

void GameOverScreen::reset() {
    selectedOption = 0;
    optionConfirmed = false;
    updateDisplay();
}