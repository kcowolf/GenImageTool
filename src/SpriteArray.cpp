#include <stdexcept>
#include "SpriteArray.hpp"

namespace GenImageTool
{
    SpriteArray::SpriteArray
        (
        std::size_t tileWidth,
        std::size_t tileHeight
        )
        : m_tileHeight(tileHeight)
        , m_tileWidth(tileWidth)
    {
    }

    void SpriteArray::addSprite
        (
        const Sprite& sprite
        )
    {
        if (sprite.getTileWidth() != m_tileWidth || sprite.getTileHeight() != m_tileHeight)
        {
            throw std::runtime_error("Sprite size does not match.");
        }

        m_sprites.push_back(sprite);
    }

    Sprite SpriteArray::getSprite
        (
        std::size_t idx
        ) const
    {
        if (idx >= m_sprites.size())
        {
            throw std::runtime_error("Invalid sprite index.");
        }

        return m_sprites[idx];
    }

    std::size_t SpriteArray::getSpriteCount() const
    {
        return m_sprites.size();
    }

    std::size_t SpriteArray::getTileHeight() const
    {
        return m_tileHeight;
    }

    std::size_t SpriteArray::getTileWidth() const
    {
        return m_tileWidth;
    }
}
