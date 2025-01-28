#include "TextureManager.hpp"


sf::Texture& TextureManager::getTexture(const std::string& filePath)
{
    const std::string realFilePath("res/" + filePath);

    // Search for the texture in the cache
    if (m_textures.find(realFilePath) == m_textures.end()) loadTexture(realFilePath);

    // Return the cached texture
    return m_textures[realFilePath];
}

void TextureManager::loadTexture(const std::string& realFilePath)
{
    sf::Texture texture;
    if (!texture.loadFromFile(realFilePath))
        throw std::runtime_error("Failed to load texture: " + realFilePath);
    std::cout << "Texture loaded successfully: " + realFilePath << std::endl;
    m_textures[realFilePath] = std::move(texture);
}