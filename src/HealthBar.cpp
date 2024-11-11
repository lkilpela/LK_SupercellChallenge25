#include "HealthBar.h"
#include <SFML/Graphics/RenderTarget.hpp>

HealthBar::HealthBar(sf::Vector2f position, sf::Vector2f size)
: m_healthBarBackground(size), m_healthBar(size)
{
    m_healthBarBackground.setFillColor(sf::Color::Red);
    m_healthBarBackground.setPosition(position);

    m_healthBar.setFillColor(sf::Color::Green);
    m_healthBar.setPosition(position);
}

void HealthBar::setHealthBarSize(float healthPercentage)
{
    m_healthBar.setSize(sf::Vector2f(m_healthBarBackground.getSize().x * healthPercentage, m_healthBarBackground.getSize().y));
}

void HealthBar::setPosition(sf::Vector2f position)
{
    m_healthBarBackground.setPosition(position);
    m_healthBar.setPosition(position);
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_healthBarBackground, states);
    target.draw(m_healthBar, states);
}