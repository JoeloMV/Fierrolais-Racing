#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <memory>

class OptionsMenu {
private:
    std::shared_ptr<sf::Font> font;
    std::vector<sf::Text> optionLabels;
    std::vector<sf::Text> optionValues;
    int selectedIndex;
    sf::Color normalColor;
    sf::Color selectedColor;
    std::shared_ptr<sf::Text> titleText;
    
    // Volúmenes
    float masterVolume;      // 0 a 100
    float musicVolume;       // 0 a 100
    
    // Referencia a la música compartida del juego
    std::shared_ptr<sf::Music> sharedBackgroundMusic;
    
    bool loadFont();
    void updateDisplay();
    
public:
    OptionsMenu();
    ~OptionsMenu();
    
    void handleInput(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);
    void reset();
    
    void setBackgroundMusic(std::shared_ptr<sf::Music> music);
    
    float getMasterVolume() const { return masterVolume; }
    float getMusicVolume() const { return musicVolume; }
    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
};

#endif // OPTIONSMENU_H
