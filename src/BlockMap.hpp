#ifndef BLOCKMAP_HPP
#define BLOCKMAP_HPP

#include <vector>
namespace GenImageTool
{
	class BlockMap
	{
	public:
		BlockMap
			(
			std::size_t mapWidth,
			std::size_t mapHeight,
			std::size_t tileWidth,
			std::size_t tileHeight,
			bool useShortIndexes
			);

		void addTileMapArrayIndex
			(
			std::size_t tileMapArrayIndex
			);

		std::size_t getBlockHeight() const;

		std::size_t getBlockTileHeight() const;

		std::size_t getBlockTileWidth() const;

		std::size_t getBlockWidth() const;

		std::size_t getTileMapArrayIndex
			(
			std::size_t x,
			std::size_t y
			) const;

		bool getUseShortIndexes() const;

	private:
		std::size_t m_blockHeight;
		std::size_t m_blockTileHeight;
		std::size_t m_blockTileWidth;
		std::size_t m_blockWidth;
		std::vector<std::size_t> m_tileMapArrayIndexes;
		bool m_useShortIndexes;
	};
}

#endif
