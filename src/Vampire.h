#pragma once

#include "Rectangle.h"

class Game;
class Player;

class Vampire : public Rectangle
{
public:
    Vampire(Game* game, sf::Vector2f position);
    virtual ~Vampire() {}
    
    virtual void update(float deltaTime);

    void setIsKilled(bool isKilled) { m_isKilled = isKilled; }
    bool isKilled() { return m_isKilled; }

    void updateMove(Player *pPlayer, float deltaTime);

protected:
    Game* m_pGame;
    bool m_isKilled = false;
};
