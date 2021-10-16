#include <stdexcept>
#include "CollisionBlockArray.hpp"

namespace GenImageTool
{
    std::size_t CollisionBlockArray::addCollisionBlock
        (
        const CollisionBlock& ceilingCollisionBlock,
        const CollisionBlock& floorCollisionBlock,
        const CollisionBlock& leftCollisionBlock,
        const CollisionBlock& rightCollisionBlock
        )
    {
        std::size_t index = m_ceilingCollisionBlocks.size();

        m_ceilingCollisionBlocks.push_back(ceilingCollisionBlock);
        m_floorCollisionBlocks.push_back(floorCollisionBlock);
        m_leftCollisionBlocks.push_back(leftCollisionBlock);
        m_rightCollisionBlocks.push_back(rightCollisionBlock);

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

    const CollisionBlock& CollisionBlockArray::getLeftCollisionBlock
        (
        std::size_t idx
        ) const
    {
        if (idx > m_leftCollisionBlocks.size())
        {
            throw std::runtime_error("Invalid left collision block index.");
        }

        return m_leftCollisionBlocks[idx];
    }

    const CollisionBlock& CollisionBlockArray::getRightCollisionBlock
        (
        std::size_t idx
        ) const
    {
        if (idx > m_rightCollisionBlocks.size())
        {
            throw std::runtime_error("Invalid right collision block index.");
        }

        return m_rightCollisionBlocks[idx];
    }
}
