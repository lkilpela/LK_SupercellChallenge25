#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Rectangle.h"
#include "Vampire.h"
#include "Collectible.h"
#include "BossVampire.h"

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_playerHealthBar(sf::Vector2f (ScreenWidth / 4, ScreenHeight - 20.0f),sf::Vector2f (ScreenWidth / 2, 20.0f))
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
    loadHighScore();
}

Game::~Game()
{
    
}

bool Game::initialise()
{
    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerPickupTexture.loadFromFile(ResourceManager::getFilePath("health_pickup.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_destroyPickupTexture.loadFromFile(ResourceManager::getFilePath("crosshair.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerHitBuffer.loadFromFile(ResourceManager::getFilePath("hit.wav")))
    {
        std::cerr << "Unable to load sound" << std::endl;
        return false;
    }
    if (!m_playerPickupBuffer.loadFromFile(ResourceManager::getFilePath("pickup.wav")))
    {
        std::cerr << "Unable to load sound" << std::endl;
        return false;
    }
    if(!m_weaponBuffer.loadFromFile(ResourceManager::getFilePath("weapon.wav")))
    {
        std::cerr << "Unable to load sound" << std::endl;
        return false;
    }
    m_playerHitSound.setBuffer(m_playerHitBuffer);
    m_playerPickupSound.setBuffer(m_playerPickupBuffer);
    m_weaponSound.setBuffer(m_weaponBuffer);
    resetLevel();
    return true;
}

void Game::resetLevel()
{
    m_pVampires.clear();
    m_pCollectibles.clear();
    m_pBossVampire.reset();

    m_vampireCooldown = 2.0f;
    m_nextVampireCooldown = 2.0f;
    m_spawnCount = 0;
    m_bossVampireCooldown = 5.0f;
    m_collectibleCooldown = 5.0f;
    m_nextCollectibleCooldown = 5.0f;

    m_score = 0;

    m_pPlayer->initialise();
    m_playerHealthBar.setHealthBarSize(1.0f);

    m_pClock->restart();
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (m_pClock->getElapsedTime().asSeconds() >= 3.f)
            {
                m_state = State::ACTIVE;
                m_pClock->restart();
            }
        }
        break;
            
        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->update(deltaTime);

            vampireSpawner(deltaTime);
            collectibleSpawner(deltaTime);

            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
                handleVampireCollision(temp.get(), 10.0f);
            }
            if (m_pBossVampire)
            {
                m_pBossVampire->update(deltaTime);
                handleVampireCollision(m_pBossVampire.get(), 20.0f);
                if (m_pBossVampire->isKilled())
                {
                    m_pBossVampire.reset();
                    m_bossVampireCooldown = 5.0f;
                }
            }

            for (auto it = m_pCollectibles.begin(); it != m_pCollectibles.end();)
            {
                handleCollectibleUpdate(it->get(), deltaTime);
                if (it->get()->isCollected())
                {
                    it = m_pCollectibles.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (m_pPlayer->isDead())
            {
                m_state = State::WAITING;
                saveHighScore();
                resetLevel();
            }
        }
        break;
    }

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            updateScore(10);
            continue;
        }
        i++;
    }
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //  Draw texts.
    if (m_state == State::WAITING)
    {
        drawStartText(target, states);
    }
    else
    {
        drawTimerText(target, states);
        drawScoreText(target, states);
        drawDestroyEnemyText(target, states);
        drawAttackInstructionText(target, states);
    }

    // Draw player.
    m_pPlayer->draw(target, states);

    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }

    //  Draw collectibles.
    for (auto& temp : m_pCollectibles)
    {
        temp->draw(target, states);
    }

    //  Draw health bar.
    m_playerHealthBar.draw(target, states);

    //  Draw Boss Vampire.
    if (m_pBossVampire)
    {
        m_pBossVampire->draw(target, states);
    }

}

void Game::drawStartText(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Text startText;
    startText.setFont(m_font);
    startText.setString("Game Start!");
    startText.setFillColor(sf::Color::White);
    startText.setPosition(80.0f, 80.0f);
    startText.setStyle(sf::Text::Bold);
    target.draw(startText, states);
}

void Game::drawTimerText(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Text timerText;
    timerText.setFont(m_font);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);
    timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
    timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
    target.draw(timerText, states);
}

void Game::drawDestroyEnemyText(sf::RenderTarget &target, sf::RenderStates states) const
{
    bool showDestroyEnemyText = false;
    for (auto& temp : m_pCollectibles)
    {
        if (temp->getType() == Collectible::Type::DESTROYENEMY)
        {
            showDestroyEnemyText = true;
            break;
        }
    }
    if (showDestroyEnemyText)
    {
        sf::Text destroyEnemyText;
        destroyEnemyText.setFont(m_font);
        destroyEnemyText.setString("Collect crosshair to destroy all enemies");
        destroyEnemyText.setCharacterSize(24);
        destroyEnemyText.setFillColor(sf::Color::White);
        destroyEnemyText.setStyle(sf::Text::Bold);
        destroyEnemyText.setPosition(ScreenWidth / 2 - destroyEnemyText.getGlobalBounds().width / 2, 50);
        target.draw(destroyEnemyText, states);
    }
}

void Game::drawAttackInstructionText(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Text attackInstructionText;
    attackInstructionText.setFont(m_font);
    attackInstructionText.setString("Press SPACE to attack Vampires");
    attackInstructionText.setCharacterSize(24);
    attackInstructionText.setFillColor(sf::Color::White);
    attackInstructionText.setStyle(sf::Text::Bold);
    attackInstructionText.setPosition(25.0f, 20.0f);
    target.draw(attackInstructionText, states);
}

void Game::drawScoreText(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Text scoreText;
    sf::Text highScoreText;
    
    scoreText.setString("Score " + std::to_string(m_score));
    highScoreText.setString("High Score " + std::to_string(m_highScore));

    scoreText.setFont(m_font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(100.0f, 50.0f);

    highScoreText.setFont(m_font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setStyle(sf::Text::Bold);
    highScoreText.setPosition(ScreenWidth - 250.0f, 10.0f);

    target.draw(scoreText, states);
    target.draw(highScoreText, states);
}

void Game::loadHighScore()
{
    std::ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> m_highScore;
        file.close();
    }
}

void Game::saveHighScore()
{
    std::ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << m_highScore;
        file.close();
    }
}

void Game::updateScore(int points)
{
    m_score += points;
    if (m_score > m_highScore)
    {
        m_highScore = m_score;
    }
}

void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

Player* Game::getPlayer() const 
{
    return m_pPlayer.get();
}

void Game::vampireSpawner(float deltaTime)
{
    m_bossVampireCooldown -= deltaTime;
    if (m_vampireCooldown > 0.0f)
    {
        m_vampireCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    float distToRight = (float) ScreenWidth - randomXPos;
    float distToBottom = (float) ScreenHeight - randomYPos;

    float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
    float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

    if (abs(xMinDist) < abs(yMinDist))
        randomXPos += xMinDist;
    else
        randomYPos += yMinDist;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    if (m_bossVampireCooldown <= 0.0f && !m_pBossVampire)
    {
        m_pBossVampire = std::make_unique<BossVampire>(this, spawnPosition);
    }
    else 
        m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_spawnCount % 5 == 0 && m_nextVampireCooldown > 0.3f)
    {
        m_nextVampireCooldown -= 0.1f;
    }
    m_vampireCooldown = m_nextVampireCooldown;
}

void Game::collectibleSpawner(float deltaTime)
{
    if (m_collectibleCooldown > 0.0f)
    {
        m_collectibleCooldown -= deltaTime;
        return;
    }
    if (m_pCollectibles.size() > 2) {
        return;
    }

    sf::Vector2f spawnPosition;
    bool validPosition = false;
    float minDistance = 30.0f; // Minimum distance from the player
    float margin = 20.0f; // Margin from the screen edge

    while (!validPosition)
    {
        float randomXPos = margin + static_cast<float>(rand() % static_cast<int>(ScreenWidth - 2 * margin));
        float randomYPos = margin + static_cast<float>(rand() % static_cast<int>(ScreenHeight - 2 * margin));

        spawnPosition = sf::Vector2f(randomXPos, randomYPos);

        // Check distance from player
        sf::Vector2f playerPosition = m_pPlayer->getPosition();
        float distance = sqrt(pow(spawnPosition.x - playerPosition.x, 2) + pow(spawnPosition.y - playerPosition.y, 2));

        if (distance >= minDistance)
        {
            validPosition = true;
        }
    }

    Collectible::Type type = Collectible::Type::HEALTHBOOST;
    if (rand() % 3 == 0)
    {
        type = Collectible::Type::DESTROYENEMY;
    }
    m_pCollectibles.push_back(std::make_unique<Collectible>(this, type, spawnPosition));
    m_collectibleCooldown = m_nextCollectibleCooldown;
}

void Game::handleVampireCollision(Vampire* vampire, float damage)
{
    if (vampire->collidesWith(m_pPlayer.get()))
    {
        if (m_pPlayer->hit(damage))
        {
            m_playerHitSound.play();
            m_playerHealthBar.setHealthBarSize((float)m_pPlayer->getHealth() / m_pPlayer->getMaxHealth());
        }
    }
}

void Game::handleCollectibleUpdate(Collectible* collectible, float deltaTime)
{
    collectible->update(deltaTime);
    if (collectible->isCollected())
    {
        m_playerHealthBar.setHealthBarSize((float)m_pPlayer->getHealth() / m_pPlayer->getMaxHealth());
        m_playerPickupSound.play();
    }
}

void Game::destroyAllEnemies()
{
    for (auto& temp : m_pVampires)
    {
        temp->setIsKilled(true);
    }
    if (m_pBossVampire)
    {
        m_pBossVampire->setIsKilled(true);
        m_pBossVampire.reset();
        m_bossVampireCooldown = 5.0f;
    }
}