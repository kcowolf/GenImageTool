#ifndef COLLISIONBLOCKSET_HPP
#define COLLISIONBLOCKSET_HPP

#include <map>
#include "CollisionBlock.hpp"

namespace GenImageTool
{
	class CollisionBlockSet
	{
	public:
		CollisionBlockSet
			(
			const uint16_t width,
			const uint16_t height
			);

		void addBlock
			(
			const CollisionBlock& block,
			const uint16_t value
			);

		bool find
            (
            const CollisionBlock& block,
            uint16_t& value
            ) const;

		uint16_t getHeight() const;

		uint16_t getWidth() const;

	private:
		std::map<CollisionBlock, uint16_t> m_collisionBlocks;
		uint16_t m_height;
		uint16_t m_width;
	};
}

#endif