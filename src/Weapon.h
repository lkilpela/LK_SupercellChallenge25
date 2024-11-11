#pragma once

#include "Rectangle.h"

class Weapon : public Rectangle
{
public:
    Weapon();
    virtual ~Weapon() {}

    void setActive(bool isActive);
    void update(float deltaTime);
    bool isActive() { return m_isActive; }

    void setPlayerPosition(sf::Vector2f playerPosition) { m_playerPosition = playerPosition; }  
    void setTargetPosition(sf::Vector2f targetPosition) { m_targetPosition = targetPosition; }
    void setRotationDirection(bool clockwise) { m_clockwise = clockwise; }
    float getDamage() { return m_damage; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    bool m_isActive = false;
    float m_timer = 0.0f;
    sf::Vector2f m_playerPosition;
    sf::Vector2f m_targetPosition;
    float m_rotationSpeed = 360.0f;
    float m_radius = 50.0f;
    float m_damage = 10.0f;
    bool m_clockwise = true;
};