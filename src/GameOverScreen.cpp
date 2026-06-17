#include "GameOverScreen.h"
#include <iostream>
#include <sstream> 
#include <iomanip> 

GameOverScreen::GameOverScreen() 
    : font(std::make_shared<sf::Font>()), 
      titleText(*font),                   
      winnerText(*font),
      p1StatsText(*font),  
      p2StatsText(*font),  
      playAgainText(*font), 
      exitText(*font) 
{
    selectedOption = 0;
    optionConfirmed = false;
    
    fontLoaded = loadFont();

    if (bgTexture.loadFromFile("assets/images/fondos/bg_gameover.jpg")) {
        bgSprite.emplace(bgTexture);
        
        sf::FloatRect bounds = bgSprite->getLocalBounds();
        float scale = 1200.0f / bounds.size.x;
        bgSprite->setScale(sf::Vector2f(scale, scale)); 
        
        float scaledHeight = bounds.size.y * scale;
        float yOffset = (700.0f - scaledHeight) / 2.0f; 
        bgSprite->setPosition(sf::Vector2f(0.0f, yOffset));
    }

    overlay.setSize(sf::Vector2f(1000.0f, 600.0f));
    overlay.setFillColor(sf::Color(0, 0, 0, 180)); 
    overlay.setPosition(sf::Vector2f(100.0f, 40.0f)); 

    if (fontLoaded) {
        titleText.setString(L"¡FIN DE LA CARRERA!");
        titleText.setCharacterSize(50);
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setPosition(sf::Vector2f(180.0f, 50.0f)); 

        winnerText.setCharacterSize(40);
        winnerText.setFillColor(sf::Color::White);
        winnerText.setPosition(sf::Vector2f(250.0f, 120.0f));

        p1StatsText.setCharacterSize(22);
        p1StatsText.setFillColor(sf::Color::Cyan);
        p1StatsText.setPosition(sf::Vector2f(150.0f, 200.0f));

        p2StatsText.setCharacterSize(22);
        p2StatsText.setFillColor(sf::Color::Magenta);
        p2StatsText.setPosition(sf::Vector2f(450.0f, 200.0f));

        playAgainText.setString("Jugar de nuevo");
        playAgainText.setCharacterSize(30);
        playAgainText.setPosition(sf::Vector2f(300.0f, 400.0f));

        exitText.setString("Salir del juego");
        exitText.setCharacterSize(30);
        exitText.setPosition(sf::Vector2f(300.0f, 460.0f));

        updateDisplay();
    }
}

GameOverScreen::~GameOverScreen() {}

bool GameOverScreen::loadFont() {
    if (!font->openFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Aviso: No se encontro assets/fonts/arial.ttf. El texto no se dibujara, pero no crasheara.\n";
        return false;
    }
    return true;
}

void GameOverScreen::setWinner(const std::string& winnerName) {
    if (fontLoaded) winnerText.setString("Ganador: " + winnerName);
}

void GameOverScreen::setPlayerStats(const std::vector<float>& p1Laps, const std::vector<float>& p2Laps) {
    if (!fontLoaded) return;
    std::ostringstream stats1, stats2;
    
    stats1 << "Tiempos Jugador 1:\n\n";
    for (size_t i = 0; i < p1Laps.size(); ++i) {
        stats1 << "Vuelta " << (i + 1) << ": " 
               << std::fixed << std::setprecision(2) << p1Laps[i] << " s\n";
    }
    p1StatsText.setString(stats1.str());

    stats2 << "Tiempos Jugador 2:\n\n";
    for (size_t i = 0; i < p2Laps.size(); ++i) {
        stats2 << "Vuelta " << (i + 1) << ": " 
               << std::fixed << std::setprecision(2) << p2Laps[i] << " s\n";
    }
    p2StatsText.setString(stats2.str());
}

void GameOverScreen::updateDisplay() {
    if (!fontLoaded) return;
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
    if (bgSprite.has_value()) {
        window.draw(bgSprite.value());
    }
    window.draw(overlay);

    if (fontLoaded) {
        window.draw(titleText);
        window.draw(winnerText);
        window.draw(p1StatsText); 
        window.draw(p2StatsText); 
        window.draw(playAgainText);
        window.draw(exitText);
    }
    
    if (winnerSprite.has_value()) {
        window.draw(winnerSprite.value());
    }
}

void GameOverScreen::reset() {
    selectedOption = 0;
    optionConfirmed = false;
    updateDisplay();
}

void GameOverScreen::setWinnerImage(const sf::Texture& texture) {
    winnerSprite.emplace(texture); 
    
    sf::FloatRect bounds = winnerSprite->getLocalBounds();
    if (bounds.size.x > 0 && bounds.size.y > 0) {
        winnerSprite->setScale(sf::Vector2f(250.0f / bounds.size.x, 250.0f / bounds.size.y));
    }
    
    winnerSprite->setPosition(sf::Vector2f(800.0f, 180.0f)); 
}