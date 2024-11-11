#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"
#include "Vampire.h"
#include <iostream>

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>()),
    m_health(PlayerMaxHealth),
    m_maxHealth(PlayerMaxHealth)
{
    setOrigin(sf::Vector2f(0.0f, 0.0f));
}

Player::~Player()
{
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
    m_health = PlayerMaxHealth;
    m_maxHealth = PlayerMaxHealth;
    m_LastHit = sf::Time::Zero;

    //m_sprite.setBuffer(*m_pGame->getPlayerHitSound());


    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    
    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;

    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;
    
    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
    sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
    float clampedX = std::clamp(getPosition().x, 0.0f, static_cast<float>(ScreenWidth) - spriteBounds.width);
    float clampedY = std::clamp(getPosition().y, 0.0f, static_cast<float>(ScreenHeight) - spriteBounds.height);
    setPosition(clampedX, clampedY);

    if (m_pWeapon->isActive() == false)
    {
        if (inputData.m_movingLeft == true && inputData.m_movingRight == false)
            m_direction = LEFT;
        else if (inputData.m_movingLeft == false && inputData.m_movingRight == true)
            m_direction = RIGHT;
    }
}

void Player::attack()
{
    m_pWeapon->setActive(true);
    auto sound = m_pGame->getWeaponSound();
    if (sound->getStatus() == sf::Sound::Status::Stopped)
        sound->play();
}

bool Player::hit(int damage)
{
    if (m_isDead)
        return false;
    sf::Time time = m_pGame->getGameTimeTotal();
    if (time - m_LastHit > sf::seconds(0.5f))
    { 
        m_health -= damage;
        if (m_health <= 0)
        {
            m_health = 0;
            setIsDead(true);
        }
        m_LastHit = time;
        return true;
    }
    return false;
}

void Player::heal(int health)
{
    m_health += health;
    if (m_health > m_maxHealth)
        m_health = m_maxHealth;
}

void Player::update(float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();

    m_sprite.setPosition(getPosition());

    bool clockwise = (m_direction == RIGHT);
    m_pWeapon->setRotationDirection(clockwise);
    
    m_pWeapon->setPlayerPosition(getPosition());
    m_pWeapon->update(deltaTime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
}