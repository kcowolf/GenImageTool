#ifndef COLLISIONBLOCK_HPP
#define COLLISIONBLOCK_HPP

#include <vector>
#include "Color.hpp"

namespace GenImageTool
{
	class CollisionBlock
	{
	public:
		CollisionBlock
			(
			const std::vector<Color>& colors
			);

		bool operator<
			(
			const CollisionBlock& rhs
			) const;

	private:
		std::vector<Color> m_colors;
	};
}

#endif
