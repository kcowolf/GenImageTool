#include <stdexcept>
#include "Genesis.hpp"
#include "TileSet.hpp"

namespace GenImageTool
{
    std::size_t TileSet::addTile
        (
        const std::string& tile
        )
    {
        if (m_tiles.size() == TILESET_MAX_TILES)
        {
            throw std::runtime_error("Tileset has too many tiles.");
        }

        std::size_t tileIndex = m_tiles.size();

        m_tiles.push_back(tile);

        std::string tileHFlip;
        std::string tileVFlip;
        std::string tileHVFlip;

        tileHFlip.resize(TILE_PIXEL_COUNT);
        tileVFlip.resize(TILE_PIXEL_COUNT);
        tileHVFlip.resize(TILE_PIXEL_COUNT);

        for (std::size_t j = 0; j < TILE_PIXEL_HEIGHT; j++)
        {
            for (std::size_t i = 0; i < TILE_PIXEL_WIDTH; i++)
            {
                tileHFlip[(j * TILE_PIXEL_WIDTH) + (TILE_PIXEL_WIDTH - 1 - i)] = tile[(j * TILE_PIXEL_WIDTH) + i];
                tileVFlip[((TILE_PIXEL_HEIGHT - j - 1) * TILE_PIXEL_WIDTH) + i] = tile[(j * TILE_PIXEL_WIDTH) + i];
                tileHVFlip[((TILE_PIXEL_HEIGHT - j - 1) * TILE_PIXEL_WIDTH) + (TILE_PIXEL_WIDTH - 1 - i)] = tile[(j * TILE_PIXEL_WIDTH) + i];
            }
        }

        // Check the map before inserting values so we don't overwrite a previous one.
        std::map<std::string, std::size_t>::iterator it = m_tileTransformations.find(tile);
        if (it == m_tileTransformations.end())
        {
            m_tileTransformations.insert(std::pair(tile, tileIndex));
        }

        it = m_tileTransformations.find(tileHFlip);
        if (it == m_tileTransformations.end())
        {
            m_tileTransformations.insert(std::pair(tileHFlip, tileIndex | TILE_HFLIP));
        }

        it = m_tileTransformations.find(tileVFlip);
        if (it == m_tileTransformations.end())
        {
            m_tileTransformations.insert(std::pair(tileVFlip, tileIndex | TILE_VFLIP));
        }

        it = m_tileTransformations.find(tileHVFlip);
        if (it == m_tileTransformations.end())
        {
            m_tileTransformations.insert(std::pair(tileHVFlip, tileIndex | TILE_HVFLIP));
        }

        return tileIndex;
    }

    void TileSet::clear()
    {
        m_tiles.clear();
    }

    std::size_t TileSet::getSize() const
    {
        return m_tiles.size();
    }

    bool TileSet::find
        (
        const std::string& tile,
        std::size_t& index
        ) const
    {
        std::map<std::string, std::size_t>::const_iterator it = m_tileTransformations.find(tile);

        if (it != m_tileTransformations.end())
        {
            index = it->second;
            return true;
        }
        
        return false;
    }

    bool TileSet::findTileSet
        (
        const TileSet& tileSet,
        std::size_t& startIdx
        ) const
    {
        bool found = false;

        if (getSize() >= tileSet.getSize())
        {
            for (std::size_t i = 0; !found && i <= getSize() - tileSet.getSize(); i++)
            {
                found = true;

                for (std::size_t j = 0; found && j < tileSet.getSize(); j++)
                {
                    if (getTile(i + j) != tileSet.getTile(j))
                    {
                        found = false;
                    }
                }

                if (found)
                {
                    startIdx = i;
                }
            }
        }

        return found;
    }

    std::string TileSet::getTile
        (
        std::size_t index
        ) const
    {
        if (index >= m_tiles.size())
        {
            throw std::runtime_error("Invalid tile index.");
        }

        return m_tiles[index];
    }
}
