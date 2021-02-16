#include <stdexcept>
#include "Sprite.hpp"

namespace GenImageTool
{
    Sprite::Sprite
        (
        std::size_t tileWidth,
        std::size_t tileHeight,
        std::size_t startTileIdx
        )
        : m_startTileIdx(startTileIdx)
        , m_tileHeight(tileHeight)
        , m_tileWidth(tileWidth)
    {
    }

    std::size_t Sprite::getStartTileIdx() const
    {
        return m_startTileIdx;
    }

    std::size_t Sprite::getTileHeight() const
    {
        return m_tileHeight;
    }

    std::size_t Sprite::getTileWidth() const
    {
        return m_tileWidth;
    }
}
