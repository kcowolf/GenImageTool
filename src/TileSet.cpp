#include <stdexcept>
#include "TileSet.hpp"

namespace GenImageTool
{
	void TileSet::addTile
		(
		std::string tile
		)
	{
		m_tiles.push_back(tile);
	}

	std::size_t TileSet::getSize() const
	{
		return m_tiles.size();
	}

	std::string TileSet::getTile
		(
		std::size_t index
		) const
	{
		if (index >= m_tiles.size())
		{
			throw std::runtime_error("Invalid tile index.");
		}

		return m_tiles[index];
	}

	uint16_t TileSet::getTileIndex
		(
		std::string tile
		)
	{
		for (uint16_t i = 0; i < m_tiles.size(); i++)
		{
			if (m_tiles[i] == tile)
			{
				return i;
			}
		}

		addTile(tile);

		return static_cast<uint16_t>(m_tiles.size() - 1);
	}

}
