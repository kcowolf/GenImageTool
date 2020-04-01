#ifndef TILESET_HPP
#define TILESET_HPP

#include <string>
#include <vector>

namespace GenImageTool
{
	class TileSet
	{
	public:
		void addTile
			(
			std::string tile
			);

		std::size_t getSize() const;

		std::string getTile
			(
			std::size_t index
			) const;

		uint16_t getTileIndex
			(
			std::string tile
			);

	private:
		std::vector<std::string> m_tiles;
	};
}

#endif
