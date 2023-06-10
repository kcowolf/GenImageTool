#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include "TileSet.hpp"

namespace GenImageTool
{
    class TileMap
    {
    public:
        TileMap
            (
            std::size_t tileWidth,
            std::size_t tileHeight,
            const TileSet& tileSet
            );

        void addTileIndex
            (
            std::size_t tileIndex
            );

        std::size_t getTileHeight() const;

        std::size_t getTileIndex
            (
            std::size_t x,
            std::size_t y
            ) const;

        std::size_t getTileWidth() const;

        const TileSet& getTileSet() const;

        bool operator==(const TileMap& rhs) const;

    private:
        std::size_t m_tileHeight;
        std::size_t m_tileWidth;
        std::vector<std::size_t> m_tileIndexes;
        const TileSet& m_tileSet;
    };
}

#endif
