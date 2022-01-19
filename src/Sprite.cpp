#include <stdexcept>
#include "Genesis.hpp"
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
        if (m_tileWidth < SPRITE_MIN_DIMENSION || m_tileWidth > SPRITE_MAX_DIMENSION)
        {
            throw std::runtime_error("Invalid sprite width.");
        }

        if (m_tileHeight < SPRITE_MIN_DIMENSION || m_tileHeight > SPRITE_MAX_DIMENSION)
        {
            throw std::runtime_error("Invalid sprite height.");
        }
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
