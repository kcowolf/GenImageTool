#include <stdexcept>
#include "TileMapArrayMap.hpp"

namespace GenImageTool
{
	TileMapArrayMap::TileMapArrayMap
	(
		std::size_t mapWidth,
		std::size_t mapHeight
		)
		: m_mapHeight(mapHeight)
		, m_mapWidth(mapWidth)
	{
		m_tileMapArrayIndexes.reserve(m_mapWidth * m_mapHeight);
	}

	void TileMapArrayMap::addTileMapArrayIndex
		(
		std::size_t tileMapArrayIndex
		)
	{
		if (m_tileMapArrayIndexes.size() >= m_mapWidth * m_mapHeight)
		{
			throw std::runtime_error("TilemapArrayMap has too many indexes.");
		}

		m_tileMapArrayIndexes.push_back(tileMapArrayIndex);
	}

	std::size_t TileMapArrayMap::getTileMapArrayIndex
		(
		std::size_t x,
		std::size_t y
		) const
	{
		if (m_tileMapArrayIndexes.size() != m_mapWidth * m_mapHeight)
		{
			throw std::runtime_error("TilemapArrayMap is incomplete.");
		}

		if (x >= m_mapWidth)
		{
			throw std::runtime_error("Invalid x coordinate");
		}

		if (y >= m_mapHeight)
		{
			throw std::runtime_error("Invalid y coordinate");
		}

		return m_tileMapArrayIndexes[(static_cast<std::size_t>(y) * m_mapWidth) + x];
	}

	std::size_t TileMapArrayMap::getMapHeight() const
	{
		return m_mapHeight;
	}

	std::size_t TileMapArrayMap::getMapWidth() const
	{
		return m_mapWidth;
	}
}
