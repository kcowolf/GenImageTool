#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
namespace GenImageTool
{
    class TileMap
    {
    public:
        TileMap
            (
            std::size_t tileWidth,
            std::size_t tileHeight
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

        bool operator==(const TileMap& rhs) const;

    private:
        std::size_t m_tileHeight;
        std::size_t m_tileWidth;
        std::vector<std::size_t> m_tileIndexes;
    };
}

#endif
