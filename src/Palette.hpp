#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <vector>
#include "Color.hpp"

namespace GenImageTool
{
    class Palette
    {
    public:
        std::size_t addColor
            (
            const Color& color
            );

        bool find
            (
            const Color& color,
            std::size_t& index
            ) const;

        Color getColor
            (
            std::size_t index
            ) const;

        std::size_t getSize() const;

    private:
        std::vector<Color> m_colors;
    };
}

#endif
