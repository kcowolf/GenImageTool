#ifndef GENESIS_HPP
#define GENESIS_HPP

constexpr std::size_t PALETTE_MAX_COLORS = 16;
constexpr std::size_t PALETTES_PER_SCENE = 4;
constexpr std::size_t TILESET_MAX_TILES = 2048;  // 11 bits can be used for tile index, or 0-2047

constexpr std::size_t TILE_PIXEL_WIDTH = 8;
constexpr std::size_t TILE_PIXEL_HEIGHT = 8;
constexpr std::size_t TILE_PIXEL_COUNT = 64;

constexpr std::size_t TILE_VFLIP = (1 << 12);
constexpr std::size_t TILE_HFLIP = (1 << 11);
constexpr std::size_t TILE_HVFLIP = (3 << 11);

constexpr std::size_t TILE_PAL_BITSHIFT = 13;
constexpr std::size_t TILE_PAL0 = 0;
constexpr std::size_t TILE_PAL1 = (1 << TILE_PAL_BITSHIFT);
constexpr std::size_t TILE_PAL2 = (2 << TILE_PAL_BITSHIFT);
constexpr std::size_t TILE_PAL3 = (3 << TILE_PAL_BITSHIFT);

constexpr std::size_t SPRITE_MIN_DIMENSION = 1;
constexpr std::size_t SPRITE_MAX_DIMENSION = 4;

#define PIXEL_TO_TILE(x) (x >> 3)

#endif
