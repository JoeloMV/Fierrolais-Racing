#ifndef CARSELECTIONSCREEN_H
#define CARSELECTIONSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>
#include <memory>

struct Car {
    int id;
    std::string name;
};

class CarSelectionScreen {
private:
    std::shared_ptr<sf::Font> font;
    std::vector<Car> cars;
    sf::Texture puercheTexture;

    int player1Selection;
    int player2Selection;
    bool player1Confirmed;
    bool player2Confirmed;

    std::shared_ptr<sf::Text> titleText;
    std::vector<sf::Text> carTexts;
    std::shared_ptr<sf::Text> player1StatusText;
    std::shared_ptr<sf::Text> player2StatusText;

    // Vamos a poner 6 carros para elegir
    static const int NUM_CARS = 6;
    static const int CAR_BOX_WIDTH = 120;
    static const int CAR_BOX_HEIGHT = 80;

    bool loadFont();
    void initializeCars();
    void updateDisplay();

public:
    CarSelectionScreen();
    ~CarSelectionScreen();

    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void reset();

    bool areBothConfirmed() const;
    int getPlayer1Selection() const { return player1Selection; }
    int getPlayer2Selection() const { return player2Selection; }
};

#endif // CARSELECTIONSCREEN_H