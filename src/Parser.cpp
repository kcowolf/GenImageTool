#include <fstream>
#include <iomanip>
#include <sstream>
#include "Genesis.hpp"
#include "Parser.hpp"

namespace GenImageTool
{
    const char Parser::COMMENT_CHAR = '#';

    // out_h "filename.h" INCLUDE_GUARD
    // out_c "filename.c"

    // image IMAGE_NAME "filename.png" {FIX_COLORS}

    // palette PALETTE_NAME
    // palette_color PALETTE_NAME RED GREEN BLUE
    // palette_collection PALETTE_COLLECTION_NAME PALETTE_NAME PALETTE_NAME PALETTE_NAME PALETTE_NAME (use ? to leave palette undefined, collection name must not match any palette name)

    // tileset TILESET_NAME

    // tilemap TILEMAP_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y TILE_W TILE_H

    // sprite SPRITE_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y TILE_W TILE_H

    // scene_tilemap TILEMAP_NAME IMAGE_NAME SCENE_NAME TILESET_NAME X Y TILE_W TILE_H

    // NOT YET IMPLEMENTED:
    // tilemap_array TILEMAPARRAY_NAME TILE_W TILE_H
    // tilemap_array_entry TILEMAPARRAY_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y
    // sprite_array SPRITEARRAY_NAME TILE_W TILE_H
    // sprite_array_entry SPRITEARRAY_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y

    Parser::Parser()
    {
        m_lineCounter = 0;

        m_commands["image"] = CommandTableEntry{ -1, "", &Parser::parseImage };
        m_commands["out_h"] = CommandTableEntry{ 3, "out_h \"filename.h\" INCLUDE_GUARD", &Parser::parseOutputHFile };
        m_commands["out_c"] = CommandTableEntry{ 2, "out_c \"filename.c\"", &Parser::parseOutputCFile };
        m_commands["palette"] = CommandTableEntry{ 2, "palette PALETTE_NAME", &Parser::parsePalette };
        m_commands["palette_color"] = CommandTableEntry{ 5, "palette_color PALETTE_NAME RED GREEN BLUE", &Parser::parsePaletteColor };
        m_commands["palette_collection"] = CommandTableEntry{ 6, "palette_collection PALETTE_COLLECTION_NAME PALETTE_NAME PALETTE_NAME PALETTE_NAME PALETTE_NAME (use ? to leave palette undefined, collection name must not match any palette name)", &Parser::parsePaletteCollection };
        m_commands["sprite"] = CommandTableEntry{ 9, "sprite TILEMAP_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y TILE_W TILE_H", &Parser::parseSprite };
        m_commands["tilemap"] = CommandTableEntry{ 9, "tilemap TILEMAP_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y TILE_W TILE_H", &Parser::parseTileMap };
        m_commands["tileset"] = CommandTableEntry{ 2, "tileset TILESET_NAME", &Parser::parseTileSet };
    }

    GenesisObjects Parser::parse
        (
        const std::filesystem::path& filename,
        const std::filesystem::path& graphicsDir
        )
    {
        m_genesisObjects = GenesisObjects();

        if (filename.empty() || !std::filesystem::exists(filename) || !std::filesystem::is_regular_file(filename))
        {
            throw std::runtime_error("Input filename " + filename.string() + " is not valid.");
        }

        if (!std::filesystem::exists(graphicsDir) || !std::filesystem::is_directory(graphicsDir))
        {
            throw std::runtime_error("Graphics directory " + graphicsDir.string() + " is not valid.");
        }

        m_inputFile = filename;
        m_graphicsDir = graphicsDir;
        m_lineCounter = 0;

        std::ifstream inputStream{ filename };
        std::string line;

        while (std::getline(inputStream, line))
        {
            m_lineCounter++;

            std::vector<std::string> tokens = getTokens(line);

            if (tokens.empty())
            {
                continue;
            }

            std::map<std::string, CommandTableEntry>::iterator command = m_commands.find(tokens[0]);
            if (command != m_commands.end())
            {
                if (command->second.tokenCount != -1 && command->second.tokenCount != tokens.size())
                {
                    throw std::runtime_error(getErrorPrefix() + "Invalid format for \"" + tokens[0] + "\", must be: " + command->second.format);
                }

                try
                {
                    (this->*(command->second.handler))(tokens);
                }
                catch (const std::runtime_error& err)
                {
                    throw std::runtime_error(getErrorPrefix() + err.what());
                }
            }
            else
            {
                throw std::runtime_error(getErrorPrefix() + "Unrecognized command: " + tokens[0]);
            }
        }

        return m_genesisObjects;
    }

    std::string Parser::getErrorPrefix()
    {
        std::string prefix = m_inputFile.string();
        prefix += ":";
        prefix += std::to_string(m_lineCounter);
        prefix += " ";

        return prefix;
    }

    Image& Parser::getImage
        (
        const std::string& name
        )
    {
        std::map<std::string, Image>::iterator image = m_genesisObjects.images.find(name);

        if (image == m_genesisObjects.images.end())
        {
            throw std::runtime_error("Image " + name + " does not exist.");
        }

        return image->second;
    }

    Palette& Parser::getPalette
        (
        const std::string& name
        )
    {
        std::map<std::string, Palette>::iterator palette = m_genesisObjects.palettes.find(name);

        if (palette == m_genesisObjects.palettes.end())
        {
            throw std::runtime_error("Palette " + name + " does not exist.");
        }

        return palette->second;
    }

    bool Parser::getPaletteCollection
        (
        const std::string& name,
        std::map<std::size_t, Palette&>& paletteCollection
        )
    {
        std::map<std::string, std::map<std::size_t, Palette&>>::iterator it = m_genesisObjects.paletteCollections.find(name);

        if (it == m_genesisObjects.paletteCollections.end())
        {
            return false;
        }

        paletteCollection = it->second;
        return true;
    }

    TileSet& Parser::getTileSet
        (
        const std::string& name
        )
    {
        std::map<std::string, TileSet>::iterator tileSet = m_genesisObjects.tileSets.find(name);

        if (tileSet == m_genesisObjects.tileSets.end())
        {
            throw std::runtime_error("TileSet " + name + " does not exist.");
        }

        return tileSet->second;
    }

    std::vector<std::string> Parser::getTokens
        (
        const std::string& line
        )
    {
        std::vector<std::string> result;
        std::string str;

        std::istringstream iss(line);
        while (iss >> std::quoted(str))
        {
            if (str[0] == COMMENT_CHAR)
            {
                break;
            }

            result.push_back(str);
        }

        return result;
    }

    void Parser::parseImage
        (
	    const std::vector<std::string>& tokens
        )
    {
        bool fixColors = (tokens.size() == 4 && tokens[3] == "FIX_COLORS");

        if (tokens.size() != 3 && !fixColors)
        {
            throw std::runtime_error("Invalid format for \"" + tokens[0] + "\", must be: IMAGE_NAME \"filename.png\" {FIX_COLORS}");
        }

        if (m_genesisObjects.images.find(tokens[1]) != m_genesisObjects.images.end())
        {
            throw std::runtime_error("Image " + tokens[1] + " already exists.");
        }

        m_genesisObjects.images.insert(std::pair(tokens[1], Image{ m_graphicsDir.append(tokens[2]), fixColors }));
    }

    void Parser::parseOutputCFile
        (
        const std::vector<std::string>& tokens
        )
    {
        m_genesisObjects.outputCFile = tokens[1];
    }

    void Parser::parseOutputHFile
        (
        const std::vector<std::string>& tokens
        )
    {
        m_genesisObjects.outputHFile = tokens[1];
        m_genesisObjects.includeGuard = tokens[2];
    }

    void Parser::parsePalette
        (
        const std::vector<std::string>& tokens
        )
    {
        if (m_genesisObjects.palettes.find(tokens[1]) != m_genesisObjects.palettes.end())
        {
            throw std::runtime_error("Palette " + tokens[1] + " already exists.");
        }

        if (m_genesisObjects.paletteCollections.find(tokens[1]) != m_genesisObjects.paletteCollections.end())
        {
            throw std::runtime_error("Palette name " + tokens[1] + " conflicts with a palette collection.");
        }

        m_genesisObjects.palettes[tokens[1]] = Palette();
    }

    void Parser::parsePaletteColor
        (
        const std::vector<std::string>& tokens
        )
    {
        Palette& palette = getPalette(tokens[1]);

        Color color{ static_cast<uint8_t>(std::stoi(tokens[2])), static_cast<uint8_t>(std::stoi(tokens[3])), static_cast<uint8_t>(std::stoi(tokens[4])), false };
        palette.addColor(color);
    }

    void Parser::parsePaletteCollection
        (
        const std::vector<std::string>& tokens
        )
    {
        if (m_genesisObjects.paletteCollections.find(tokens[1]) != m_genesisObjects.paletteCollections.end())
        {
            throw std::runtime_error("Palette collection " + tokens[1] + " already exists.");
        }

        if (m_genesisObjects.palettes.find(tokens[1]) != m_genesisObjects.palettes.end())
        {
            throw std::runtime_error("Palette collection name " + tokens[1] + " conflicts with a palette.");
        }

        std::map<std::size_t, Palette&> paletteCollection;

        for (std::size_t i = 0; i < PALETTES_PER_SCENE; i++)
        {
            if (tokens[i + 2] != "?")
            {
                Palette& palette = getPalette(tokens[i + 2]);
                paletteCollection.insert(std::pair<std::size_t, Palette&>(i << TILE_PAL_BITSHIFT, palette));
            }
        }

        m_genesisObjects.paletteCollections.insert(std::pair<std::string, std::map<std::size_t, Palette&>>(tokens[1], paletteCollection));
    }

    void Parser::parseSprite
        (
        const std::vector<std::string>& tokens
        )
    {
        if (m_genesisObjects.sprites.find(tokens[1]) != m_genesisObjects.sprites.end())
        {
            throw std::runtime_error("Sprite " + tokens[1] + " already exists.");
        }

        Image& image = getImage(tokens[2]);
        TileSet& tileSet = getTileSet(tokens[4]);

        uint16_t x = std::stoi(tokens[5]);
        uint16_t y = std::stoi(tokens[6]);
        uint16_t tileW = std::stoi(tokens[7]);
        uint16_t tileH = std::stoi(tokens[8]);

        std::map<std::size_t, Palette&> palettes;
        bool addColors = true;

        if (getPaletteCollection(tokens[3], palettes))
        {
            addColors = false;
        }
        else
        {
            palettes.insert(std::pair<std::size_t, Palette&>(0, getPalette(tokens[3])));
        }

        Sprite sprite = readSprite(image, palettes, tileSet, x, y, tileW, tileH, addColors);
        m_genesisObjects.sprites.insert(std::pair<std::string, Sprite>(tokens[1], sprite));
    }

    void Parser::parseSpriteArray
        (
        const std::vector<std::string>& tokens
        )
    {
    }

    void Parser::parseTileMap
        (
        const std::vector<std::string>& tokens
        )
    {
        if (m_genesisObjects.tileMaps.find(tokens[1]) != m_genesisObjects.tileMaps.end())
        {
            throw std::runtime_error("Tilemap " + tokens[1] + " already exists.");
        }

        Image& image = getImage(tokens[2]);
        TileSet& tileSet = getTileSet(tokens[4]);

        uint16_t x = std::stoi(tokens[5]);
        uint16_t y = std::stoi(tokens[6]);
        uint16_t tileW = std::stoi(tokens[7]);
        uint16_t tileH = std::stoi(tokens[8]);

        std::map<std::size_t, Palette&> palettes;
        bool addColors = true;

        if (getPaletteCollection(tokens[3], palettes))
        {
            addColors = false;
        }
        else
        {
            palettes.insert(std::pair<std::size_t, Palette&>(0, getPalette(tokens[3])));
        }

        TileMap tileMap = readTileMap(image, palettes, tileSet, x, y, tileW, tileH, addColors);
        m_genesisObjects.tileMaps.insert(std::pair<std::string, TileMap>(tokens[1], tileMap));
    }

    void Parser::parseTileMapArray
        (
        const std::vector<std::string>& tokens
        )
    {
    }

    void Parser::parseTileSet
        (
        const std::vector<std::string>& tokens
        )
    {
        if (m_genesisObjects.tileSets.find(tokens[1]) != m_genesisObjects.tileSets.end())
        {
            throw std::runtime_error("Tileset " + tokens[1] + " already exists.");
        }

        m_genesisObjects.tileSets[tokens[1]] = TileSet();
    }

    Sprite Parser::readSprite
        (
        Image& image,
        std::map<std::size_t, Palette&>& palettes,
        TileSet& tileSet,
        uint16_t x,
        uint16_t y,
        uint16_t tileW,
        uint16_t tileH,
        bool addColors
        )
    {
        Sprite sprite{ tileW, tileH };

        for (int i = 0; i < tileW; i++)
        {
            for (int j = 0; j < tileH; j++)
            {
                bool added = false;

                for (std::map<std::size_t, Palette&>::iterator it = palettes.begin(); !added && it != palettes.end(); ++it)
                {
                    std::string tile;
                    if (image.readTile(x + (i * TILE_PIXEL_WIDTH), y + (j * TILE_PIXEL_HEIGHT), it->second, addColors, tile))
                    {
                        std::size_t tileIndex;
                        if (!tileSet.find(tile, tileIndex))
                        {
                            tileIndex = tileSet.addTile(tile);
                        }

                        tileIndex = tileIndex | it->first;

                        sprite.addTileIndex(tileIndex);
                        added = true;
                    }
                }

                if (!added)
                {
                    throw std::runtime_error("Failed to add tile to sprite.  If using palette collection, make sure all colors in palettes are specified.");
                }
            }
        }

        return sprite;
    }

    TileMap Parser::readTileMap
        (
        Image& image,
        std::map<std::size_t, Palette&>& palettes,
        TileSet& tileSet,
        uint16_t x,
        uint16_t y,
        uint16_t tileW,
        uint16_t tileH,
        bool addColors
        )
    {
        TileMap tileMap{ tileW, tileH };

        for (int j = 0; j < tileH; j++)
        {
            for (int i = 0; i < tileW; i++)
            {
                bool added = false;

                for (std::map<std::size_t, Palette&>::iterator it = palettes.begin(); !added && it != palettes.end(); ++it)
                {
                    std::string tile;
                    if (image.readTile(x + (i * TILE_PIXEL_WIDTH), y + (j * TILE_PIXEL_HEIGHT), it->second, addColors, tile))
                    {
                        std::size_t tileIndex;
                        if (!tileSet.find(tile, tileIndex))
                        {
                            tileIndex = tileSet.addTile(tile);
                        }

                        tileIndex = tileIndex | it->first;

                        tileMap.addTileIndex(tileIndex);
                        added = true;
                    }
                }

                if (!added)
                {
                    throw std::runtime_error("Failed to add tile to tilemap.  If using palette collection, make sure all colors in palettes are specified.");
                }
            }
        }

        return tileMap;
    }
}
