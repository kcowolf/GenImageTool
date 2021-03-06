#ifndef TILESET_HPP
#define TILESET_HPP

#include <map>
#include <string>
#include <vector>

namespace GenImageTool
{
    class TileSet
    {
    public:
        std::size_t addTile
            (
            const std::string& tile
            );

        void clear();

        bool find
            (
            const std::string& tile,
            std::size_t& index
            ) const;

        bool findTileSet
            (
            const TileSet& tileSet,
            std::size_t& startIdx
            ) const;

        std::size_t getSize() const;

        std::string getTile
            (
            std::size_t index
            ) const;

    private:
        std::vector<std::string> m_tiles;
        std::map<std::string, std::size_t> m_tileTransformations;
    };
}

#endif
