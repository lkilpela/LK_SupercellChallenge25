#pragma once

#include "Rectangle.h"
#include "Game.h"

class Collectible : public Rectangle {
public:
    enum class Type {
        HEALTHBOOST,
        DESTROYENEMY,
    };

    Collectible(Game* game, Type type, sf::Vector2f spawnPosition);

    void update(float deltaTime);

    Type getType() const { return m_type; }
    bool isCollected() const { return m_collected; };
    void hide();
private:
    Game*   m_pGame;
    Type m_type;
    bool m_collected = false;
};