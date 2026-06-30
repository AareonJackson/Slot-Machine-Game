#include "ui/button.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, const std::string &text)
    : m_state(State::Idle) {

    // Classic Casino Red Button
    m_idleColor = sf::Color(200, 0, 0);
    m_hoverColor = sf::Color(250, 50, 50);
    m_activeColor = sf::Color(130, 0, 0);

    m_shape.setPosition(x, y);
    m_shape.setSize(sf::Vector2f(width, height));
    m_shape.setFillColor(m_idleColor);
    m_shape.setOutlineColor(sf::Color(255, 215, 0));
    m_shape.setOutlineThickness(3.0f);

    // Load Font
    m_hasFont = m_font.loadFromFile("assets/fonts/Roboto-Regular.ttf");
    if (!m_hasFont) m_hasFont = m_font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
    if (!m_hasFont) m_hasFont = m_font.loadFromFile("/Library/Fonts/Arial.ttf");
    if (!m_hasFont) m_hasFont = m_font.loadFromFile("/System/Library/Fonts/Monaco.ttf");

    if (m_hasFont) {
        m_text.setFont(m_font);
        m_text.setString(text);
        m_text.setCharacterSize(24);
        m_text.setFillColor(sf::Color::White);
        m_text.setStyle(sf::Text::Bold);

        // Center the text inside the button
        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        m_text.setPosition(x + width / 2.0f, y + height / 2.0f);
    }
}

void Button::setOnClick(std::function<void()> callback) {
    m_onClick = callback;
}

void Button::setText(const std::string& text) {
    if (!m_hasFont) {
        return;
    }

    m_text.setString(text);
    sf::FloatRect textRect = m_text.getLocalBounds(); // set origin and position after setting text
    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    sf::FloatRect buttonBound = m_shape.getGlobalBounds();
    m_text.setPosition(buttonBound.left + buttonBound.width / 2.0f, buttonBound.top + buttonBound.height / 2.0f);
}

void Button::update(sf::Vector2i mousePos) {
    bool isHovered = m_shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    if (m_state != State::Active) {
        if (isHovered) {
            m_state = State::Hover;
            m_shape.setFillColor(m_hoverColor);
        } else {
            m_state = State::Idle;
            m_shape.setFillColor(m_idleColor);
        }
    }
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool isHovered = m_shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (isHovered) {
            m_state = State::Active;
            m_shape.setFillColor(m_activeColor);
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (m_state == State::Active) {
            if (isHovered) {
                // Button was clicked successfully!
                if (m_onClick) {
                    m_onClick();
                }
                m_state = State::Hover;
                m_shape.setFillColor(m_hoverColor);
            } else {
                m_state = State::Idle;
                m_shape.setFillColor(m_idleColor);
            }
        }
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
    if (m_hasFont) {
        window.draw(m_text);
    }
}
