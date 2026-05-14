#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <functional>
#include "ui/reel_view.h"
#include "ui/button.h"

class GameWindow {
public:
    GameWindow(int width, int height, const std::string& title);
    ~GameWindow();

    bool isOpen() const;
    void pollEvents();
    void render();

    void updateReels(const std::vector<std::vector<std::string>>& symbols);
    void updateStatusText(double balance, double currentBet, double lastWin);
    void updateStatsText(int totalSpins, double totalWagered, double totalWon, double biggestWin);
    void updateFreeSpinsText(int freeSpinsRemaining);
    void updatePaytableText(const std::string& paytableText);
    void setHighlightedCells(const std::vector<std::vector<bool>>& highlightedCells);
    void clearHighlightedCells();
    void setSpinCallback(std::function<void()> callback);
    void setBetUpCallback(std::function<void()> callback);
    void setBetDownCallback(std::function<void()> callback);
    void setAutoPlayCallback(std::function<void()> callback);

    // Getter just in case I want to pass the window to other UI components later
    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow m_window;

    // UI Components
    std::unique_ptr<ReelView> m_reelView;
    std::unique_ptr<Button> m_spinButton;
    std::unique_ptr<Button> m_betUpButton;
    std::unique_ptr<Button> m_betDownButton;
    std::unique_ptr<Button> m_helpButton;
    std::unique_ptr<Button> m_autoPlayButton;

    sf::Font m_font;
    sf::Text m_balanceText;
    sf::Text m_betText;
    sf::Text m_lastWinText;
    sf::Text m_statsText;
    sf::Text m_freeSpinsText;
    sf::Text m_paytableText;
    sf::RectangleShape m_paytableOverlay;

    bool m_showPaytable = false;

    std::function<void()> m_spinCallback;
    std::function<void()> m_betUpCallback;
    std::function<void()> m_betDownCallback;
    std::function<void()> m_autoPlayCallback;
};
