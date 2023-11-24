#include "CollisionBlock.hpp"

namespace GenImageTool
{
	CollisionBlock::CollisionBlock
		(
		const std::vector<Color>& colors
		)
		: m_colors(colors)
	{
	}

	bool CollisionBlock::operator<
		(
		const CollisionBlock& rhs
		) const
	{
		if (m_colors.size() != rhs.m_colors.size())
		{
			return m_colors.size() < rhs.m_colors.size();
		}

		for (std::size_t i = 0; i < m_colors.size(); i++)
		{
			if (m_colors[i].getString() != rhs.m_colors[i].getString())
			{
				return m_colors[i].getString() < rhs.m_colors[i].getString();
			}
		}

		return false;
	}
}
