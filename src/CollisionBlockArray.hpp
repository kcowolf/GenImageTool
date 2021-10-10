#ifndef COLLISIONBLOCKARRAY_HPP
#define COLLISIONBLOCKARRAY_HPP

#include <array>
#include <stdint.h>
#include <vector>

namespace GenImageTool
{
    typedef std::array<uint8_t, 16> CollisionBlock;

    class CollisionBlockArray
    {
    public:
        std::size_t addCollisionBlock
            (
            const CollisionBlock& ceilingCollisionBlock,
            const CollisionBlock& floorCollisionBlock
            );

        std::size_t getBlockCount() const;

        const CollisionBlock& getCeilingCollisionBlock
            (
            std::size_t idx
            ) const;

        const CollisionBlock& getFloorCollisionBlock
            (
            std::size_t idx
            ) const;

    private:
        std::vector<CollisionBlock> m_ceilingCollisionBlocks;
        std::vector<CollisionBlock> m_floorCollisionBlocks;
    };
}

#endif
