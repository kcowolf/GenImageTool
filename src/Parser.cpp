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

    // tileset TILESET_NAME

    // tilemap TILEMAP_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y TILE_W TILE_H

    // tilemap_array TILEMAPARRAY_NAME TILE_W TILE_H
    // tilemap_array_entry TILEMAPARRAY_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y

    // sprite SPRITE_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y TILE_W TILE_H

    // sprite_array SPRITEARRAY_NAME TILE_W TILE_H
    // sprite_array_entry SPRITEARRAY_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y

    Parser::Parser()
    {
        m_lineCounter = 0;

        m_commands["image"] = CommandTableEntry{ -1, "" };
        m_commands["out_h"] = CommandTableEntry{ 3, "out_h \"filename.h\" INCLUDE_GUARD" };
        m_commands["out_c"] = CommandTableEntry{ 2, "out_c \"filename.c\"" };
        m_commands["palette"] = CommandTableEntry{ 2, "palette PALETTE_NAME" };
        m_commands["palette_color"] = CommandTableEntry{ 5, "palette_color PALETTE_NAME RED GREEN BLUE" };
        m_commands["tilemap"] = CommandTableEntry{ 9, "tilemap TILEMAP_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y TILE_W TILE_H" };
        m_commands["tileset"] = CommandTableEntry{ 2, "tileset TILESET_NAME" };
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
                    if (tokens[0] == "image")
                    {
                        parseImage(tokens);
                    }
                    else if (tokens[0] == "out_c")
                    {
                        parseOutputCFile(tokens);
                    }
                    else if (tokens[0] == "out_h")
                    {
                        parseOutputHFile(tokens);
                    }
                    else if (tokens[0] == "palette")
                    {
                        parsePalette(tokens);
                    }
                    else if (tokens[0] == "palette_color")
                    {
                        parsePaletteColor(tokens);
                    }
                    else if (tokens[0] == "sprite")
                    {
                        parseSprite(tokens);
                    }
                    else if (tokens[0] == "tilemap")
                    {
                        parseTileMap(tokens);
                    }
                    else if (tokens[0] == "tileset")
                    {
                        parseTileSet(tokens);
                    }
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

    Image& Parser::getImage(const std::string& name)
    {
        std::map<std::string, Image>::iterator image = m_genesisObjects.images.find(name);

        if (image == m_genesisObjects.images.end())
        {
            throw std::runtime_error("Image " + name + " does not exist.");
        }

        return image->second;
    }

    Palette& Parser::getPalette(const std::string& name)
    {
        std::map<std::string, Palette>::iterator palette = m_genesisObjects.palettes.find(name);

        if (palette == m_genesisObjects.palettes.end())
        {
            throw std::runtime_error("Palette " + name + " does not exist.");
        }

        return palette->second;
    }

    TileSet& Parser::getTileSet(const std::string& name)
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
        Palette& palette = getPalette(tokens[3]);
        TileSet& tileSet = getTileSet(tokens[4]);

        uint16_t x = std::stoi(tokens[5]);
        uint16_t y = std::stoi(tokens[6]);
        uint16_t tileW = std::stoi(tokens[7]);
        uint16_t tileH = std::stoi(tokens[8]);

        Sprite sprite = readSprite(image, palette, tileSet, x, y, tileW, tileH);
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
        Palette& palette = getPalette(tokens[3]);
        TileSet& tileSet = getTileSet(tokens[4]);

        uint16_t x = std::stoi(tokens[5]);
        uint16_t y = std::stoi(tokens[6]);
        uint16_t tileW = std::stoi(tokens[7]);
        uint16_t tileH = std::stoi(tokens[8]);

        TileMap tileMap = readTileMap(image, palette, tileSet, x, y, tileW, tileH);
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
        Palette& palette,
        TileSet& tileSet,
        uint16_t x,
        uint16_t y,
        uint16_t tileW,
        uint16_t tileH
        )
    {
        Sprite sprite{ tileW, tileH };

        for (int i = 0; i < tileW; i++)
        {
            for (int j = 0; j < tileH; j++)
            {
                std::string tile = image.readTile(x + (i * TILE_PIXEL_WIDTH), y + (j * TILE_PIXEL_HEIGHT), palette);
                uint16_t tileIndex = tileSet.getTileIndex(tile);
                sprite.addTileIndex(tileIndex);
            }
        }

        return sprite;
    }

    TileMap Parser::readTileMap
        (
        Image& image,
        Palette& palette,
        TileSet& tileSet,
        uint16_t x,
        uint16_t y,
        uint16_t tileW,
        uint16_t tileH
        )
    {
        TileMap tileMap{ tileW, tileH };

        for (int j = 0; j < tileH; j++)
        {
            for (int i = 0; i < tileW; i++)
            {
                std::string tile = image.readTile(x + (i * TILE_PIXEL_WIDTH), y + (j * TILE_PIXEL_HEIGHT), palette);
                uint16_t tileIndex = tileSet.getTileIndex(tile);
                tileMap.addTileIndex(tileIndex);
            }
        }

        return tileMap;
    }

    /*void Parser::writePalette
        (
        std::ofstream& hFile,
        std::ofstream& cFile,
        const std::string& name,
        const Palette& palette
        )
    {
        hFile << std::endl;
        hFile << "extern const u16 " << name << "[16];" << std::endl;

        cFile << std::endl;
        cFile << "const u16 " << name << "[16] =" << std::endl;
        cFile << "{" << std::endl;

        for (int i = 0; i < PALETTE_MAX_COLORS; i++)
        {
            if (i < palette.getSize())
            {
                cFile << "    0x" << palette.getColor(i).getString();
            }
            else
            {
                cFile << "    0x0000";
            }

            if (i != PALETTE_MAX_COLORS - 1)
            {
                cFile << ",";
            }
            cFile << std::endl;
        }

        cFile << "};" << std::endl;
    }

    void Parser::writeTileMap
        (
        std::ofstream& hFile,
        std::ofstream& cFile,
        const std::string& name,
        const TileMap& tileMap
        )
    {
        hFile << std::endl;
        hFile << "#define " << name << "_TILE_WIDTH " << tileMap.getTileWidth() << std::endl;
        hFile << "#define " << name << "_TILE_HEIGHT " << tileMap.getTileHeight() << std::endl;
        hFile << "#define " << name << "_PIXEL_WIDTH " << tileMap.getTileWidth() * TILE_PIXEL_WIDTH << std::endl;
        hFile << "#define " << name << "_PIXEL_HEIGHT " << tileMap.getTileHeight() * TILE_PIXEL_HEIGHT << std::endl;
        hFile << "#define " << name << "_TILE_COUNT " << tileMap.getTileWidth() * tileMap.getTileHeight() << std::endl;
        hFile << "extern const u16 " << name << "[" << name << "_TILE_COUNT];" << std::endl;

        cFile << std::endl;
        cFile << "const u16 " << name << "[" << name << "_TILE_COUNT] =" << std::endl;
        cFile << "{" << std::endl;

        for (std::size_t j = 0; j < tileMap.getTileHeight(); j++)
        {
            cFile << "    ";

            for (std::size_t i = 0; i < tileMap.getTileWidth(); i++)
            {
                cFile << tileMap.getTileIndex(i, j);
                if ((i + 1) * (j + 1) != (tileMap.getTileWidth() * tileMap.getTileHeight()))
                {
                    cFile << ",";

                    if (i != tileMap.getTileWidth() - 1)
                    {
                        cFile << " ";
                    }
                }
            }

            cFile << std::endl;
        }

        cFile << "};" << std::endl;
    }

    void Parser::writeTileSet
        (
        std::ofstream& hFile,
        std::ofstream& cFile,
        const std::string& name,
        const TileSet& tileSet
        )
    {
        hFile << std::endl;
        hFile << "#define " << name << "_TILE_COUNT " << tileSet.getSize() << std::endl;
        hFile << "extern const u32 " << name << "[" << name << "_TILE_COUNT][" << TILE_PIXEL_HEIGHT << "];" << std::endl;

        cFile << std::endl;
        cFile << "const u32 " << name << "[" << name << "_TILE_COUNT][" << TILE_PIXEL_HEIGHT << "] =" << std::endl;
        cFile << "{" << std::endl;

        for (std::size_t j = 0; j < tileSet.getSize(); j++)
        {
            cFile << "    {" << std::endl;

            std::string tile = tileSet.getTile(j);

            for (std::size_t i = 0; i < TILE_PIXEL_HEIGHT; i++)
            {
                cFile << "        0x" << tile.substr(i * TILE_PIXEL_WIDTH, TILE_PIXEL_WIDTH);
                if (i != TILE_PIXEL_HEIGHT - 1)
                {
                    cFile << ",";
                }

                cFile << std::endl;
            }

            cFile << "    }";
            if (j != tileSet.getSize() - 1)
            {
                cFile << ",";
            }

            cFile << std::endl;
        }

        cFile << "};" << std::endl;
    }*/
}
