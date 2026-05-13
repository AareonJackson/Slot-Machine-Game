#include "ui/game_window.h"
#include <iostream>
#include <sstream>
#include <iomanip>

GameWindow::GameWindow(int width, int height, const std::string &title)
    : m_window(sf::VideoMode(width, height), title) {
        m_window.setFramerateLimit(60);

        // Calculate a centered position for the 600x400 reel grid
        float gridWidth = 600.0f;
        float gridHeight = 400.0f;
        float startX = (width - gridWidth) / 2.0f;
        float startY = 50.0f; // Padding front the top

        m_reelView = std::make_unique<ReelView>(3, 3, startX, startY, gridWidth, gridHeight);

        // Setup initial dummy symbols so the grid looks populated when launching
        std::vector<std::vector<std::string>> initialSymbols = {
            {"SEVEN", "CHERRY", "BAR"},
            {"LEMON", "BELL", "SEVEN"},
            {"CHERRY", "BAR", "BELL"}
        };
    m_reelView->updateSymbols(initialSymbols);

    if (!m_font.loadFromFile("assets/fonts/Roboto-Regular.ttf")) {
        std::cerr << "Failed to load font: assets/fonts/Roboto-Regular.ttf" << std::endl;
    }

    m_balanceText.setFont(m_font);
    m_balanceText.setCharacterSize(24);
    m_balanceText.setFillColor(sf::Color::White);
    m_balanceText.setPosition(30.0f, 15.0f);

    m_betText.setFont(m_font);
    m_betText.setCharacterSize(24);
    m_betText.setFillColor(sf::Color::White);
    m_betText.setPosition(300.0f, 15.0f);

    m_lastWinText.setFont(m_font);
    m_lastWinText.setCharacterSize(24);
    m_lastWinText.setFillColor(sf::Color::Yellow);
    m_lastWinText.setPosition(520.0f, 15.0f);

    m_statsText.setFont(m_font);
    m_statsText.setCharacterSize(18);
    m_statsText.setFillColor(sf::Color::White);
    m_statsText.setPosition(30.0f, static_cast<float>(height) - 35.0f);

    updateStatusText(1000.0, 10.0, 0.0);
    updateStatsText(0, 0.0, 0.0, 0.0);

    // Spin button setup
    float btnWidth = 200.0f;
    float btnHeight = 60.0f;
    float btnX = (width - btnWidth) / 2.0f; // Centered
    float btnY = startY + gridHeight + 40.0f; // Placed below the reels

    m_spinButton = std::make_unique<Button>(btnX, btnY, btnWidth, btnHeight, "SPIN!");

    float smallBtnWidth = 120.0f;
    float smallBtnHeight = 50.0f;
    float betButtonY = btnY + btnHeight + 20.0f;

    m_betDownButton = std::make_unique<Button>(
        btnX - smallBtnWidth - 20.0f,
        betButtonY,
        smallBtnWidth,
        smallBtnHeight,
        "BET -"
    );

    m_betUpButton = std::make_unique<Button>(
        btnX + btnWidth + 20.0f,
        betButtonY,
        smallBtnWidth,
        smallBtnHeight,
        "BET +"
    );

    m_autoPlayButton = std::make_unique<Button>(
        btnX,
        betButtonY,
        smallBtnWidth,
        smallBtnHeight,
        "AUTO"
    );

    m_helpButton = std::make_unique<Button>(
        static_cast<float>(width) - 150.0f,
        static_cast<float>(height) - 70.0f,
        120.0f,
        50.0f,
        "HELP"
    );

    m_paytableOverlay.setPosition(100.0f, 80.0f);
    m_paytableOverlay.setSize(sf::Vector2f(static_cast<float>(width) - 200.0f, static_cast<float>(height) - 160.0f));
    m_paytableOverlay.setFillColor(sf::Color(0, 0, 0, 220));
    m_paytableOverlay.setOutlineColor(sf::Color(255, 215, 0));
    m_paytableOverlay.setOutlineThickness(4.0f);

    m_paytableText.setFont(m_font);
    m_paytableText.setCharacterSize(22);
    m_paytableText.setFillColor(sf::Color::White);
    m_paytableText.setPosition(130.0f, 110.0f);
    m_paytableText.setString("PAYTABLE\n\nLoading...");

    // Define button click callbacks
    m_spinButton->setOnClick([this]() {
        if (m_spinCallback) {
            m_spinCallback();
        }
    });

    m_betDownButton->setOnClick([this]() {
        if (m_betDownCallback) {
            m_betDownCallback();
        }
    });

    m_betUpButton->setOnClick([this]() {
        if (m_betUpCallback) {
            m_betUpCallback();
        }
    });

    m_autoPlayButton->setOnClick([this]() {
        if (m_autoPlayCallback) {
            m_autoPlayCallback();
        }
    });

    m_helpButton->setOnClick([this]() {
        m_showPaytable = !m_showPaytable;
    });
}

GameWindow::~GameWindow() {
    if (m_window.isOpen()) {
        m_window.close();
    }
}

bool GameWindow::isOpen() const {
    return m_window.isOpen();
}

void GameWindow::pollEvents() {
    sf::Event event;
    // Poll for events
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        // Pass events to UI components
        if (m_spinButton) {
            m_spinButton->handleEvent(event, m_window);
        }

        if (m_betDownButton) {
            m_betDownButton->handleEvent(event, m_window);
        }

        if (m_betUpButton) {
            m_betUpButton->handleEvent(event, m_window);
        }

        if (m_autoPlayButton) {
            m_autoPlayButton->handleEvent(event, m_window);
        }

        if (m_helpButton) {
            m_helpButton->handleEvent(event, m_window);
        }
    }

    // Update UI components
    if (m_spinButton) {
        m_spinButton->update(sf::Mouse::getPosition(m_window));
    }

    if (m_betDownButton) {
        m_betDownButton->update(sf::Mouse::getPosition(m_window));
    }

    if (m_betUpButton) {
        m_betUpButton->update(sf::Mouse::getPosition(m_window));
    }

    if (m_autoPlayButton) {
        m_autoPlayButton->update(sf::Mouse::getPosition(m_window));
    }

    if (m_helpButton) {
        m_helpButton->update(sf::Mouse::getPosition(m_window));
    }
}

void GameWindow::render() {
    // Clear the screen with a casino green background color
    m_window.clear(sf::Color(34, 139, 34));

    // Draw the slot reels
    if (m_reelView) {
        m_reelView->draw(m_window);
    }

    m_window.draw(m_balanceText);
    m_window.draw(m_betText);
    m_window.draw(m_lastWinText);
    m_window.draw(m_statsText);

    if (m_spinButton) {
        m_spinButton->draw(m_window);
    }

    if (m_betDownButton) {
        m_betDownButton->draw(m_window);
    }

    if (m_betUpButton) {
        m_betUpButton->draw(m_window);
    }

    if (m_autoPlayButton) {
        m_autoPlayButton->draw(m_window);
    }

    if (m_helpButton) {
        m_helpButton->draw(m_window);
    }

    if (m_showPaytable) {
        m_window.draw(m_paytableOverlay);
        m_window.draw(m_paytableText);
    }

    // Display window
    m_window.display();
}

void GameWindow::updateReels(const std::vector<std::vector<std::string>> &symbols) {
    if (m_reelView) {
        m_reelView->updateSymbols(symbols);
    }
}

void GameWindow::setHighlightedCells(const std::vector<std::vector<bool>>& highlightedCells) {
    if (m_reelView) {
        m_reelView->setHighlightedCells(highlightedCells);
    }
}

void GameWindow::clearHighlightedCells() {
    if (m_reelView) {
        m_reelView->clearHighlightedCells();
    }
}

void GameWindow::updateStatusText(double balance, double currentBet, double lastWin) {
    std::ostringstream balanceStream;
    balanceStream << std::fixed << std::setprecision(2) << "Balance: $" << balance;
    m_balanceText.setString(balanceStream.str());

    std::ostringstream betStream;
    betStream << std::fixed << std::setprecision(2) << "Bet: $" << currentBet;
    m_betText.setString(betStream.str());

    std::ostringstream winStream;
    winStream << std::fixed << std::setprecision(2) << "Win: $" << lastWin;
    m_lastWinText.setString(winStream.str());
}

void GameWindow::updateStatsText(int totalSpins, double totalWagered, double totalWon, double biggestWin) {
    double net = totalWon - totalWagered;

    std::ostringstream statsStream;
    statsStream << std::fixed << std::setprecision(2)
                << "Spins: " << totalSpins
                << " |  Wagered: $" << totalWagered
                << " |  Won: $" << totalWon
                << " |  Net: $" << net
                << " |  Best: $" << biggestWin;
    m_statsText.setString(statsStream.str());
}

void GameWindow::updatePaytableText(const std::string& paytableText) {
    m_paytableText.setString(paytableText);
}

void GameWindow::setSpinCallback(std::function<void()> callback) {
    m_spinCallback = std::move(callback);
}

void GameWindow::setBetUpCallback(std::function<void()> callback) {
    m_betUpCallback = std::move(callback);
}

void GameWindow::setBetDownCallback(std::function<void()> callback) {
    m_betDownCallback = std::move(callback);
}

void GameWindow::setAutoPlayCallback(std::function<void()> callback) {
    m_autoPlayCallback = std::move(callback);
}

sf::RenderWindow& GameWindow::getWindow() {
    return m_window;
}