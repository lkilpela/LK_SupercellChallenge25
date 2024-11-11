#include "ResourceManager.h"
#include <iostream>

// Get the singleton instance of the ResourceManager
ResourceManager& ResourceManager::getInstance() {
   static ResourceManager instance;
   return instance;
}

// TEXTURE MANAGER
bool ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
   sf::Texture texture;
   if (!texture.loadFromFile(filename)) {
      std::cerr << "Failed to load texture: " << filename << std::endl;
      return false;
   }
   m_Textures[name] = std::move(texture);
   return true;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
   return m_Textures.at(name);
}

// SOUND MANAGER
bool ResourceManager::loadSoundBuffer(const std::string& name, const std::string& filename) {
   sf::SoundBuffer soundBuffer;
   if (!soundBuffer.loadFromFile(filename)) {
      std::cerr << "Failed to load sound buffer: " << filename << std::endl;
      return false;
   }
   m_SoundBuffers[name] = std::move(soundBuffer);
   return true;
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& name) {
   return m_SoundBuffers.at(name);
}

// FONT MANAGER
bool ResourceManager::loadFont(const std::string& name, const std::string& filename) {
   sf::Font font;
   if (!font.loadFromFile(filename)) {
      std::cerr << "Failed to load font: " << filename << std::endl;
      return false;
   }
   m_Fonts[name] = std::move(font);
   return true;
}

sf::Font& ResourceManager::getFont(const std::string& name) {
   return m_Fonts.at(name);
}

// Load all resources needed for the game
void ResourceManager::loadAllResources() {
   loadFont("font", "assets/fonts/KOMIKAP_.ttf");
   //loadTexture("background", "assets/graphics/.png");
   loadTexture("player", "assets/graphics/player.png");
   loadTexture("enemy", "assets/graphics/enemy.png");
   loadTexture("health", "assets/graphics/health_pickup.png");
   //loadSoundBuffer("death", "assets/sound/death.wav");
   //loadSoundBuffer("hit", "assets/sound/hit.wav");
   loadSoundBuffer("pickup", "assets/sound/pickup.wav");
   //loadSoundBuffer("explosion", "assets/sounds/explosion.wav");
   // Add more resources as needed
}