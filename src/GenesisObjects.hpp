#ifndef GENESISOBJECTS_HPP
#define GENESISOBJECTS_HPP

#include <filesystem>
#include <map>
#include <string>
#include "Image.hpp"
#include "Palette.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "TileSet.hpp"

struct GenesisObjects
{
	std::string includeGuard;
	std::filesystem::path outputCFile;
	std::filesystem::path outputHFile;

	std::map<std::string, GenImageTool::Image> images;
	std::map<std::string, GenImageTool::Palette> palettes;
	std::map<std::string, GenImageTool::Sprite> sprites;
	std::map<std::string, GenImageTool::TileMap> tileMaps;
	std::map<std::string, GenImageTool::TileSet> tileSets;
};

#endif
