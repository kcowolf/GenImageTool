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

		kainjow::mustache::data getSpriteArrayData
			(
			const std::string& name,
			const SpriteArray& spriteArray
			);

		kainjow::mustache::list getSpriteArrayListData
			(
			const std::map<std::string, SpriteArray>& spriteArrays
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

		kainjow::mustache::data getTileMapArrayData
			(
			const std::string& name,
			const TileMapArray& tileMapArray
			);

		kainjow::mustache::list getTileMapArrayListData
			(
			const std::map<std::string, TileMapArray>& tileMapArrays
			);

		kainjow::mustache::data getTileMapArrayMapData
			(
			const std::string& name,
			const TileMapArrayMap& tileMapArrayMap
			);

		kainjow::mustache::list getTileMapArrayMapListData
			(
			const std::map<std::string, TileMapArrayMap>& tileMapArrayMaps
			);

		kainjow::mustache::data getTileMapData
			(
			const std::string& name,
			const TileMap& tileMapArrayMap
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
