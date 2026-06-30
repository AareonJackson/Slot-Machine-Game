#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
public:
    Button(float x, float y, float width, float height, const std::string& text);

    // Set the function to execute when the button is clicked
    void setOnClick(std::function<void()> callback);
    void setText(const std::string& text);
    // Update hover state based on mouse position
    void update(sf::Vector2i mousePos);

    // Handle mouse click events
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    // Draw the button
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    sf::Font m_font;
    bool m_hasFont;

    // Colors for different button states
    sf::Color m_idleColor;
    sf::Color m_hoverColor;
    sf::Color m_activeColor;

    enum class State {Idle, Hover, Active};
    State m_state;

    // Callback function to execute on click
    std::function<void()> m_onClick;
};
