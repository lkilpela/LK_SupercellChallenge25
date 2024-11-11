#include "BossVampire.h"
#include "Game.h"
#include "Player.h"
#include "Weapon.h"
#include <iostream>

BossVampire::BossVampire(Game* game, sf::Vector2f position)
: Vampire(game, position)
, m_BossHealthBar(position, sf::Vector2f(100.0f, 10.0f))
{
    m_sprite.setColor(sf::Color::Red);
    m_sprite.setScale(6.0f, 6.0f);

    setSize(m_sprite.getGlobalBounds().getSize());

    healthBarPosition = getPosition() - sf::Vector2f(0, m_sprite.getGlobalBounds().height / 2 + 10);
    m_BossHealthBar.setPosition(healthBarPosition);
}

void BossVampire::update(float deltaTime)
{
    if (m_isKilled)
        return;
    
    Player* pPlayer = m_pGame->getPlayer();
    sf::Time time = m_pGame->getGameTimeTotal();

    if (collidesWith(pPlayer->getWeapon()))
    {
        if (time - m_LastHit > sf::seconds(0.5f)) 
        {
            m_health -= pPlayer->getWeapon()->getDamage();
            m_BossHealthBar.setHealthBarSize(m_health / BossVampireHealth);
            if (m_health <= 0)
            {
                setIsKilled(true);
            }
            m_LastHit = time;
            return;
        }
    }
    // Update the health bar position to follow the Boss Vampire
    healthBarPosition = getPosition() - sf::Vector2f(0, m_sprite.getGlobalBounds().height / 2 + 10);
    m_BossHealthBar.setPosition(healthBarPosition);

    updateMove(pPlayer, deltaTime);

}

void BossVampire::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    m_BossHealthBar.draw(target, states);
}
