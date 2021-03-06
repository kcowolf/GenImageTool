#ifndef MUSTACHE_TEMPLATES_HPP
#define MUSTACHE_TEMPLATES_HPP

constexpr const char* BLOCK_COUNT_TAG = "BLOCK_COUNT";
constexpr const char* BLOCK_HEIGHT_TAG = "BLOCK_HEIGHT";
constexpr const char* BLOCK_WIDTH_TAG = "BLOCK_WIDTH";
constexpr const char* BLOCK_TILE_HEIGHT_TAG = "BLOCK_TILE_HEIGHT";
constexpr const char* BLOCK_TILE_WIDTH_TAG = "BLOCK_TILE_WIDTH";
constexpr const char* BLOCKMAPS_TAG = "BLOCKMAPS";
constexpr const char* COLORS_TAG = "COLORS";
constexpr const char* COLOR_TAG = "COLOR";
constexpr const char* HEADER_FILENAME_TAG = "HEADER_FILENAME";
constexpr const char* INCLUDE_GUARD_TAG = "INCLUDE_GUARD";
constexpr const char* ISLAST_TAG = "ISLAST";
constexpr const char* NAME_TAG = "NAME";
constexpr const char* PALETTES_TAG = "PALETTES";
constexpr const char* PIXEL_HEIGHT_TAG = "PIXEL_HEIGHT";
constexpr const char* PIXEL_WIDTH_TAG = "PIXEL_WIDTH";
constexpr const char* ROW_TAG = "ROW";
constexpr const char* ROWS_TAG = "ROWS";
constexpr const char* SHORT_INDEXES_TAG = "SHORT_INDEXES";
constexpr const char* SPRITE_ARRAYS_TAG = "SPRITEARRAYS";
constexpr const char* SPRITE_COUNT_TAG = "SPRITE_COUNT";
constexpr const char* SPRITE_TILE_TAG = "SPRITE_TILE";
constexpr const char* SPRITES_TAG = "SPRITES";
constexpr const char* TILE_COUNT_TAG = "TILE_COUNT";
constexpr const char* TILE_HEIGHT_TAG = "TILE_HEIGHT";
constexpr const char* TILE_WIDTH_TAG = "TILE_WIDTH";
constexpr const char* TILEMAP_ARRAYS_TAG = "TILEMAPARRAYS";
constexpr const char* TILEMAP_COUNT_TAG = "TILEMAP_COUNT";
constexpr const char* TILEMAPS_TAG = "TILEMAPS";
constexpr const char* TILES_TAG = "TILES";
constexpr const char* TILESETS_TAG = "TILESETS";

constexpr const char* C_BLOCKMAP_TEMPLATE = R"(

const uint{{#SHORT_INDEXES}}8{{/SHORT_INDEXES}}{{^SHORT_INDEXES}}16{{/SHORT_INDEXES}}_t {{NAME}}[{{NAME}}_BLOCK_COUNT] =
{
{{#ROWS}}
{{> indent}}{{ROW}}
{{/ROWS}}
};
)";



constexpr const char* C_FILE_TEMPLATE = R"(/* Autogenerated by GenImageTool */

#include "{{HEADER_FILENAME}}"
{{#PALETTES}}{{> c_palette}}{{/PALETTES}}{{#TILESETS}}{{> c_tileset}}{{/TILESETS}}{{#TILEMAPS}}{{> c_tilemap}}{{/TILEMAPS}}{{#TILEMAPARRAYS}}{{> c_tilemaparray}}{{/TILEMAPARRAYS}}{{#BLOCKMAPS}}{{> c_blockmap}}{{/BLOCKMAPS}}{{#SPRITEARRAYS}}{{> c_spritearray}}{{/SPRITEARRAYS}}
)";



constexpr const char* C_PALETTE_TEMPLATE = R"(

const uint16_t {{NAME}}[16] =
{
{{#COLORS}}
{{> indent}}0x{{COLOR}}{{^ISLAST}},{{/ISLAST}}
{{/COLORS}}
};
)";



constexpr const char* C_SPRITEARRAY_TEMPLATE = R"(

const uint16_t {{NAME}}[{{NAME}}_SPRITE_COUNT] =
{
{{#SPRITES}}
{{> indent}}{{SPRITE_TILE}}{{^ISLAST}},{{/ISLAST}}
{{/SPRITES}}
};
)";



constexpr const char* C_TILEMAP_TEMPLATE = R"(

const uint16_t {{NAME}}[{{NAME}}_TILE_COUNT] =
{
{{#ROWS}}
{{> indent}}{{ROW}}
{{/ROWS}}
};
)";



constexpr const char* C_TILEMAPARRAY_TEMPLATE = R"(

const uint16_t {{NAME}}[{{NAME}}_TILEMAP_COUNT][{{NAME}}_TILE_COUNT] =
{
{{#TILEMAPS}}
{{> indent}}{
{{> indent}}{{#ROWS}}
{{> indent}}{{> indent}}{{ROW}}
{{> indent}}{{/ROWS}}
{{> indent}}}{{^ISLAST}},{{/ISLAST}}
{{/TILEMAPS}}
};
)";



constexpr const char* C_TILESET_TEMPLATE = R"(

const uint32_t {{NAME}}[{{NAME}}_TILE_COUNT][8] =
{
{{#TILES}}
{{> indent}}{
{{#ROWS}}
{{> indent}}{{> indent}}0x{{ROW}}{{^ISLAST}},{{/ISLAST}}
{{/ROWS}}
{{> indent}}}{{^ISLAST}},{{/ISLAST}}
{{/TILES}}
};
)";



constexpr const char* H_BLOCKMAP_TEMPLATE = R"(

#define {{NAME}}_BLOCK_WIDTH {{BLOCK_WIDTH}}
#define {{NAME}}_BLOCK_HEIGHT {{BLOCK_HEIGHT}}
#define {{NAME}}_BLOCK_COUNT {{BLOCK_COUNT}}
#define {{NAME}}_BLOCK_TILE_WIDTH {{BLOCK_TILE_WIDTH}}
#define {{NAME}}_BLOCK_TILE_HEIGHT {{BLOCK_TILE_HEIGHT}}
#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
extern const uint{{#SHORT_INDEXES}}8{{/SHORT_INDEXES}}{{^SHORT_INDEXES}}16{{/SHORT_INDEXES}}_t {{NAME}}[{{NAME}}_BLOCK_COUNT];
)";



constexpr const char* H_FILE_TEMPLATE = R"(/* Autogenerated by GenImageTool */

#ifndef {{INCLUDE_GUARD}}
#define {{INCLUDE_GUARD}}

#include <genesis.h>
{{#PALETTES}}{{> h_palette}}{{/PALETTES}}{{#TILESETS}}{{> h_tileset}}{{/TILESETS}}{{#TILEMAPS}}{{> h_tilemap}}{{/TILEMAPS}}{{#SPRITES}}{{> h_sprite}}{{/SPRITES}}{{#TILEMAPARRAYS}}{{> h_tilemaparray}}{{/TILEMAPARRAYS}}{{#BLOCKMAPS}}{{> h_blockmap}}{{/BLOCKMAPS}}{{#SPRITEARRAYS}}{{> h_spritearray}}{{/SPRITEARRAYS}}

#endif
)";



constexpr const char* H_PALETTE_TEMPLATE = R"(

extern const uint16_t {{NAME}}[16];
)";



constexpr const char* H_SPRITE_TEMPLATE = R"(

#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
#define {{NAME}}_TILE {{SPRITE_TILE}}
)";



constexpr const char* H_SPRITEARRAY_TEMPLATE = R"(

#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
#define {{NAME}}_SPRITE_COUNT {{SPRITE_COUNT}}
extern const uint16_t {{NAME}}[{{NAME}}_SPRITE_COUNT];
)";



constexpr const char* H_TILEMAP_TEMPLATE = R"(

#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
extern const uint16_t {{NAME}}[{{NAME}}_TILE_COUNT];
)";



constexpr const char* H_TILEMAPARRAY_TEMPLATE = R"(

#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
#define {{NAME}}_TILEMAP_COUNT {{TILEMAP_COUNT}}
extern const uint16_t {{NAME}}[{{NAME}}_TILEMAP_COUNT][{{NAME}}_TILE_COUNT];
)";



constexpr const char* H_TILESET_TEMPLATE = R"(

#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
extern const uint32_t {{NAME}}[{{NAME}}_TILE_COUNT][8];
)";



constexpr const char* INDENT_TEMPLATE = "    ";

#endif
