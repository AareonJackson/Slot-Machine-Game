#include "ui/reel_view.h"
#include <iostream>

ReelView::ReelView(int numReels, int numRows, float startX, float startY, float width, float height)
    : m_numReels(numReels), m_numRows(numRows), m_x(startX), m_y(startY), m_width(width), m_height(height) {

    // Setup the main background frame for the slot machine reels
    m_machineFrame.setPosition(m_x, m_y);
    m_machineFrame.setSize(sf::Vector2f(m_width, m_height));
    m_machineFrame.setFillColor(sf::Color(20, 20, 20)); // Dark grey background
    m_machineFrame.setOutlineColor(sf::Color(218, 165, 32)); // Golden outline
    m_machineFrame.setOutlineThickness(5.0f);

    // Initialize with empty symbols
    m_symbols.resize(m_numReels, std::vector<std::string>(m_numRows, "???"));
    m_highlightedCells.resize(m_numReels, std::vector<bool>(m_numRows, false));
    // Try to load font from local assets first
    m_hasFont = m_font.loadFromFile("assets/fonts/Roboto-Regular.ttf");
    // Fallback 1: Modern MacOS (Big Sur and newer / Apple Silicon)
    if (!m_hasFont) {
        m_hasFont = m_font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf"); // macOS
    }
    // Fallback 2: Old macOS
    if (!m_hasFont) {
        m_hasFont = m_font.loadFromFile("/Library/Fonts/Arial.ttf");
    }
    // Fallback 3: Standard Monospace font on macOS
    if (!m_hasFont) {
        m_hasFont = m_font.loadFromFile("/System/Library/Fonts/Monaco.ttf");
    }

    if (!m_hasFont) {
        std::cerr << "Warning: No suitable font found. Text will not be rendered." << std::endl;
    }
}

void ReelView::updateSymbols(const std::vector<std::vector<std::string>>& symbols) {
    if (symbols.size() == m_numReels && symbols[0].size() == m_numRows) {
        m_symbols = symbols;
    }
}

void ReelView::setHighlightedCells(const std::vector<std::vector<bool>>& highlightedCells) {
    if (highlightedCells.size() == m_numReels && highlightedCells[0].size() == m_numRows) {
        m_highlightedCells = highlightedCells;
    }
}

void ReelView::clearHighlightedCells() {
    m_highlightedCells.assign(m_numReels, std::vector<bool>(m_numRows, false));
}

sf::Color ReelView::getSymbolColor(const std::string& symbol) {
    if (symbol == "SEVEN") return sf::Color::Red;
    if (symbol == "CHERRY") return sf::Color(222, 49, 99); // Cherry Red
    if (symbol == "BAR") return sf::Color(105, 105, 105); // Grey
    if (symbol == "LEMON") return sf::Color::Yellow;
    if (symbol == "BELL") return sf::Color(255,215,0); // Gold
    return sf::Color::White; // Default
}

void ReelView::draw(sf::RenderWindow& window) {
    // Draw the main machine frame
    window.draw(m_machineFrame);

    // Calculate dynamic sizing for the grid cells with padding
    float padding = 10.0f;
    float cellWidth = (m_width - (padding * (m_numReels + 1))) / m_numReels;
    float cellHeight = (m_height - (padding * (m_numRows + 1))) / m_numRows;

    for (int reel = 0; reel < m_numReels; ++reel) {
        for (int row = 0; row < m_numRows; ++row) {
            // Calculate the position of the cell
            float cellX = m_x + padding + reel * (cellWidth + padding);
            float cellY = m_y + padding + row * (cellHeight + padding);

            // Draw Symbol Background (White Card)
            sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
            cell.setPosition(cellX, cellY);
            cell.setFillColor(sf::Color::White); // White background

            // cell.setOutlineColor(sf::Color(100, 100, 100)); // Dark grey outline
            // cell.setOutlineThickness(2.0f);
            // window.draw(cell);

            if (m_highlightedCells[reel][row]) {
                cell.setOutlineColor(sf::Color(255, 215, 0)); // Gold highlight
                cell.setOutlineThickness(6.0f);
            } else {
                cell.setOutlineColor(sf::Color(100, 100, 100)); // Dark grey outline
                cell.setOutlineThickness(2.0f);
            }

            window.draw(cell);

            // Draw a colored inner rectangle to represent the graphical symbol
            sf::RectangleShape graphic(sf::Vector2f(cellWidth * 0.6f, cellHeight * 0.6f));
            graphic.setPosition(cellX + cellWidth * 0.2f, cellY + cellHeight * 0.1f);
            graphic.setFillColor(getSymbolColor(m_symbols[reel][row]));
            window.draw(graphic);

            // Draw text identifier below the graphic if font is loaded
            if (m_hasFont) {
                sf::Text text(m_symbols[reel][row], m_font, 18);
                text.setFillColor(sf::Color::Black);

                // Center the text at the bottom of the cell
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.setPosition(cellX + cellWidth / 2.0f, cellY + cellHeight * 0.85f);

                window.draw(text);
            }
        }
    }
}