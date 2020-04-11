#include <stdexcept>
#include "Genesis.hpp"
#include "Palette.hpp"

namespace GenImageTool
{
	std::size_t Palette::addColor
		(
		const Color& color
		)
	{
		m_colors.push_back(color);

		if (m_colors.size() > PALETTE_MAX_COLORS)
		{
			throw std::runtime_error("Palette has too many colors.");
		}

		return m_colors.size() - 1;
	}

	bool Palette::find
		(
		const Color& color,
		std::size_t& index
		) const
	{
		for (std::size_t i = 0; i < m_colors.size(); i++)
		{
			if (m_colors[i] == color)
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	Color Palette::getColor
		(
		std::size_t index
		) const
	{
		if (index >= m_colors.size())
		{
			throw std::runtime_error("Invalid color index.");
		}

		return m_colors[index];
	}

	std::size_t Palette::getSize() const
	{
		return m_colors.size();
	}
}
