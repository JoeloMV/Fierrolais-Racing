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
        
      float x = 0.0f;
        float y = 0.0f;

        // Posiciones manuales fijas para que todo quede centrado y arriba
        switch(i) {
            case 0: x = 120.0f; y = 180.0f; break; // Fierrari (Arriba Izquierda)
            case 1: x = 440.0f; y = 180.0f; break; // Lamborgota (Arriba Centro)
            case 2: x = 760.0f; y = 180.0f; break; // Ford (Arriba Derecha)
            case 3: x = 280.0f; y = 380.0f; break; // Puerche (Abajo Izquierda centrado)
            case 4: x = 600.0f; y = 380.0f; break; // Dodge (Abajo Derecha centrado)
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
   static bool texturesLoaded = false;
if (!texturesLoaded) {
    puercheTexture.loadFromFile("assets/puerche.png");
    if (!puercheTexture.loadFromFile("assets/puerche.png")) {
    std::cerr << "Error: No se pudo cargar assets/puerche.png" << std::endl;
}
    fierrariTexture.loadFromFile("assets/fierrari.png");
    lamborgotaTexture.loadFromFile("assets/lamborgota.png");
    fordTexture.loadFromFile("assets/ford.png");
    dodgeTexture.loadFromFile("assets/dodge.png");
    texturesLoaded = true;
}

    // 2. Dibujar los textos originales
    window.draw(*titleText);
    window.draw(*player1StatusText);
    window.draw(*player2StatusText);

    for (const auto& text : carTexts) {
        window.draw(text);
    }
// 3. ¡Dibujar los carros dinámicos sobre el texto seleccionado!
    if (texturesLoaded && !carTexts.empty()) {
        
        // --- JUGADOR 1 ---
        // 1. Primero creamos un "puntero" para elegir qué textura usar
        sf::Texture* p1Texture = &puercheTexture; 
        if (player1Selection == 0) p1Texture = &fierrariTexture;
        else if (player1Selection == 1) p1Texture = &lamborgotaTexture;
        else if (player1Selection == 2) p1Texture = &fordTexture;
        else if (player1Selection == 3) p1Texture = &puercheTexture;
        else if (player1Selection == 4) p1Texture = &dodgeTexture;

        // 2. Ahora sí, creamos el sprite dándole la textura de golpe. ¡Así SFML no se queja!
        sf::Sprite p1CarSprite(*p1Texture);
        p1CarSprite.setScale(sf::Vector2f(0.4f, 0.4f)); 
        float p1CarX = carTexts[player1Selection].getPosition().x;
        float p1CarY = carTexts[player1Selection].getPosition().y - 120;
        p1CarSprite.setPosition(sf::Vector2f(p1CarX, p1CarY));
        window.draw(p1CarSprite);


        // --- JUGADOR 2 ---
        // 1. Hacemos lo mismo para el Jugador 2
        sf::Texture* p2Texture = &lamborgotaTexture; 
        if (player2Selection == 0) p2Texture = &fierrariTexture;
        else if (player2Selection == 1) p2Texture = &lamborgotaTexture;
        else if (player2Selection == 2) p2Texture = &fordTexture;
        else if (player2Selection == 3) p2Texture = &puercheTexture;
        else if (player2Selection == 4) p2Texture = &dodgeTexture;

        // 2. Creamos el sprite del Jugador 2 con su textura directa
        sf::Sprite p2CarSprite(*p2Texture);
        p2CarSprite.setScale(sf::Vector2f(0.4f, 0.4f));
        float p2CarX = carTexts[player2Selection].getPosition().x;
        float p2CarY = carTexts[player2Selection].getPosition().y - 120;
        p2CarSprite.setPosition(sf::Vector2f(p2CarX, p2CarY));
        window.draw(p2CarSprite);
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