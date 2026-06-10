#include "CarSelectionScreen.h"
#include <iostream>

CarSelectionScreen::CarSelectionScreen() {
    player1Selection = 0;
    player2Selection = 1;
    player1Confirmed = false;
    player2Confirmed = false;

    font = std::make_shared<sf::Font>();
    loadFont(); 

    titleText = std::make_shared<sf::Text>(*font);
    player1StatusText = std::make_shared<sf::Text>(*font);
    player2StatusText = std::make_shared<sf::Text>(*font);

    // Asignamos los textos principales de forma segura
    titleText->setString("SELECCIONA TU NAVE");
    titleText->setCharacterSize(50);
    titleText->setFillColor(sf::Color::White);
    titleText->setPosition({150.0f, 50.0f}); 

    player1StatusText->setCharacterSize(24);
    player1StatusText->setPosition({50.0f, 500.0f});
    player1StatusText->setFillColor(sf::Color::White);

    player2StatusText->setCharacterSize(24);
    player2StatusText->setPosition({600.0f, 500.0f});
    player2StatusText->setFillColor(sf::Color::White);

    initializeCars();
    updateDisplay();
}

bool CarSelectionScreen::loadFont() {
    if (!font->openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error al cargar la fuente\n";
        return false;
    }
    return true;
}

void CarSelectionScreen::initializeCars() {
    cars = {
        {0, "Fierrari F-Ocho"}, {1, "Lamborgota veneno"}, {2, "Ford Firulais"},
        {3, "Puerche 9-Once"}, {4, "Dodge Chanclenger"}
    };

    carTexts.clear(); // Limpiamos la memoria por si acaso
    // Usamos cars.size() en lugar de NUM_CARS para evitar el Segmentation Fault
    for (size_t i = 0; i < cars.size(); ++i) {
        sf::Text text(*font); 
        text.setString(cars[i].name);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        
        float x = 100.0f + (i % 3) * 200.0f;
        float y = 200.0f + (i / 3) * 150.0f;
        text.setPosition({x, y}); 
        
        carTexts.push_back(text);
    }
}

void CarSelectionScreen::updateDisplay() {
    for (size_t i = 0; i < carTexts.size(); ++i) {
        carTexts[i].setFillColor(sf::Color::White);
        if (i == (size_t)player1Selection) carTexts[i].setFillColor(sf::Color::Red);
        if (i == (size_t)player2Selection) carTexts[i].setFillColor(sf::Color::Blue);
        if (i == (size_t)player1Selection && i == (size_t)player2Selection) carTexts[i].setFillColor(sf::Color::Magenta);
    }

    player1StatusText->setString(player1Confirmed ? "P1: LISTO" : "P1: Eligiendo...");
    player2StatusText->setString(player2Confirmed ? "P2: LISTO" : "P2: Eligiendo...");
}

void CarSelectionScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        int maxCars = cars.size(); // Tope dinámico seguro
        if (maxCars == 0) return;

        // JUGADOR 1 (WASD)
        if (!player1Confirmed) {
            if (keyEvent->code == sf::Keyboard::Key::D) player1Selection = (player1Selection + 1) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::A) player1Selection = (player1Selection - 1 + maxCars) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::W) player1Selection = (player1Selection - 3 + maxCars) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::S) player1Selection = (player1Selection + 3) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::Space) player1Confirmed = true;
        }

        // JUGADOR 2 (Flechas)
        if (!player2Confirmed) {
            if (keyEvent->code == sf::Keyboard::Key::Right) player2Selection = (player2Selection + 1) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::Left) player2Selection = (player2Selection - 1 + maxCars) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::Up) player2Selection = (player2Selection - 3 + maxCars) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::Down) player2Selection = (player2Selection + 3) % maxCars;
            else if (keyEvent->code == sf::Keyboard::Key::Enter) player2Confirmed = true;
        }
    }
    updateDisplay();
}

void CarSelectionScreen::render(sf::RenderWindow& window) {
    window.draw(*titleText);
    window.draw(*player1StatusText);
    window.draw(*player2StatusText);
    
    for (const auto& text : carTexts) {
        window.draw(text);
    }
}

bool CarSelectionScreen::areBothConfirmed() const {
    return player1Confirmed && player2Confirmed;
}

CarSelectionScreen::~CarSelectionScreen() {
}

void CarSelectionScreen::reset() {
    player1Selection = 0;
    player2Selection = 1;
    player1Confirmed = false;
    player2Confirmed = false;
    updateDisplay();
}