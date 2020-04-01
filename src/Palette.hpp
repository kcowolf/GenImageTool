#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <vector>
#include "Color.hpp"

namespace GenImageTool
{
	class Palette
	{
	public:
		void addColor
			(
			Color color
			);

		Color getColor
			(
			std::size_t index
			) const;

		std::size_t getColorIndex
			(
			Color color
			);

		std::size_t getSize() const;

	private:
		std::vector<Color> m_colors;
	};
}

#endif
