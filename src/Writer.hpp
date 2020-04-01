#ifndef WRITER_HPP
#define WRITER_HPP

#include <mustache.hpp>
#include "GenesisObjects.hpp"

namespace GenImageTool
{
	class Writer
	{
	public:
		void write
			(
			const GenesisObjects& genesisObjects
			);

		kainjow::mustache::data getPaletteData
			(
			const std::string& name,
			const Palette& palette
			);

		kainjow::mustache::list getPaletteListData
			(
			const std::map<std::string, Palette>& palettes
			);

		kainjow::mustache::data getSpriteData
			(
			const std::string& name,
			const Sprite& sprite
			);

		kainjow::mustache::list getSpriteListData
			(
			const std::map<std::string, Sprite>& sprites
			);

		kainjow::mustache::data getTileMapData
			(
			const std::string& name,
			const TileMap& tileMap
			);

		kainjow::mustache::list getTileMapListData
			(
			const std::map<std::string, TileMap>& tileMaps
			);

		kainjow::mustache::data getTileSetData
			(
			const std::string& name,
			const TileSet& tileSet
			);

		kainjow::mustache::list getTileSetListData
			(
			const std::map<std::string, TileSet>& tileSets
			);
	};
}

#endif
