#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ReelView {
public:
    ReelView(int numReels, int numRows, float startX, float startY, float width, float height);

    // Update the symbols to be displayed.
    // Outer vector is reels (columns), inner vector is rows.
    void updateSymbols(const std::vector<std::vector<std::string>>& symbols);

    void setHighlightedCells(const std::vector<std::vector<bool>>& highlightedCells);
    void clearHighlightedCells();

    // Draw the reel
    void draw(sf::RenderWindow& window);

private:
    int m_numReels;
    int m_numRows;
    float m_x;
    float m_y;
    float m_width;
    float m_height;

    std::vector<std::vector<std::string>> m_symbols;
    std::vector<std::vector<bool>> m_highlightedCells;

    // SFML Graphics objects.
    sf::RectangleShape m_machineFrame;
    sf::Font m_font;
    bool m_hasFont;
    
    std::map<std::string, sf::Texture> m_symbolTextures;

    void loadSymbolTextures();
    bool hasTextureForSymbol(const std::string& symbol) const;
    void drawSymbolImage(sf::RenderWindow& window, const std::string& symbol, float cellX, float cellY, float cellWidth, float cellHeight);


    // Helper to get the color based on symbol string.
    sf::Color getSymbolColor(const std::string& symbol);
};