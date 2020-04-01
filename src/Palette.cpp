#include <stdexcept>
#include "Genesis.hpp"
#include "Palette.hpp"

namespace GenImageTool
{
	void Palette::addColor
		(
		Color color
		)
	{
		m_colors.push_back(color);

		if (m_colors.size() > PALETTE_MAX_COLORS)
		{
			throw std::runtime_error("Palette has too many colors.");
		}
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

	std::size_t Palette::getColorIndex
		(
		Color color
		)
	{
		for (std::size_t i = 0; i < m_colors.size(); i++)
		{
			if (m_colors[i] == color)
			{
				return i;
			}
		}

		addColor(color);

		return m_colors.size() - 1;
	}

	std::size_t Palette::getSize() const
	{
		return m_colors.size();
	}
}
