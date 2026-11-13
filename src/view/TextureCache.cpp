#include "stdafx.h"
#include "view/TextureCache.h"

const sf::Texture& TextureCache::get(const std::string& path)
{
	auto it = m_textures.find(path);
	if (it != m_textures.end())
	{
		return it->second;
	}

	//keep the empty texture in the map if loading fails
	sf::Texture& texture = m_textures.try_emplace(path).first->second;
	if (!texture.loadFromFile(path))
	{
		std::cout << "ERROR::TEXTURECACHE::GET::Failed to load " << path << '\n';
	}
	return texture;
}
