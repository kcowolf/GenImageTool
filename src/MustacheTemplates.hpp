#ifndef MUSTACHE_TEMPLATES_HPP
#define MUSTACHE_TEMPLATES_HPP

#define COLORS_TAG "COLORS"
#define COLOR_TAG "COLOR"
#define HEADER_FILENAME_TAG "HEADER_FILENAME"
#define INCLUDE_GUARD_TAG "INCLUDE_GUARD"
#define ISLAST_TAG "ISLAST"
#define NAME_TAG "NAME"
#define PALETTES_TAG "PALETTES"
#define PIXEL_HEIGHT_TAG "PIXEL_HEIGHT"
#define PIXEL_WIDTH_TAG "PIXEL_WIDTH"
#define ROW_TAG "ROW"
#define ROWS_TAG "ROWS"
#define TILE_COUNT_TAG "TILE_COUNT"
#define TILE_HEIGHT_TAG "TILE_HEIGHT"
#define TILE_WIDTH_TAG "TILE_WIDTH"
#define TILEMAPS_TAG "TILEMAPS"
#define TILES_TAG "TILES"
#define TILESETS_TAG "TILESETS"

#define C_FILE_TEMPLATE R"(/* Autogenerated by GenImageTool */

#include "{{HEADER_FILENAME}}"
{{#PALETTES}}{{> c_palette}}{{/PALETTES}}
{{#TILESETS}}{{> c_tileset}}{{/TILESETS}}
{{#TILEMAPS}}{{> c_tilemap}}{{/TILEMAPS}}
)"



#define C_PALETTE_TEMPLATE R"(

const u16 {{NAME}}[16] =
{
{{#COLORS}}
{{> indent}}0x{{COLOR}}{{^ISLAST}},{{/ISLAST}}
{{/COLORS}}
};
)"



#define C_SPRITE_TEMPLATE R"(

const u16 {{NAME}}[{{NAME}}_TILE_COUNT] =
{
{{#ROWS}}
{{> indent}}{{ROW}}
{{/ROWS}}
};
)"



#define C_TILEMAP_TEMPLATE R"(

const u16 {{NAME}}[{{NAME}}_TILE_COUNT] =
{
{{#ROWS}}
{{> indent}}{{ROW}}
{{/ROWS}}
};
)"



#define C_TILESET_TEMPLATE R"(

const u32 {{NAME}}[{{NAME}}_TILE_COUNT][8] =
{
{{#TILES}}
{{> indent}}{
{{#ROWS}}
{{> indent}}{{> indent}}0x{{ROW}}{{^ISLAST}},{{/ISLAST}}
{{/ROWS}}
{{> indent}}}{{^ISLAST}},{{/ISLAST}}
{{/TILES}}
};
)"



#define H_FILE_TEMPLATE R"(/* Autogenerated by GenImageTool */

#ifndef {{INCLUDE_GUARD}}
#define {{INCLUDE_GUARD}}

#include <genesis.h>
{{#PALETTES}}{{> h_palette}}{{/PALETTES}}
{{#TILESETS}}{{> h_tileset}}{{/TILESETS}}
{{#TILEMAPS}}{{> h_tilemap}}{{/TILEMAPS}}

#endif
)"



#define H_PALETTE_TEMPLATE R"(

extern const u16 {{NAME}}[16];
)"



#define H_SPRITE_TEMPLATE R"(

#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
extern const u16 {{NAME}}[{{NAME}}_TILE_COUNT];
)"



#define H_TILEMAP_TEMPLATE R"(

#define {{NAME}}_TILE_WIDTH {{TILE_WIDTH}}
#define {{NAME}}_TILE_HEIGHT {{TILE_HEIGHT}}
#define {{NAME}}_PIXEL_WIDTH {{PIXEL_WIDTH}}
#define {{NAME}}_PIXEL_HEIGHT {{PIXEL_HEIGHT}}
#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
extern const u16 {{NAME}}[{{NAME}}_TILE_COUNT];
)"



#define H_TILESET_TEMPLATE R"(

#define {{NAME}}_TILE_COUNT {{TILE_COUNT}}
extern const u32 {{NAME}}[{{NAME}}_TILE_COUNT][8];
)"

#define INDENT_TEMPLATE "    "

#endif
