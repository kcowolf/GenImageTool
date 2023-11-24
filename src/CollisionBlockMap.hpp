#ifndef COLLISIONBLOCKMAP_HPP
#define COLLISIONBLOCKMAP_HPP

#include <vector>

namespace GenImageTool
{
	class CollisionBlockMap
	{
	public:
		CollisionBlockMap
			(
			std::size_t blockWidth,
			std::size_t blockHeight
			);

		void addCollisionBlockValue
			(
			uint16_t value
			);

		std::size_t getBlockHeight() const;

		std::size_t getBlockWidth() const;

		uint16_t getCollisionBlockValue
            (
            std::size_t x,
            std::size_t y
            ) const;

	private:
		std::size_t m_blockHeight;
		std::size_t m_blockWidth;
		std::vector<uint16_t> m_collisionBlockMapValues;
	};
}


#endif
