#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include "Constants.h"
#include "HealthBar.h"

class Player;
class Game;
class GameInput;
class Vampire;
class Collectible;
class BossVampire;

namespace sf { class Clock; }

class Game : public sf::Drawable
{
public:
    
    enum class State
    {
        WAITING,
        ACTIVE,
    };
    
    Game();
    ~Game();
    
    bool initialise();
    void update(float deltaTime);
    void resetLevel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
    State getState() const { return m_state; }
    sf::Time getGameTimeTotal() const { return m_pClock->getElapsedTime(); }
    
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);

    Player* getPlayer() const;
    sf::Texture* getPlayerTexture() { return &m_playerTexture; }
    sf::Texture* getVampireTexture() { return &m_vampTexture; }
    sf::Texture* getPlayerPickupTexture() { return &m_playerPickupTexture; }
    sf::Texture* getDestroyPickupTexture() { return &m_destroyPickupTexture; }
    sf::Sound* getPlayerHitSound() { return &m_playerHitSound; }
    sf::Sound* getPlayerPickupSound() { return &m_playerPickupSound; }
    sf::Sound* getWeaponSound() { return &m_weaponSound; }

    void drawStartText(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawTimerText(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawDestroyEnemyText(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawAttackInstructionText(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawScoreText(sf::RenderTarget &target, sf::RenderStates states) const;

    void vampireSpawner(float deltaTime);
    void collectibleSpawner(float deltaTime);
    void loadHighScore();
    void saveHighScore();
    void updateScore(int points);
    void handleVampireCollision(Vampire* vampire, float damage);
    void handleCollectibleUpdate(Collectible* collectible, float deltaTime);
    void destroyAllEnemies();

    const std::vector<std::unique_ptr<Vampire>>& getEnemies() const { return m_pVampires; }

private:
    std::unique_ptr<Player> m_pPlayer;

    std::vector<std::unique_ptr<Vampire>> m_pVampires;

    std::unique_ptr<BossVampire> m_pBossVampire;

    std::vector<std::unique_ptr<Collectible>> m_pCollectibles;

    State m_state;
    sf::Time gameTimeTotal;
    std::unique_ptr<sf::Clock> m_pClock;
    std::unique_ptr<GameInput> m_pGameInput;

    float m_vampireCooldown = 2.0f;
    float m_nextVampireCooldown = 2.0f;
    int m_spawnCount = 0;

    float m_bossVampireCooldown = 10.0f;

    float m_collectibleCooldown = 5.0f;
    float m_nextCollectibleCooldown = 5.0f;

    HealthBar m_playerHealthBar;

    sf::Font m_font;
    sf::Texture m_vampTexture;
    sf::Texture m_playerTexture;
    sf::Texture m_playerPickupTexture;
    sf::Texture m_destroyPickupTexture;

    sf::SoundBuffer m_playerHitBuffer;
    sf::SoundBuffer m_playerPickupBuffer;
    sf::SoundBuffer m_weaponBuffer;
    sf::Sound m_playerHitSound;
    sf::Sound m_playerPickupSound;
    sf::Sound m_weaponSound;

    int m_score = 0;
    int m_highScore = 0;
};
