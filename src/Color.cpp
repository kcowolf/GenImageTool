#include <sstream>
#include <stdexcept>
#include "Color.hpp"

namespace GenImageTool
{
	Color::Color
        (
        uint8_t r,
        uint8_t g,
        uint8_t b,
        bool fix
        )
	{
        if (fix)
        {
            if (r % 32 != 0)
            {
                if (r % 32 >= 16 && (r / 32) < 7)
                {
                    r = (r - (r % 32)) + 32;
                }
                else
                {
                    r = (r - (r % 32));
                }
            }

            if (g % 32 != 0)
            {
                if (g % 32 >= 16 && (g / 32) < 7)
                {
                    g = (g - (g % 32)) + 32;
                }
                else
                {
                    g = (g - (g % 32));
                }
            }

            if (b % 32 != 0)
            {
                if (b % 32 >= 16 && (b / 32) < 7)
                {
                    b = (b - (b % 32)) + 32;
                }
                else
                {
                    b = (b - (b % 32));
                }
            }
        }
        else
        {
            if (r % 32 != 0)
            {
                throw std::runtime_error("Red must be divisible by 32.");
            }

            if (g % 32 != 0)
            {
                throw std::runtime_error("Green must be divisible by 32.");
            }

            if (b % 32 != 0)
            {
                throw std::runtime_error("Blue must be divisible by 32.");
            }
        }

        std::stringstream stringstream;
        stringstream << "0";
        stringstream << std::hex << (b / 16);
        stringstream << std::hex << (g / 16);
        stringstream << std::hex << (r / 16);

        m_genesisColor = stringstream.str();
    }

    bool Color::operator==
        (
        const Color& rhs
        ) const
    {
        return m_genesisColor == rhs.m_genesisColor;
    }

    std::string Color::getString() const
    {
        return m_genesisColor;
    }
}
