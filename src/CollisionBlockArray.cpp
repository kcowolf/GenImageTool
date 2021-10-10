#include <stdexcept>
#include "CollisionBlockArray.hpp"

namespace GenImageTool
{
    std::size_t CollisionBlockArray::addCollisionBlock
        (
        const CollisionBlock& ceilingCollisionBlock,
        const CollisionBlock& floorCollisionBlock
        )
    {
        std::size_t index = m_ceilingCollisionBlocks.size();

        m_ceilingCollisionBlocks.push_back(ceilingCollisionBlock);
        m_floorCollisionBlocks.push_back(floorCollisionBlock);

        return index;
    }

    std::size_t CollisionBlockArray::getBlockCount() const
    {
        return m_ceilingCollisionBlocks.size();
    }

    const CollisionBlock& CollisionBlockArray::getCeilingCollisionBlock
        (
        std::size_t idx
        ) const
    {
        if (idx > m_ceilingCollisionBlocks.size())
        {
            throw std::runtime_error("Invalid ceiling collision block index.");
        }

        return m_ceilingCollisionBlocks[idx];
    }

    const CollisionBlock& CollisionBlockArray::getFloorCollisionBlock
        (
        std::size_t idx
        ) const
    {
        if (idx > m_floorCollisionBlocks.size())
        {
            throw std::runtime_error("Invalid floor collision block index.");
        }

        return m_floorCollisionBlocks[idx];
    }
}