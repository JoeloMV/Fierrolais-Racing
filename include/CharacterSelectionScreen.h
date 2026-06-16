#ifndef CHARACTERSELECTIONSCREEN_H
#define CHARACTERSELECTIONSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

struct Character {
    int id;
    std::string name;
};

class CharacterSelectionScreen {
private:
    sf::Font font; // Ahora la fuente es un objeto directo
    sf::Texture puercheTexture;
    std::vector<Character> characters;
    
    int player1Selection;
    int player2Selection;
    bool player1Confirmed;
    bool player2Confirmed;
    
    std::string player1Name;
    std::string player2Name;
    
    // Quitamos los shared_ptr de los textos
    sf::Text titleText;
    std::vector<sf::Text> characterTexts;
    sf::Text player1StatusText;
    sf::Text player2StatusText;
    
    static const int NUM_CHARACTERS = 5;
    static const int CHARACTER_BOX_SIZE = 60;
    static const int CHARACTER_SPACING = 20;
    
    bool loadFont();
    void initializeCharacters();
    void updateDisplay();
    
public:
    CharacterSelectionScreen();
    ~CharacterSelectionScreen();
    
    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void setPlayerNames(const std::string& p1, const std::string& p2);
    void reset();
    void update() {}  // No-op, solo para compatibilidad con Game
    
    bool areBothConfirmed() const;
    int getPlayer1Selection() const { return player1Selection; }
    int getPlayer2Selection() const { return player2Selection; }
    const Character& getCharacter(int id) const { return characters[id]; }
};

#endif // CHARACTERSELECTIONSCREEN_H