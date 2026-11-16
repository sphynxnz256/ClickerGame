#pragma once

class TextureCache
{
private:
	std::unordered_map<std::string, sf::Texture> m_textures;

public:
	//returns the texture for this file, loading it the first time it's asked for
	const sf::Texture& get(const std::string& path);
};
