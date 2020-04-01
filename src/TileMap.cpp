#include <stdexcept>
#include "TileMap.hpp"

namespace GenImageTool
{
	TileMap::TileMap
		(
		std::size_t tileWidth,
		std::size_t tileHeight
		)
		: m_tileHeight(tileHeight)
		, m_tileWidth(tileWidth)
	{
		m_tileIndexes.reserve(m_tileWidth * m_tileHeight);
	}

	void TileMap::addTileIndex
		(
		std::size_t tileIndex
		)
	{
		if (m_tileIndexes.size() >= m_tileWidth * m_tileHeight)
		{
			throw std::runtime_error("Tilemap has too many tiles.");
		}

		m_tileIndexes.push_back(tileIndex);
	}

	std::size_t TileMap::getTileIndex
		(
		std::size_t x,
		std::size_t y
		) const
	{
		if (m_tileIndexes.size() != m_tileWidth * m_tileHeight)
		{
			throw std::runtime_error("Tilemap is incomplete.");
		}

		if (x >= m_tileWidth)
		{
			throw std::runtime_error("Invalid x coordinate");
		}

		if (y >= m_tileHeight)
		{
			throw std::runtime_error("Invalid y coordinate");
		}

		return m_tileIndexes[(static_cast<std::size_t>(y) * m_tileWidth) + x];
	}

	std::size_t TileMap::getTileHeight() const
	{
		return m_tileHeight;
	}

	std::size_t TileMap::getTileWidth() const
	{
		return m_tileWidth;
	}
}
