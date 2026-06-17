#include "CarSelectionScreen.h"
#include <iostream>

CarSelectionScreen::CarSelectionScreen() {
    player1Selection = 0;
    player2Selection = 1;
    player1Confirmed = false;
    player2Confirmed = false;

    darkOverlay.setSize(sf::Vector2f(1200.0f, 700.0f));
    darkOverlay.setPosition(sf::Vector2f(0.0f, 0.0f));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));

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
    player1StatusText->setPosition({150.0f, 630.0f});
    player1StatusText->setFillColor(sf::Color::Blue);

    player2StatusText->setCharacterSize(24);
    player2StatusText->setPosition({850.0f, 630.0f});
    player2StatusText->setFillColor(sf::Color::Red);

    initializeCars(); // Aquí solo creamos los textos
    updateDisplay();
}

bool CarSelectionScreen::loadFont() {
    if (!font->openFromFile("assets/fonts/arial.ttf")) { 
        std::cerr << "Advertencia: No se encontró en assets, intentando usar la de Windows...\n";
        if (!font->openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "Error crítico: Definitivamente no se pudo cargar arial.ttf\n";
            return false;
        }
    }
    return true;
}

void CarSelectionScreen::initializeCars() {
    // Current data (los nombres de tus carros)
    cars = {
        {0, "Fierrari F-Ocho"}, {1, "Lamborgota veneno"}, {2, "Ford Firulais"},
        {3, "Puerche 9-Once"}, {4, "Dodge Chanclenger"}
    };
    carTexts.clear(); // Limpiamos el vector.

    // === LÓGICA DE CENTRADO Y REUBICACIÓN EN CUADRÍCULA ===
    // Configuración para centrar la cuadrícula de carros (3x2).
    int numCols = 3;
    float screenWidth = 1200.0f;
    float colWidth = 333.0f; // 1000px de área de selección / 3 columnas.
    float gridOffset = 100.0f; // El área oscura empieza 100px desde la izquierda.

    for (size_t i = 0; i < cars.size(); ++i) {
        sf::Text text(*font);
        text.setString(cars[i].name);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        int col = i % numCols; // Calcula columna (0, 1, 2)
        int row = i / numCols; // Calcula fila (0, 1)

        // --- CÁLCULO DE POSICIONES CENTRADAS ---
        // X: Centramos el texto horizontalmente bajo su columna.
        float textWidth = text.getLocalBounds().size.x;
        float x = gridOffset + (col * colWidth) + (colWidth - textWidth) / 2.0f;

        // Y: Posicionamos verticalmente para dejar espacio para los carros arriba.
        // Los carros se centran alrededor de la posición grid (125px horizontally centered on screen).
        float carSelectionAreaStartY = 125.0f;
        float y = carSelectionAreaStartY + (row * 225.0f) + 160.0f; // Centramos el texto en la altura de la fila.

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
    
    // 1. CARGA DE RECURSOS
    if (!texturesLoaded) {
        if (bgTexture.loadFromFile("assets/bg_car_selection.jpg")) {
            bgSprite.emplace(bgTexture);
            sf::FloatRect bounds = bgSprite->getLocalBounds();
            bgSprite->setScale(sf::Vector2f({1200.0f / bounds.size.x, 700.0f / bounds.size.y}));
        }
        fierrariTexture.loadFromFile("assets/fierrari.png");
        lamborgotaTexture.loadFromFile("assets/lamborgota.png");
        fordTexture.loadFromFile("assets/ford.png");
        puercheTexture.loadFromFile("assets/puerche.png");
        dodgeTexture.loadFromFile("assets/dodge.png");
        texturesLoaded = true;
    }

    // 2. DIBUJAR FONDO
    if (bgSprite.has_value()) {
        window.draw(bgSprite.value());
    }

    // 3. DIBUJAR PANEL CENTRAL OSCURO
    sf::RectangleShape carSelectionBox;
    carSelectionBox.setSize(sf::Vector2f({900.0f, 450.0f}));
    carSelectionBox.setPosition(sf::Vector2f({150.0f, 150.0f})); 
    carSelectionBox.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(carSelectionBox);

    // 4. DIBUJAR TÍTULO
    float titleWidth = titleText->getLocalBounds().size.x;
    titleText->setPosition(sf::Vector2f({(1200.0f - titleWidth) / 2.0f, 60.0f}));
    window.draw(*titleText);

    // 5. DIBUJAR CARROS (Centrados automáticamente con sus nombres)
    sf::Texture* textures[] = {&fierrariTexture, &lamborgotaTexture, &fordTexture, &puercheTexture, &dodgeTexture};
    
    for (size_t i = 0; i < 5; ++i) {
        sf::Sprite sprite(*textures[i]);
        sprite.setScale(sf::Vector2f({0.25f, 0.25f})); 
        
        // Obtenemos la posición y tamaño del TEXTO de este carro
        sf::Vector2f textPos = carTexts[i].getPosition();
        float textWidth = carTexts[i].getLocalBounds().size.x;
        
        // Obtenemos el tamaño real del SPRITE (ya escalado a 0.25)
        float spriteWidth = sprite.getLocalBounds().size.x * 0.25f;
        float spriteHeight = sprite.getLocalBounds().size.y * 0.25f;
        
        // Matemáticas para centrar: X del texto + mitad del ancho del texto - mitad del ancho del sprite
        float x = textPos.x + (textWidth / 2.0f) - (spriteWidth / 2.0f);
        // Y: Lo ponemos justo arriba del texto (con 15 pixeles de separación)
        float y = textPos.y - spriteHeight - 15.0f; 
        
        sprite.setPosition(sf::Vector2f({x, y}));
        window.draw(sprite);
    }

    // 6. DIBUJAR TEXTOS DE LOS NOMBRES
    for (const auto& text : carTexts) {
        window.draw(text);
    }

    // 7. DIBUJAR ESTADOS (Ahora en la parte de abajo de la pantalla)
    window.draw(*player1StatusText);
    window.draw(*player2StatusText);
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