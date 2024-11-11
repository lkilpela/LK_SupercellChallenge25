#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class HealthBar : public sf::Drawable {
public:
    HealthBar(sf::Vector2f position, sf::Vector2f size);
    
    void setHealthBarSize(float healthPercentage);
    void setPosition(sf::Vector2f position);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RectangleShape m_healthBar;
    sf::RectangleShape m_healthBarBackground;
};