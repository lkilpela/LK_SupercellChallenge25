#include "Weapon.h"
#include "Constants.h"
#include <cmath>

Weapon::Weapon() : Rectangle(sf::Vector2f(0, 0))
{
    setPosition(sf::Vector2f(ScreenWidth * 0.5f, ScreenHeight * 0.5f));
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    setColor(sf::Color::Blue);
}

void Weapon::setActive(bool isActive)
{
    m_isActive = isActive;
    if (isActive)
    {
        setSize(sf::Vector2f(WeaponWidth, WeaponHeight));
        m_timer = WeaponActiveTime;
    }
    else
    {
        setSize(sf::Vector2f(0.0f, 0.0f));
        m_timer = 0.0f;
    }
}

void Weapon::update(float deltaTime)
{
    if (m_isActive)
    {
        m_timer -= deltaTime;
        if (m_timer <= 0.0f)
        {
            setActive(false);
        } 
        else
        {
            float angle = m_timer * m_rotationSpeed / WeaponActiveTime;
            if (!m_clockwise)
            {
                angle = -angle;
            }
            float radianAngle = angle * (pi / 180.0f);

            float offsetX = m_radius * cos(radianAngle);
            float offsetY = m_radius * sin(radianAngle);

            setPosition(m_playerPosition + sf::Vector2f(offsetX, offsetY));
            setRotation(angle);
        }
    }
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape graphicsRect(getSize());
    graphicsRect.setFillColor(getColor());
    graphicsRect.setPosition(getPosition());
    target.draw(graphicsRect);
}