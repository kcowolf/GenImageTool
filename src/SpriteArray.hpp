#ifndef SPRITEARRAY_HPP
#define SPRITEARRAY_HPP

#include "Sprite.hpp"

#include <vector>
namespace GenImageTool
{
    class SpriteArray
    {
    public:
        SpriteArray
            (
            std::size_t tileWidth,
            std::size_t tileHeight
            );

        void addSprite
            (
            const Sprite& sprite
            );

        Sprite getSprite
            (
            std::size_t idx
            ) const;

        std::size_t getSpriteCount() const;

        std::size_t getTileHeight() const;

        std::size_t getTileWidth() const;

    private:
        std::size_t m_tileHeight;
        std::size_t m_tileWidth;
        std::vector<Sprite> m_sprites;
    };
}

#endif
