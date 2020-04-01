#include <fstream>
#include "genesis.hpp"
#include "MustacheContext.hpp"
#include "Writer.hpp"

namespace GenImageTool
{
	void Writer::write
        (
        const GenesisObjects& genesisObjects
        )
	{
        if (genesisObjects.outputHFile.empty())
        {
            throw std::runtime_error("out_h missing from input file.");
        }

        if (genesisObjects.outputCFile.empty())
        {
            throw std::runtime_error("out_c missing from input file.");
        }

        std::ofstream hFile{ genesisObjects.outputHFile, std::fstream::trunc };
        if (!hFile.is_open())
        {
            throw std::runtime_error("Failed to open " + genesisObjects.outputHFile.string() + " for writing.");
        }

        std::ofstream cFile{ genesisObjects.outputCFile, std::fstream::trunc };
        if (!cFile.is_open())
        {
            throw std::runtime_error("Failed to open " + genesisObjects.outputCFile.string() + " for writing.");
        }

        kainjow::mustache::data data;
        data[HEADER_FILENAME_TAG] = genesisObjects.outputHFile.filename().string();
        data[INCLUDE_GUARD_TAG] = genesisObjects.includeGuard;
        data[PALETTES_TAG] = getPaletteListData(genesisObjects.palettes);
        data[TILEMAPS_TAG] = getTileMapListData(genesisObjects.tileMaps);
        data[TILESETS_TAG] = getTileSetListData(genesisObjects.tileSets);

        MustacheContext<kainjow::mustache::mustache::string_type> ctx{ &data };
        kainjow::mustache::mustache hFileTemplate{ "{{> h_file}}" };
        hFile << hFileTemplate.render(ctx);

        kainjow::mustache::mustache cFileTemplate{ "{{> c_file}}" };
        cFile << cFileTemplate.render(ctx);
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

        kainjow::mustache::list rows;
        for (std::size_t j = 0; j < sprite.getTileHeight(); j++)
        {
            kainjow::mustache::data rowData;
            std::stringstream row;

            for (std::size_t i = 0; i < sprite.getTileWidth(); i++)
            {
                row << sprite.getTileIndex(i, j);

                if (i != sprite.getTileWidth() - 1 || j != sprite.getTileHeight() - 1)
                {
                    row << ",";
                }

                if (i != sprite.getTileWidth() - 1)
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
                row << tileMap.getTileIndex(i, j);

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
