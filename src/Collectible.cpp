#include "Collectible.h"
#include "Constants.h"
#include "Player.h"
#include "Vampire.h"

Collectible::Collectible(Game* game, Type type, sf::Vector2f spawnPosition)
: Rectangle(sf::Vector2f(50.0f, 50.0f), spawnPosition)
, m_pGame(game)
, m_type(type)
{
    if (m_type == Type::HEALTHBOOST)
    {
        m_sprite.setTexture(*m_pGame->getPlayerPickupTexture());
        m_sprite.setScale(1.5f, 1.5f);
    }
     else if (m_type == Type::DESTROYENEMY)
    {
        m_sprite.setTexture(*m_pGame->getDestroyPickupTexture());
        m_sprite.setScale(1.5f, 1.5f);
    }
    m_sprite.setPosition(getPosition());
}

void Collectible::hide()
{
    m_sprite.setColor(sf::Color(255, 255, 255, 0)); // Set the sprite color to fully transparent
}

void Collectible::update(float deltaTime)
{
    if (m_collected)
        return;

    Player* pPlayer = m_pGame->getPlayer();

    if (collidesWith(pPlayer))
    {
        m_collected = true;
        hide();
        if (m_type == Type::HEALTHBOOST)
        {
            pPlayer->heal(10);
            m_pGame->getPlayerPickupSound()->play();

        }
        else if (m_type == Type::DESTROYENEMY)
        {
            m_pGame->destroyAllEnemies();
        }  
    } 
}