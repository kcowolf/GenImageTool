#include <fstream>
#include "genesis.hpp"
#include "MustacheContext.hpp"
#include "Writer.hpp"

namespace GenImageTool
{
    void Writer::write
        (
        const GenesisObjects& genesisObjects,
        const std::filesystem::path& outputDir
        )
    {
        m_outputDir = outputDir;

        if (genesisObjects.outputHFile.empty())
        {
            throw std::runtime_error("out_h missing from input file.");
        }

        if (genesisObjects.outputCFile.empty())
        {
            throw std::runtime_error("out_c missing from input file.");
        }

        std::ofstream hFile{ m_outputDir / genesisObjects.outputHFile, std::fstream::trunc };
        if (!hFile.is_open())
        {
            throw std::runtime_error("Failed to open " + genesisObjects.outputHFile.string() + " for writing.");
        }

        std::ofstream cFile{ m_outputDir / genesisObjects.outputCFile, std::fstream::trunc };
        if (!cFile.is_open())
        {
            throw std::runtime_error("Failed to open " + genesisObjects.outputCFile.string() + " for writing.");
        }

        kainjow::mustache::data data;
        data[HEADER_FILENAME_TAG] = genesisObjects.outputHFile.filename().string();
        data[INCLUDE_GUARD_TAG] = genesisObjects.includeGuard;
        if (genesisObjects.library == SGDK)
        {
            data[LIBRARY_SGDK_TAG] = true;
        }
        else if (genesisObjects.library == MDK)
        {
            data[LIBRARY_MDK_TAG] = true;
        }
        data[PALETTES_TAG] = getPaletteListData(genesisObjects.palettes);
        data[SPRITES_TAG] = getSpriteListData(genesisObjects.sprites);
        data[SPRITE_ARRAYS_TAG] = getSpriteArrayListData(genesisObjects.spriteArrays);
        data[TILEMAPS_TAG] = getTileMapListData(genesisObjects.tileMaps);
        data[TILEMAP_ARRAYS_TAG] = getTileMapArrayListData(genesisObjects.tileMapArrays);
        data[BLOCKMAPS_TAG] = getBlockMapListData(genesisObjects.blockMaps);
        data[TILESETS_TAG] = getTileSetListData(genesisObjects.tileSets);
        data[COLLISIONBLOCKMAPS_TAG] = getCollisionBlockMapListData(genesisObjects.collisionBlockMaps);

        MustacheContext<kainjow::mustache::mustache::string_type> ctx{ &data };
        kainjow::mustache::mustache hFileTemplate{ "{{> h_file}}" };
        hFile << hFileTemplate.render(ctx);

        kainjow::mustache::mustache cFileTemplate{ "{{> c_file}}" };
        cFile << cFileTemplate.render(ctx);
    }

    kainjow::mustache::data Writer::getBlockMapData
        (
        const std::string& name,
        const BlockMap& blockMap
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[BLOCK_COUNT_TAG] = std::to_string(blockMap.getBlockHeight() * blockMap.getBlockWidth());
        data[BLOCK_WIDTH_TAG] = std::to_string(blockMap.getBlockWidth());
        data[BLOCK_HEIGHT_TAG] = std::to_string(blockMap.getBlockHeight());
        data[BLOCK_TILE_WIDTH_TAG] = std::to_string(blockMap.getBlockTileWidth());
        data[BLOCK_TILE_HEIGHT_TAG] = std::to_string(blockMap.getBlockTileHeight());
        data[TILE_WIDTH_TAG] = std::to_string(blockMap.getBlockWidth() * blockMap.getBlockTileWidth());
        data[TILE_HEIGHT_TAG] = std::to_string(blockMap.getBlockHeight() * blockMap.getBlockTileHeight());
        data[PIXEL_WIDTH_TAG] = std::to_string(blockMap.getBlockWidth() * blockMap.getBlockTileWidth() * TILE_PIXEL_WIDTH);
        data[PIXEL_HEIGHT_TAG] = std::to_string(blockMap.getBlockHeight() * blockMap.getBlockTileHeight() * TILE_PIXEL_HEIGHT);
        data[SHORT_INDEXES_TAG] = blockMap.getUseShortIndexes();

        kainjow::mustache::list rows;
        for (std::size_t j = 0; j < blockMap.getBlockHeight(); j++)
        {
            kainjow::mustache::data rowData;
            std::stringstream row;

            for (std::size_t i = 0; i < blockMap.getBlockWidth(); i++)
            {
                row << "0x" << std::uppercase << std::setfill('0') << std::setw(blockMap.getUseShortIndexes() ? 2 : 4) << std::hex << blockMap.getTileMapArrayIndex(i, j);

                if (i != blockMap.getBlockWidth() - 1 || j != blockMap.getBlockHeight() - 1)
                {
                    row << ",";
                }

                if (i != blockMap.getBlockWidth() - 1)
                {
                    row << " ";
                }
            }

            rowData[ROW_TAG] = row.str();
            rows.push_back(rowData);
        }

        data[ROWS_TAG] = rows;

        return data;
    }

    kainjow::mustache::list Writer::getBlockMapListData
        (
        const std::map<std::string, BlockMap>& blockMaps
        )
    {
        kainjow::mustache::list blockMapsData;

        for (std::map<std::string, BlockMap>::const_iterator it = blockMaps.begin(); it != blockMaps.end(); ++it)
        {
            blockMapsData.push_back(getBlockMapData(it->first, it->second));
        }

        return blockMapsData;
    }

    kainjow::mustache::data Writer::getCollisionBlockMapData
        (
        const std::string& name,
        const CollisionBlockMap& collisionBlockMap
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[BLOCK_COUNT_TAG] = std::to_string(collisionBlockMap.getBlockHeight() * collisionBlockMap.getBlockWidth());
        data[BLOCK_WIDTH_TAG] = std::to_string(collisionBlockMap.getBlockWidth());
        data[BLOCK_HEIGHT_TAG] = std::to_string(collisionBlockMap.getBlockHeight());

        kainjow::mustache::list rows;
        for (std::size_t j = 0; j < collisionBlockMap.getBlockHeight(); j++)
        {
            kainjow::mustache::data rowData;
            std::stringstream row;

            for (std::size_t i = 0; i < collisionBlockMap.getBlockWidth(); i++)
            {
                row << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << collisionBlockMap.getCollisionBlockValue(i, j);

                if (i != collisionBlockMap.getBlockWidth() - 1 || j != collisionBlockMap.getBlockHeight() - 1)
                {
                    row << ",";
                }

                if (i != collisionBlockMap.getBlockWidth() - 1)
                {
                    row << " ";
                }
            }

            rowData[ROW_TAG] = row.str();
            rows.push_back(rowData);
        }

        data[ROWS_TAG] = rows;

        return data;
    }

    kainjow::mustache::list Writer::getCollisionBlockMapListData
        (
        const std::map<std::string, CollisionBlockMap>& collisionBlockMaps
        )
    {
        kainjow::mustache::list collisionBlockMapsData;

        for (std::map<std::string, CollisionBlockMap>::const_iterator it = collisionBlockMaps.begin(); it != collisionBlockMaps.end(); ++it)
        {
            collisionBlockMapsData.push_back(getCollisionBlockMapData(it->first, it->second));
        }

        return collisionBlockMapsData;
    }

    kainjow::mustache::data Writer::getPaletteData
        (
        const std::string& name,
        const Palette& palette
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;

        kainjow::mustache::list colorsData;

        Color defaultColor{ 0, 0, 0, false };

        for (std::size_t i = 0; i < PALETTE_MAX_COLORS; i++)
        {
            kainjow::mustache::data colorData;

            if (i < palette.getSize())
            {
                colorData[COLOR_TAG] = palette.getColor(i).getString();
            }
            else
            {
                colorData[COLOR_TAG] = defaultColor.getString();
            }

            if (i == PALETTE_MAX_COLORS - 1)
            {
                colorData[ISLAST_TAG] = true;
            }

            colorsData.push_back(colorData);
        }

        data[COLORS_TAG] = colorsData;

        return data;
    }

    kainjow::mustache::list Writer::getPaletteListData
        (
        const std::map<std::string, Palette>& palettes
        )
    {
        kainjow::mustache::list palettesData;

        for (std::map<std::string, Palette>::const_iterator it = palettes.begin(); it != palettes.end(); ++it)
        {
            palettesData.push_back(getPaletteData(it->first, it->second));
        }

        return palettesData;
    }

    kainjow::mustache::data Writer::getSpriteData
        (
        const std::string& name,
        const Sprite& sprite
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[TILE_COUNT_TAG] = std::to_string(sprite.getTileHeight() * sprite.getTileWidth());
        data[TILE_WIDTH_TAG] = std::to_string(sprite.getTileWidth());
        data[TILE_HEIGHT_TAG] = std::to_string(sprite.getTileHeight());
        data[PIXEL_WIDTH_TAG] = std::to_string(sprite.getTileWidth() * TILE_PIXEL_WIDTH);
        data[PIXEL_HEIGHT_TAG] = std::to_string(sprite.getTileHeight() * TILE_PIXEL_HEIGHT);
        data[SPRITE_TILE_TAG] = std::to_string(sprite.getStartTileIdx() + (sprite.getTileSet().getStartIdx() != UINT16_MAX ? sprite.getTileSet().getStartIdx() : 0));

        return data;
    }

    kainjow::mustache::data Writer::getSpriteArrayData
        (
        const std::string& name,
        const SpriteArray& spriteArray
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[SPRITE_COUNT_TAG] = std::to_string(spriteArray.getSpriteCount());
        data[TILE_COUNT_TAG] = std::to_string(spriteArray.getTileHeight() * spriteArray.getTileWidth());
        data[TILE_WIDTH_TAG] = std::to_string(spriteArray.getTileWidth());
        data[TILE_HEIGHT_TAG] = std::to_string(spriteArray.getTileHeight());
        data[PIXEL_WIDTH_TAG] = std::to_string(spriteArray.getTileWidth() * TILE_PIXEL_WIDTH);
        data[PIXEL_HEIGHT_TAG] = std::to_string(spriteArray.getTileHeight() * TILE_PIXEL_HEIGHT);

        kainjow::mustache::list spritesData;

        for (std::size_t i = 0; i < spriteArray.getSpriteCount(); i++)
        {
            kainjow::mustache::data spriteData = getSpriteData(std::string{}, spriteArray.getSprite(i));
            spriteData[ISLAST_TAG] = (i == spriteArray.getSpriteCount() - 1);
            spritesData.push_back(spriteData);
        }

        data[SPRITES_TAG] = spritesData;

        return data;
    }

    kainjow::mustache::list Writer::getSpriteArrayListData
        (
        const std::map<std::string, SpriteArray>& spriteArrays
        )
    {
        kainjow::mustache::list spritesArraysData;

        for (std::map<std::string, SpriteArray>::const_iterator it = spriteArrays.begin(); it != spriteArrays.end(); ++it)
        {
            spritesArraysData.push_back(getSpriteArrayData(it->first, it->second));
        }

        return spritesArraysData;
    }

    kainjow::mustache::list Writer::getSpriteListData
        (
        const std::map<std::string, Sprite>& sprites
        )
    {
        kainjow::mustache::list spritesData;

        for (std::map<std::string, Sprite>::const_iterator it = sprites.begin(); it != sprites.end(); ++it)
        {
            spritesData.push_back(getSpriteData(it->first, it->second));
        }

        return spritesData;
    }

    kainjow::mustache::data Writer::getTileMapArrayData
        (
        const std::string& name,
        const TileMapArray& tileMapArray
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[TILEMAP_COUNT_TAG] = std::to_string(tileMapArray.getTileMapCount());
        data[TILE_COUNT_TAG] = std::to_string(tileMapArray.getTileHeight() * tileMapArray.getTileWidth());
        data[TILE_WIDTH_TAG] = std::to_string(tileMapArray.getTileWidth());
        data[TILE_HEIGHT_TAG] = std::to_string(tileMapArray.getTileHeight());
        data[PIXEL_WIDTH_TAG] = std::to_string(tileMapArray.getTileWidth() * TILE_PIXEL_WIDTH);
        data[PIXEL_HEIGHT_TAG] = std::to_string(tileMapArray.getTileHeight() * TILE_PIXEL_HEIGHT);

        kainjow::mustache::list tileMapsData;

        for (std::size_t i = 0; i < tileMapArray.getTileMapCount(); i++)
        {
            kainjow::mustache::data tileMapData = getTileMapData(std::string{}, tileMapArray.getTileMap(i));
            tileMapData[ISLAST_TAG] = (i == tileMapArray.getTileMapCount() - 1);

            tileMapsData.push_back(tileMapData);
        }

        data[TILEMAPS_TAG] = tileMapsData;

        return data;
    }

    kainjow::mustache::list Writer::getTileMapArrayListData
        (
        const std::map<std::string, TileMapArray>& tileMapArrays
        )
    {
        kainjow::mustache::list tileMapArraysData;

        for (std::map<std::string, TileMapArray>::const_iterator it = tileMapArrays.begin(); it != tileMapArrays.end(); ++it)
        {
            tileMapArraysData.push_back(getTileMapArrayData(it->first, it->second));
        }

        return tileMapArraysData;
    }

    kainjow::mustache::data Writer::getTileMapData
        (
        const std::string& name,
        const TileMap& tileMap
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[TILE_COUNT_TAG] = std::to_string(tileMap.getTileHeight() * tileMap.getTileWidth());
        data[TILE_WIDTH_TAG] = std::to_string(tileMap.getTileWidth());
        data[TILE_HEIGHT_TAG] = std::to_string(tileMap.getTileHeight());
        data[PIXEL_WIDTH_TAG] = std::to_string(tileMap.getTileWidth() * TILE_PIXEL_WIDTH);
        data[PIXEL_HEIGHT_TAG] = std::to_string(tileMap.getTileHeight() * TILE_PIXEL_HEIGHT);

        kainjow::mustache::list rows;
        for (std::size_t j = 0; j < tileMap.getTileHeight(); j++)
        {
            kainjow::mustache::data rowData;
            std::stringstream row;

            for (std::size_t i = 0; i < tileMap.getTileWidth(); i++)
            {
                row << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << (tileMap.getTileIndex(i, j) + (tileMap.getTileSet().getStartIdx() != UINT16_MAX ? tileMap.getTileSet().getStartIdx() : 0));

                if (i != tileMap.getTileWidth() - 1 || j != tileMap.getTileHeight() - 1)
                {
                    row << ",";
                }

                if (i != tileMap.getTileWidth() - 1)
                {
                    row << " ";
                }
            }

            rowData[ROW_TAG] = row.str();
            rows.push_back(rowData);
        }

        data[ROWS_TAG] = rows;

        return data;
    }

    kainjow::mustache::list Writer::getTileMapListData
        (
        const std::map<std::string, TileMap>& tileMaps
        )
    {
        kainjow::mustache::list tileMapsData;

        for (std::map<std::string, TileMap>::const_iterator it = tileMaps.begin(); it != tileMaps.end(); ++it)
        {
            tileMapsData.push_back(getTileMapData(it->first, it->second));
        }

        return tileMapsData;
    }

    kainjow::mustache::data Writer::getTileSetData
        (
        const std::string& name,
        const TileSet& tileSet
        )
    {
        kainjow::mustache::data data;
        data[NAME_TAG] = name;
        data[TILE_COUNT_TAG] = std::to_string(tileSet.getSize());
        if (tileSet.getStartIdx() != UINT16_MAX)
        {
            data[START_TILE_IDX_TAG] = std::to_string(tileSet.getStartIdx());
        }

        kainjow::mustache::list tilesData;

        for (std::size_t j = 0; j < tileSet.getSize(); j++)
        {
            std::string tileString = tileSet.getTile(j);

            kainjow::mustache::data tileData;
            kainjow::mustache::list rows;

            for (std::size_t i = 0; i < TILE_PIXEL_HEIGHT; i++)
            {
                kainjow::mustache::data rowData;

                rowData[ROW_TAG] = tileString.substr(i * TILE_PIXEL_WIDTH, TILE_PIXEL_WIDTH);
                rowData[ISLAST_TAG] = (i == TILE_PIXEL_HEIGHT - 1);

                rows.push_back(rowData);
            }

            tileData[ROWS_TAG] = rows;
            tileData[ISLAST_TAG] = (j == tileSet.getSize() - 1);
            tilesData.push_back(tileData);
        }

        data[TILES_TAG] = tilesData;

        return data;
    }

    kainjow::mustache::list Writer::getTileSetListData
        (
        const std::map<std::string, TileSet>& tileSets
        )
    {
        kainjow::mustache::list tileSetsData;

        for (std::map<std::string, TileSet>::const_iterator it = tileSets.begin(); it != tileSets.end(); ++it)
        {
            tileSetsData.push_back(getTileSetData(it->first, it->second));
        }

        return tileSetsData;
    }
}
