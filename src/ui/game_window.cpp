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

    m_freeSpinsText.setFont(m_font);
    m_freeSpinsText.setCharacterSize(24);
    m_freeSpinsText.setFillColor(sf::Color(255, 215, 0));
    m_freeSpinsText.setPosition(30.0f, 45.0f);
    m_freeSpinsText.setString("");

    m_modeText.setFont(m_font);
    m_modeText.setCharacterSize(22);
    m_modeText.setFillColor(sf::Color::Cyan);
    m_modeText.setPosition(700.0f, 15.0f);
    m_modeText.setString("Mode: Manual");

    m_winMessageText.setFont(m_font);
    m_winMessageText.setCharacterSize(36);
    m_winMessageText.setFillColor(sf::Color(255, 215, 0));
    m_winMessageText.setStyle(sf::Text::Bold);
    m_winMessageText.setString("");

    float winMessageWidth = 420.0f;
    float winMessageHeight = 90.0f;
    m_winMessageBackground.setSize(sf::Vector2f(winMessageWidth, winMessageHeight));
    m_winMessageBackground.setPosition(
      (static_cast<float>(width) - winMessageWidth) / 2.0f,
      (static_cast<float>(height) - winMessageHeight) / 2.0f
    );
    m_winMessageBackground.setFillColor(sf::Color(0, 0, 0, 190));
    m_winMessageBackground.setOutlineColor(sf::Color(255, 215, 0));
    m_winMessageBackground.setOutlineThickness(3.0f);

    m_statsText.setFont(m_font);
    m_statsText.setCharacterSize(18);
    m_statsText.setFillColor(sf::Color::White);
    m_statsText.setPosition(30.0f, static_cast<float>(height) - 35.0f);

    updateStatusText(1000.0, 10.0, 0.0);
    updateStatsText(0, 0.0, 0.0, 0.0);
    updateFreeSpinsText(0);
    updateModeText("Manual");

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

    m_resetButton = std::make_unique<Button>(
        static_cast<float>(width) - 290.0f,
        static_cast<float>(height) - 70.0f,
        120.0f,
        50.0f,
        "RESET"
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
        if (m_helpCallback) {
            m_helpCallback();
        } else {
            togglePaytable();
        }
    });

    m_resetButton->setOnClick([this]() {
       if (m_resetCallback) {
            m_resetCallback();
        }
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

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    if (m_spinCallback) {
                        m_spinCallback();
                    }
                    break;

                case sf::Keyboard::A:
                    if (m_autoPlayCallback) {
                        m_autoPlayCallback();
                    }
                    break;

                case sf::Keyboard::Up:
                case sf::Keyboard::Add:
                case sf::Keyboard::Equal:
                    if (m_betUpCallback) {
                        m_betUpCallback();
                    }
                    break;

                case sf::Keyboard::Down:
                case sf::Keyboard::Subtract:
                case sf::Keyboard::Hyphen:
                    if (m_betDownCallback) {
                        m_betDownCallback();
                    }
                    break;

                case sf::Keyboard::H:
                    if (m_helpCallback) {
                        m_helpCallback();
                    } else {
                        togglePaytable();
                    }
                    break;

                case sf::Keyboard::R:
                    if (m_resetCallback) {
                        m_resetCallback();
                    }
                    break;

                case sf::Keyboard::Escape:
                    if (m_escapeCallback) {
                        m_escapeCallback();
                    } else if (m_showPaytable) {
                        m_showPaytable = false;
                    }
                    break;

                default:
                    break;
            }
        }
        if (m_showPaytable && event.type == sf::Event::MouseWheelScrolled) {
            m_paytableScrollOffset -= event.mouseWheelScroll.delta * m_paytableScrollSpeed;

            if (m_paytableScrollOffset < 0.0f) {
                m_paytableScrollOffset = 0.0f;
            }

            if (m_paytableScrollOffset > m_paytableMaxScrollOffset) {
                m_paytableScrollOffset = m_paytableMaxScrollOffset;
            }
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

        if (m_resetButton) {
            m_resetButton->handleEvent(event, m_window);
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

    if (m_resetButton) {
        m_resetButton->update(sf::Mouse::getPosition(m_window));
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
    m_window.draw(m_freeSpinsText);
    m_window.draw(m_modeText);

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

    if (m_resetButton) {
        m_resetButton->draw(m_window);
    }

    if (m_showWinMessage) {
        m_window.draw(m_winMessageBackground);
        m_window.draw(m_winMessageText);
    }

    if (m_showPaytable) {
        m_window.draw(m_paytableOverlay);
        sf::View previousView = m_window.getView();

        sf::FloatRect overlayBounds = m_paytableOverlay.getGlobalBounds();

        float padding = 30.0f;
        float contentLeft = overlayBounds.left + padding;
        float contentTop = overlayBounds.top + padding;
        float contentWidth = overlayBounds.width - padding * 2.0f;
        float contentHeight = overlayBounds.height - padding * 2.0f;

        sf::View paytableView;
        paytableView.reset(sf::FloatRect(
            contentLeft,
            contentTop + m_paytableScrollOffset,
            contentWidth,
            contentHeight
        ));

        paytableView.setViewport(sf::FloatRect(
            contentLeft / static_cast<float>(m_window.getSize().x),
            contentTop / static_cast<float>(m_window.getSize().y),
            contentWidth / static_cast<float>(m_window.getSize().x),
            contentHeight / static_cast<float>(m_window.getSize().y)
        ));

        m_window.setView(paytableView);
        m_paytableText.setPosition(contentLeft, contentTop);
        m_window.draw(m_paytableText);

        m_window.setView(previousView);
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

void GameWindow::updateFreeSpinsText(int freeSpinsRemaining) {
    if (freeSpinsRemaining > 0) {
        std::ostringstream stream;
    stream << "FREE SPINS: " << freeSpinsRemaining;
    m_freeSpinsText.setString(stream.str());
    } else {
        m_freeSpinsText.setString("");
    }
}

void GameWindow::updateModeText(const std::string& modeText) {
    m_modeText.setString("Mode: " + modeText);
}

void GameWindow::updatePaytableText(const std::string& paytableText) {
    m_paytableText.setString(paytableText);

    sf::FloatRect overlayBounds = m_paytableOverlay.getGlobalBounds();
    sf::FloatRect textBounds = m_paytableText.getLocalBounds();

    float padding = 30.0f;
    float visibleHeight = overlayBounds.height - padding * 2.0f;
    float textHeight = textBounds.height;

    m_paytableMaxScrollOffset = textHeight - visibleHeight;

    if (m_paytableMaxScrollOffset < 0.0f) {
        m_paytableMaxScrollOffset = 0.0f;
    }

    if (m_paytableScrollOffset > m_paytableMaxScrollOffset) {
        m_paytableScrollOffset = m_paytableMaxScrollOffset;
    }
}

void GameWindow::showWinMessage(const std::string& message) {
    m_winMessageText.setString(message);

    sf::FloatRect textBounds = m_winMessageText.getLocalBounds();
    m_winMessageText.setOrigin(
        textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f
    );

    sf::FloatRect backgroundBounds = m_winMessageBackground.getGlobalBounds();
    m_winMessageText.setPosition(
        backgroundBounds.left + backgroundBounds.width / 2.0f,
        backgroundBounds.top + backgroundBounds.height / 2.0f
    );

    m_showWinMessage = true;
}

void GameWindow::clearWinMessage() {
    m_winMessageText.setString("");
    m_showWinMessage = false;
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

void GameWindow::setResetCallback(std::function<void()> callback) {
    m_resetCallback = std::move(callback);
}

void GameWindow::setHelpCallback(std::function<void()> callback) {
    m_helpCallback = std::move(callback);
}

void GameWindow::setEscapeCallback(std::function<void()> callback) {
    m_escapeCallback = std::move(callback);
}

void GameWindow::togglePaytable() {
    m_showPaytable = !m_showPaytable;
}

void GameWindow::closePaytable() {
    m_showPaytable = false;
}

bool GameWindow::isPaytableOpen() const {
    return m_showPaytable;
}

sf::RenderWindow& GameWindow::getWindow() {
    return m_window;
}