/// 22i-1222 shoaib irfan 22i-1104 Mubariz haroon  22i-0996
#include "Game.cpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
using namespace std;
using namespace sf;

int main()
{
    // Initialize SFML graphics without hardware acceleration
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Default;
    settings.majorVersion = 2;
    settings.minorVersion = 0; // Deactivate joystick

    // Can be Adjusted according to Screen Size
    const int rows = 15, cols = 20;
    const float cellSize = 50.f, borderWidth = 2.f, margin = 20.f;

    const float windowWidth = (cols * cellSize + (cols + 1) * borderWidth + 2 * margin) + 300; // Calculate the window width
    const float windowHeight = rows * cellSize + (rows + 1) * borderWidth + 2 * margin;        // Calculate the window height

    Game game(rows, cols);
    game.initializeMaze(rows, cols);
    game.run(windowWidth + 300, windowHeight, rows, cols, cellSize, borderWidth, margin);

    return 0;
}