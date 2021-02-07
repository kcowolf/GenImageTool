#ifndef TILEMAPARRAY_HPP
#define TILEMAPARRAY_HPP

#include "TileMap.hpp"

#include <vector>
namespace GenImageTool
{
    class TileMapArray
    {
    public:
        TileMapArray
            (
            std::size_t tileWidth,
            std::size_t tileHeight
            );

        std::size_t addTileMap
            (
            const TileMap& tileMap
            );

        bool find
        (
            const TileMap& tileMap,
            std::size_t& index
        ) const;

        std::size_t getTileHeight() const;

        TileMap getTileMap
            (
            std::size_t idx
            ) const;

        std::size_t getTileMapCount() const;

        std::size_t getTileWidth() const;

    private:
        std::size_t m_tileHeight;
        std::size_t m_tileWidth;
        std::vector<TileMap> m_tileMaps;
    };
}

#endif
