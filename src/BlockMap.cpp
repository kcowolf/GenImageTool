#include <stdexcept>
#include "BlockMap.hpp"

namespace GenImageTool
{
	BlockMap::BlockMap
	(
		std::size_t blockWidth,
		std::size_t blockHeight,
		std::size_t blockTileWidth,
		std::size_t blockTileHeight,
		bool useShortIndexes
		)
		: m_blockHeight(blockHeight)
		, m_blockTileHeight(blockTileHeight)
		, m_blockTileWidth(blockTileWidth)
		, m_blockWidth(blockWidth)
		, m_useShortIndexes(useShortIndexes)
	{
		m_tileMapArrayIndexes.reserve(m_blockWidth * m_blockHeight);
	}

	void BlockMap::addTileMapArrayIndex
		(
		std::size_t tileMapArrayIndex
		)
	{
		if (m_tileMapArrayIndexes.size() >= m_blockWidth * m_blockHeight)
		{
			throw std::runtime_error("BlockMap has too many indexes.");
		}

		if (m_useShortIndexes && m_tileMapArrayIndexes.size() >= 255)
		{
			throw std::runtime_error("BlockMap has too many indexes for uint8_t.");
		}

		m_tileMapArrayIndexes.push_back(tileMapArrayIndex);
	}

	std::size_t BlockMap::getBlockHeight() const
	{
		return m_blockHeight;
	}

	std::size_t BlockMap::getBlockTileHeight() const
	{
		return m_blockTileHeight;
	}

	std::size_t BlockMap::getBlockTileWidth() const
	{
		return m_blockTileWidth;
	}

	std::size_t BlockMap::getBlockWidth() const
	{
		return m_blockWidth;
	}

	std::size_t BlockMap::getTileMapArrayIndex
		(
		std::size_t x,
		std::size_t y
		) const
	{
		if (m_tileMapArrayIndexes.size() != m_blockWidth * m_blockHeight)
		{
			throw std::runtime_error("BlockMap is incomplete.");
		}

		if (x >= m_blockWidth)
		{
			throw std::runtime_error("Invalid x coordinate");
		}

		if (y >= m_blockHeight)
		{
			throw std::runtime_error("Invalid y coordinate");
		}

		return m_tileMapArrayIndexes[(static_cast<std::size_t>(y) * m_blockWidth) + x];
	}

	bool BlockMap::getUseShortIndexes() const
	{
		return m_useShortIndexes;
	}
}
