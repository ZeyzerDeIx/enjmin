#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

/**
 * @class TextureManager
 * @brief A utility class to manage textures, with a static cache.
 *
 * The TextureManager class provides static access to textures in a global cache.
 * Textures are loaded from files the first time they are requested and cached for later use.
 */
class TextureManager
{
public:
    /**
     * @brief Retrieve a texture, loading it if it hasn't been loaded yet.
     *
     * If the texture has already been loaded (i.e., it exists in the cache),
     * it will be returned immediately. Otherwise, the texture is loaded from the
     * file system and added to the cache.
     *
     * @param filepath The path to the texture file.
     * @return A reference to the loaded texture.
     * @throws std::runtime_error If the texture cannot be loaded from the specified file path.
     */
    sf::Texture& getTexture(const std::string& filePath);

private:
    // Cache of textures, indexed by file path
    std::map<std::string, sf::Texture> m_textures;

    void loadTexture(const std::string& realFilePath);
};
