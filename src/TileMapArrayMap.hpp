#ifndef TILEMAPARRAYMAP_HPP
#define TILEMAPARRAYMAP_HPP

#include <vector>
namespace GenImageTool
{
	class TileMapArrayMap
	{
	public:
		TileMapArrayMap
			(
			std::size_t mapWidth,
			std::size_t mapHeight
			);

		void addTileMapArrayIndex
			(
			std::size_t tileMapArrayIndex
			);

		std::size_t getMapHeight() const;

		std::size_t getTileMapArrayIndex
			(
			std::size_t x,
			std::size_t y
			) const;

		std::size_t getMapWidth() const;

	private:
		std::size_t m_mapHeight;
		std::size_t m_mapWidth;
		std::vector<std::size_t> m_tileMapArrayIndexes;
	};
}

#endif
