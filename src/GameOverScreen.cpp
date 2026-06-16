#include "GameOverScreen.h"
#include <iostream>
#include <sstream> // Para formatear el texto
#include <iomanip> // Para limitar los decimales

GameOverScreen::GameOverScreen() 
    : font(std::make_shared<sf::Font>()), 
      titleText(*font),                   
      winnerText(*font),
      p1StatsText(*font),  // Inicializamos el texto del J1
      p2StatsText(*font),  // Inicializamos el texto del J2
      playAgainText(*font), 
      exitText(*font) 
{
    selectedOption = 0;
    optionConfirmed = false;
    
    loadFont();

    // Título (Lo subimos un poco a Y=50)
    titleText.setString(L"¡FIN DE LA CARRERA!");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({180.0f, 50.0f}); 

    // Texto del ganador (Lo subimos un poco a Y=120)
    winnerText.setCharacterSize(40);
    winnerText.setFillColor(sf::Color::White);
    winnerText.setPosition({250.0f, 120.0f});

    // Estadísticas Jugador 1 (Lado izquierdo)
    p1StatsText.setCharacterSize(22);
    p1StatsText.setFillColor(sf::Color::Cyan);
    p1StatsText.setPosition({150.0f, 200.0f});

    // Estadísticas Jugador 2 (Lado derecho)
    p2StatsText.setCharacterSize(22);
    p2StatsText.setFillColor(sf::Color::Magenta);
    p2StatsText.setPosition({450.0f, 200.0f});

    // Opciones del menú (Las bajamos un poquito)
    playAgainText.setString("Jugar de nuevo");
    playAgainText.setCharacterSize(30);
    playAgainText.setPosition({300.0f, 400.0f});

    exitText.setString("Salir del juego");
    exitText.setCharacterSize(30);
    exitText.setPosition({300.0f, 460.0f});

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

// --- NUEVA FUNCIÓN DE ESTADÍSTICAS ---
void GameOverScreen::setPlayerStats(const std::vector<float>& p1Laps, const std::vector<float>& p2Laps) {
    std::ostringstream stats1, stats2;
    
    // Formatear tiempos del J1
    stats1 << "Tiempos Jugador 1:\n\n";
    for (size_t i = 0; i < p1Laps.size(); ++i) {
        stats1 << "Vuelta " << (i + 1) << ": " 
               << std::fixed << std::setprecision(2) << p1Laps[i] << " s\n";
    }
    p1StatsText.setString(stats1.str());

    // Formatear tiempos del J2
    stats2 << "Tiempos Jugador 2:\n\n";
    for (size_t i = 0; i < p2Laps.size(); ++i) {
        stats2 << "Vuelta " << (i + 1) << ": " 
               << std::fixed << std::setprecision(2) << p2Laps[i] << " s\n";
    }
    p2StatsText.setString(stats2.str());
}

void GameOverScreen::updateDisplay() {
    playAgainText.setFillColor(sf::Color::White);
    exitText.setFillColor(sf::Color::White);

    if (selectedOption == 0) {
        playAgainText.setFillColor(sf::Color::Green);
    } else if (selectedOption == 1) {
        exitText.setFillColor(sf::Color::Green);
    }
}

void GameOverScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (!optionConfirmed) {
            if (keyEvent->code == sf::Keyboard::Key::Down || keyEvent->code == sf::Keyboard::Key::S) {
                selectedOption = 1;
            }
            else if (keyEvent->code == sf::Keyboard::Key::Up || keyEvent->code == sf::Keyboard::Key::W) {
                selectedOption = 0;
            }
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
    window.draw(p1StatsText); // Dibujamos tiempos J1
    window.draw(p2StatsText); // Dibujamos tiempos J2
    window.draw(playAgainText);
    window.draw(exitText);
}

void GameOverScreen::reset() {
    selectedOption = 0;
    optionConfirmed = false;
    updateDisplay();
}