#pragma once

#include "Vampire.h"
#include "Constants.h"
#include "HealthBar.h"

class BossVampire : public Vampire 
{
public:
    BossVampire(Game* game, sf::Vector2f position);

    void update(float deltaTime) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    int m_health = BossVampireHealth;
    HealthBar m_BossHealthBar;
    sf::Vector2f healthBarPosition;
    sf::Time m_LastHit;
};