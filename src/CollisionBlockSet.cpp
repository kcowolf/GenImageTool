#include <stdexcept>
#include "CollisionBlockSet.hpp"

namespace GenImageTool
{
	CollisionBlockSet::CollisionBlockSet
		(
		const uint16_t width,
		const uint16_t height
		)
		: m_height(height)
		, m_width(width)
	{
	}

	void CollisionBlockSet::addBlock
		(
		const CollisionBlock& block,
		const uint16_t value
		)
	{
		if (m_collisionBlocks.find(block) != m_collisionBlocks.end())
		{
			throw std::runtime_error("Block is already in collision block set.");
		}

		m_collisionBlocks.insert(std::pair(block, value));
	}

	bool CollisionBlockSet::find
        (
        const CollisionBlock& block,
        uint16_t& value
        ) const
	{
		std::map<CollisionBlock, uint16_t>::const_iterator it = m_collisionBlocks.find(block);

		if (it != m_collisionBlocks.end())
		{
			value = it->second;
			return true;
		}

		return false;
	}

	uint16_t CollisionBlockSet::getHeight() const
	{
		return m_height;
	}

	uint16_t CollisionBlockSet::getWidth() const
	{
		return m_width;
	}
}
