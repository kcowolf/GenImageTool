#include <stdexcept>
#include "CollisionBlockMap.hpp"

namespace GenImageTool
{
	CollisionBlockMap::CollisionBlockMap
		(
		std::size_t blockWidth,
		std::size_t blockHeight
		)
		: m_blockHeight(blockHeight)
		, m_blockWidth(blockWidth)
	{
		m_collisionBlockMapValues.reserve(blockHeight * blockWidth);
	}

	void CollisionBlockMap::addCollisionBlockValue
		(
		uint16_t value
		)
	{
		if (m_collisionBlockMapValues.size() >= m_blockWidth * m_blockHeight)
		{
			throw std::runtime_error("CollisionBlockMap has too many indexes.");
		}

		m_collisionBlockMapValues.push_back(value);
	}

	std::size_t CollisionBlockMap::getBlockHeight() const
	{
		return m_blockHeight;
	}

	std::size_t CollisionBlockMap::getBlockWidth() const
	{
		return m_blockHeight;
	}

	uint16_t CollisionBlockMap::getCollisionBlockValue
        (
        std::size_t x,
        std::size_t y
        ) const
    {
        if (m_collisionBlockMapValues.size() != m_blockWidth * m_blockHeight)
        {
            throw std::runtime_error("CollisionBlockMap is incomplete.");
        }

        if (x >= m_blockWidth)
        {
            throw std::runtime_error("Invalid x coordinate");
        }

        if (y >= m_blockHeight)
        {
            throw std::runtime_error("Invalid y coordinate");
        }

        return m_collisionBlockMapValues[(static_cast<std::size_t>(y) * m_blockWidth) + x];
    }
}
