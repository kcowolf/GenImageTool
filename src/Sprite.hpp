#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
namespace GenImageTool
{
    class Sprite
    {
    public:
        Sprite
            (
            std::size_t tileWidth,
            std::size_t tileHeight,
            std::size_t startTileIdx
            );

        std::size_t getStartTileIdx() const;

        std::size_t getTileHeight() const;

        std::size_t getTileWidth() const;

    private:
        std::size_t m_startTileIdx;
        std::size_t m_tileHeight;
        std::size_t m_tileWidth;
    };
}

#endif
