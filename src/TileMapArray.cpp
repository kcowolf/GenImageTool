#include <stdexcept>
#include "TileMapArray.hpp"

namespace GenImageTool
{
	TileMapArray::TileMapArray
		(
		std::size_t tileWidth,
		std::size_t tileHeight
		)
		: m_tileHeight(tileHeight)
		, m_tileWidth(tileWidth)
	{
	}

	std::size_t TileMapArray::addTileMap
		(
		const TileMap& tileMap
		)
	{
		std::size_t index = m_tileMaps.size();

		if (tileMap.getTileWidth() != m_tileWidth || tileMap.getTileHeight() != m_tileHeight)
		{
			throw std::runtime_error("Tilemap size does not match.");
		}

		m_tileMaps.push_back(tileMap);
		return index;
	}

	bool TileMapArray::find
		(
		const TileMap& tileMap,
		std::size_t& index
		) const
	{
		for (std::size_t i = 0; i < m_tileMaps.size(); i++)
		{
			if (m_tileMaps[i] == tileMap)
			{
				index = i;
				return true;
			}
		}

		return false;
	}


	std::size_t TileMapArray::getTileHeight() const
	{
		return m_tileHeight;
	}

	TileMap TileMapArray::getTileMap
		(
		std::size_t idx
		) const
	{
		if (idx >= m_tileMaps.size())
		{
			throw std::runtime_error("Invalid tilemap index.");
		}

		return m_tileMaps[idx];
	}

	std::size_t TileMapArray::getTileMapCount() const
	{
		return m_tileMaps.size();
	}

	std::size_t TileMapArray::getTileWidth() const
	{
		return m_tileWidth;
	}
}
