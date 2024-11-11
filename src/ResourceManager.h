#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>


class ResourceManager {
private:
   ResourceManager() {}

   std::map<std::string, sf::Texture> m_Textures;
   std::map<std::string, sf::SoundBuffer> m_SoundBuffers;
   std::map<std::string, sf::Font> m_Fonts;
public:
   // Get the singleton instance of the ResourceManager
   static ResourceManager& getInstance();

   bool loadTexture(const std::string& name, const std::string& filename);
   bool loadSoundBuffer(const std::string& name, const std::string& filename);
   bool loadFont(const std::string& name, const std::string& filename);
   sf::Texture& getTexture(const std::string& name);
   sf::SoundBuffer& getSoundBuffer(const std::string& name);
   sf::Font& getFont(const std::string& name);

   void loadAllResources();
};
