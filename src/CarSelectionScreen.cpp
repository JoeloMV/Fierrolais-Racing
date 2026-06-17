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

    titleText->setString("SELECCIONA TU NAVE PARA LA CARRERA");
    titleText->setCharacterSize(50);
    titleText->setFillColor(sf::Color::Blue);
    titleText->setPosition({75.0f, 5.0f}); 

    player1StatusText->setCharacterSize(24);
    player1StatusText->setPosition({50.0f, 500.0f});
    player1StatusText->setFillColor(sf::Color::Blue);

    player2StatusText->setCharacterSize(24);
    player2StatusText->setPosition({600.0f, 500.0f});
    player2StatusText->setFillColor(sf::Color::Red);

    initializeCars(); // Aquí solo creamos los textos
    updateDisplay();
}

bool CarSelectionScreen::loadFont() {
    if (!font->openFromFile("assets/arial.ttf")) { 
        std::cerr << "Advertencia: No se encontró en assets, intentando usar la de Windows...\n";
        if (!font->openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "Error crítico: Definitivamente no se pudo cargar arial.ttf\n";
            return false;
        }
    }
    return true;
}

void CarSelectionScreen::initializeCars() {
    cars = {
        {0, "Fierrari F-Ocho"}, {1, "Lamborgota veneno"}, {2, "Ford Firulais"},
        {3, "Puerche 9-Once"}, {4, "Dodge Chanclenger"}
    };

    carTexts.clear(); 
    
    for (size_t i = 0; i < cars.size(); ++i) {
        sf::Text text(*font); 
        text.setString(cars[i].name);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        
        float x = 0.0f;
        float y = 0.0f;

        // Posiciones manuales fijas
        switch(i) {
            case 0: x = 120.0f; y = 180.0f; break; // Fierrari 
            case 1: x = 440.0f; y = 180.0f; break; // Lamborgota 
            case 2: x = 760.0f; y = 180.0f; break; // Ford 
            case 3: x = 280.0f; y = 380.0f; break; // Puerche 
            case 4: x = 600.0f; y = 380.0f; break; // Dodge 
        }

        text.setPosition(sf::Vector2f(x, y));
        carTexts.push_back(text);
    }
}

void CarSelectionScreen::updateDisplay() {
    for (size_t i = 0; i < carTexts.size(); ++i) {
        carTexts[i].setFillColor(sf::Color::White);
        if (i == (size_t)player1Selection) carTexts[i].setFillColor(sf::Color::Blue);
        if (i == (size_t)player2Selection) carTexts[i].setFillColor(sf::Color::Red);
        if (i == (size_t)player1Selection && i == (size_t)player2Selection) carTexts[i].setFillColor(sf::Color::Magenta);
    }

    player1StatusText->setString(player1Confirmed ? "P1: LISTO" : "P1: Eligiendo...");
    player2StatusText->setString(player2Confirmed ? "P2: LISTO" : "P2: Eligiendo...");
}

void CarSelectionScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        int maxCars = cars.size(); 
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
    static bool texturesLoaded = false;
    
    // === CARGA SEGURA: Solo se ejecuta cuando la ventana ya existe ===
    if (!texturesLoaded) {
        
        // 1. Cargar fondo
        if (bgTexture.loadFromFile("assets/bg_car_selection.jpg")) {
            bgSprite.emplace(bgTexture);
            sf::FloatRect bounds = bgSprite->getLocalBounds();
            bgSprite->setScale(sf::Vector2f(1200.0f / bounds.size.x, 700.0f / bounds.size.y));
        }

        // 2. Cargar texturas de los carros
        fierrariTexture.loadFromFile("assets/fierrari.png");
        lamborgotaTexture.loadFromFile("assets/lamborgota.png");
        fordTexture.loadFromFile("assets/ford.png");
        puercheTexture.loadFromFile("assets/puerche.png");
        dodgeTexture.loadFromFile("assets/dodge.png");

        // 3. Crear los sprites de los carros estacionados
        stationaryCarSprites.clear();
        for (size_t i = 0; i < carTexts.size(); ++i) {
            sf::Texture* carTexture = nullptr;
            switch(i) {
                case 0: carTexture = &fierrariTexture; break;
                case 1: carTexture = &lamborgotaTexture; break;
                case 2: carTexture = &fordTexture; break;
                case 3: carTexture = &puercheTexture; break;
                case 4: carTexture = &dodgeTexture; break;
            }

            if (carTexture) {
                sf::Sprite carSprite(*carTexture);
                carSprite.setScale(sf::Vector2f(0.4f, 0.4f)); 
                float x = carTexts[i].getPosition().x;
                float y = carTexts[i].getPosition().y - 120;
                carSprite.setPosition(sf::Vector2f(x, y));
                stationaryCarSprites.push_back(carSprite);
            }
        }
        texturesLoaded = true;
    }

    // === DIBUJO EN PANTALLA ===
    if (bgSprite.has_value()) {
        window.draw(bgSprite.value());
    }    

    // Dibujar los carros fijos
    for (const auto& carSprite : stationaryCarSprites) {
        window.draw(carSprite);
    }

    // Dibujar los textos
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