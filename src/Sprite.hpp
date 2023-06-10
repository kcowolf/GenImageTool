#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
#include "TileSet.hpp"

namespace GenImageTool
{
    class Sprite
    {
    public:
        Sprite
            (
            std::size_t tileWidth,
            std::size_t tileHeight,
            std::size_t startTileIdx,
            const TileSet& tileSet
            );

        std::size_t getStartTileIdx() const;

        std::size_t getTileHeight() const;

        std::size_t getTileWidth() const;

        const TileSet& getTileSet() const;
    private:
        std::size_t m_startTileIdx;
        std::size_t m_tileHeight;
        std::size_t m_tileWidth;
        const TileSet& m_tileSet;
    };
}

#endif
