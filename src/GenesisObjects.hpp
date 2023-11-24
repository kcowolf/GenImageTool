#ifndef GENESISOBJECTS_HPP
#define GENESISOBJECTS_HPP

#include <filesystem>
#include <map>
#include <string>
#include "BlockMap.hpp"
#include "CollisionBlockMap.hpp"
#include "CollisionBlockSet.hpp"
#include "Image.hpp"
#include "Palette.hpp"
#include "Sprite.hpp"
#include "SpriteArray.hpp"
#include "TileMap.hpp"
#include "TileMapArray.hpp"
#include "TileSet.hpp"

enum SDKLibrary
{
    SGDK,
    MDK
};

struct GenesisObjects
{
    GenesisObjects();

    std::string includeGuard;
    std::filesystem::path outputCFile;
    std::filesystem::path outputHFile;
    SDKLibrary library;

    std::map<std::string, GenImageTool::BlockMap> blockMaps;
    std::map<std::string, GenImageTool::CollisionBlockMap> collisionBlockMaps;
    std::map<std::string, GenImageTool::CollisionBlockSet> collisionBlockSets;
    std::map<std::string, GenImageTool::Image> images;
    std::map<std::string, GenImageTool::Palette> palettes;
    std::map<std::string, std::map<std::size_t, GenImageTool::Palette&>> paletteCollections;
    std::map<std::string, GenImageTool::Sprite> sprites;
    std::map<std::string, GenImageTool::SpriteArray> spriteArrays;
    std::map<std::string, GenImageTool::TileMap> tileMaps;
    std::map<std::string, GenImageTool::TileMapArray> tileMapArrays;
    std::map<std::string, GenImageTool::TileSet> tileSets;
};

#endif
