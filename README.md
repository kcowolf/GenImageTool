GenImageTool
============
GenImageTool is a utility for processing image files to generate graphics for the Sega Genesis/Megadrive.  It outputs graphical data as as .h and .c files.

Glossary
========
The following terminology is used throughout the program:

**image** - Source image file processed by the program.  May be in formats supported by [SDL_image](https://www.libsdl.org/projects/SDL_image), including PNG, BMP, GIF, and more.

**palette** - Color palette, may hold up to 16 colors.  Red/green/blue values evenly divisible by 32 are considered valid: 0, 32, 64, 96, 128, 160, 192, 224, and 240.  Other RGB values may be rounded to these values if an image is loaded with the FIX_COLORS flag.

**palette_collection** - A set of up to 4 palettes arranged as they will be loaded in-game.

**tile** - Basic 8x8-pixel graphical unit.

**tilemap** - Map made up of tile indexes.  A tileset is used to store tiles.

**tilemap_array** - Collection of tilemaps.

**tileset** - Collection of tiles.

**block** - A small tilemap used as part of a blockmap.

**blockmap** - Map made up of "blocks" (small tilemaps).  A tilemap_array is used to store blocks.  Indexes may be 16 bits (blockmap) or 8 bits (blockmap_8) each.

**sprite** - Genesis hardware sprite, typically used for animated graphics or graphics which must be positioned at a specific pixel coordinate.

**sprite_array** - Collection of sprites.

Command Line Arguments
======================
The following arguments may be specified when running the program:

**-g** or **--graphicsDir** - Directory where image files are located (defaults to same directory as input file)

**-o** or **--outputDir** - Directory where files will be written (defaults to same directory as input file)

All other arguments arguments are expected to be .txt files to be processed by the program.  One or more files may be specified.

Input Files
===========
Input files are simple .txt files containing instructions for what data will be extracted from each image and written to the output files.

Comments
--------
Everything following a pound-sign (**#**) is considered a comment.

```
# This is a full-line comment.

palette PAL_FOREST  # Forest map palette.
```

Output Filenames
----------------
All graphics data for this .txt file will be written to these files.

**out_h "filename.h" INCLUDE_GUARD**  
Specify the filename of the output header file.  The include guard will be used in an #ifndef/#define at the top of the file.

**out_c "filename.c"**  
Specify the filename of the output .c file.

```
out_h "gfx_forest.h" GFX_FOREST_H
out_c "gfx_forest.c"
```

Source Images
-------------
**image IMAGE_NAME "filename.png"**  
Specify an image to be processed by the program.

**image IMAGE_NAME "filename.png" FIX_COLORS**  
Same as above, but allows invalid color values to be rounded to the nearest valid values.

```
image IMG_FONT "font.png"
image IMG_FOREST "forest.png" FIX_COLORS
```

Color Palettes
--------------
Color palettes may contain a maximum of 16 colors.

**palette PALETTE_NAME**  
Create a color palette.

**palette_color PALETTE_NAME RED GREEN BLUE**  
Manually add a color to the color palette.  Valid values for RED, GREEN, and BLUE are 0, 32, 64, 96, 128, 160, 192, 224, and 240.

**palette_colors PALETTE_NAME IMAGE_NAME X Y WIDTH HEIGHT**  
Automatically read colors from the image; colors will be added to the palette if they are not already present.  X, Y, WIDTH, and HEIGHT are pixel values.

```
palette PAL_FOREST
palette_color PAL_FOREST 224 0 224
palette_colors PAL_FOREST IMG_FOREST 0 0 320 224
```

Palette Collections
-------------------
Most objects can specify either a Palette or a Palette Collection when reading data from images.  Palette collections define which palettes will be loaded as PAL0, PAL1, PAL2, or PAL3 when they are loaded into color RAM.
Tile indexes be written with flags for the corresponding palette index (0, 1, 2, or 3) set.

For example, if a tilemap is always used in the background, and you always use PAL2 for your background palette, reading the tilemap using a palette collection would allow tile indexes to be written with the palette bits set for PAL2.
If you read the tilemap with a palette instead of a collection, you would need to set the bits for PAL2 when loading the tilemap into VRAM.

Palette collections are also useful if a tilemap/sprite uses multiple palettes, since each tile will be matched to the first palette in the collection which contains all of its colors.

**palette_collection PALETTE_COLLECTION_NAME PALETTE_NAME PALETTE_NAME PALETTE_NAME PALETTE_NAME**  
Create a palette collection.  ? can be used as a placeholder.

```
palette PAL_FOREST_FG
palette PAL_FOREST_BG
palette_collection PAL_FOREST_COLLECTION ? ? PAL_FOREST_BG PAL_FOREST_FG
```

This might cause the following tile indexes to be written:
```
    16385, # palette 2, tile 1 (0b100000000000001)
    24576  # palette 3, tile 0 (0b110000000000000)
```

Tilesets
--------
Tilesets are an array of tiles.  They are used by other objects for storing tiles.  Multiple objects can share tiles within a tileset, even if they are different types (e.g. tilemaps and sprites can be made of the same tiles).

**tileset TILESET_NAME**  
Create a tileset.

```
tileset TILESET_FOREST
```

Tiles
-----
Tiles can be added manually to a tileset without being de-duplicated.  This is helpful in cases when de-duplicating tiles is not desirable, such as a font.

**tiles IMAGE_NAME PALETTE(_COLLECTION)_NAME TILSET_NAME X Y TILE_W TILE_H**
Add tiles to the tileset manually without deduplicating them.  X and Y are the location in the source image where the tiles begin.

```
# Read 128 tiles for the font.
tiles IMG_FONT PAL_FONT TILESET_FONT 0 0 32 4
```

Tilemaps
--------
A tilemap breaks down a large image into 8x8-pixel tiles.  Tiles are stored in a tileset.  If the same tile is used multiple times in the tilemap, it will only be added to the tileset the first time, reducing the amount of ROM needed for storing tile data.

**tilemap TILEMAP_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y TILE_W TILE_H**  
Create a tilemap.  X and Y are the location in the source image where the tilemap begins.  TILE_W and TILE_H specify the size of the tilemap in tiles (size in pixels / 8).

**tilemap_array TILEMAPARRAY_NAME TILE_W TILE_H**  
Create a tilemap array.  Tilemap arrays are used together with blockmaps (see below).  All tilemaps in the array must be the same width and height.

**tilemap_array_entry TILEMAPARRAY_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y**  
Add a tilemap to a tilemap array.  X and Y are the location in the source image where the tilemap begins.

```
# Create a tilemap for storing a background.
tilemap TILEMAP_FOREST IMG_FOREST PAL_FOREST TILESET_FOREST 0 0 32 32

# Create a tilemap array and read three maps from the same image into it.  Each map is 320x224 pixels (40x28 tiles).
tilemap_array TILEMAPARRAY_LEVELS 40 28
tilemap_array_entry TILEMAPARRAY_LEVELS IMG_LEVELS PAL_LEVEL TILESET_LEVELS 0 0
tilemap_array_entry TILEMAPARRAY_LEVELS IMG_LEVELS PAL_LEVEL TILESET_LEVELS 320 0
tilemap_array_entry TILEMAPARRAY_LEVELS IMG_LEVELS PAL_LEVEL TILESET_LEVELS 640 0
```

Blockmaps
---------
A blockmap is similar to a tilemap.  A tilemap breaks an image into tiles, while a blockmap breaks it into "blocks" (small tilemaps).  Blocks are stored in a tilemap array.  If the same block is used multiple times in the blockmap, it is only added to the array the first time.

Blockmaps are useful when a map is based around a grid larger than 8x8 pixels.  For example, if a map is based on a 16x16-pixel grid, a blockmap with blocks of size 2x2 tiles could be used.

If fewer than 256 unique blocks are needed, a blockmap_8 can be used, which uses 8-bit indexes instead of 16-bit.  This reduces the ROM space needed to store the map by half.

**blockmap BLOCKMAP_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME TILEMAPARRAY_NAME X Y BLOCK_W BLOCK_H**  
Create a blockmap.  Block indexes will be of type uint16_t (2 bytes).  X and Y are the location in the source image where the blockmap begins.  BLOCK_W and BLOCK_H are "number of blocks" wide and "number of blocks" high.  A 640x480-pixel map made up of 16x16-pixel blocks would be 40x30 blocks.

**blockmap_8 BLOCKMAP_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME TILEMAPARRAY_NAME X Y BLOCK_W BLOCK_H**  
Same as above, except block indexes will be of type uint8_t (1 byte).  This reduces the ROM size needed to store the map, but also limits the number of unique blocks to 256.

```
# Create a tilemap_array for storing blocks.
tilemap_array TILEMAPARRAY_FOREST 2 2

# Create blockmaps.
blockmap BLOCKMAP_FOREST_FG IMG_FOREST PAL_FOREST TILESET_FOREST TILEMAPARRAY_FOREST 0 0 40 30
blockmap_8 BLOCKMAP_FOREST_BG IMG_FOREST PAL_FOREST TILESET_FOREST TILEMAPARRAY_FOREST 640 0 20 15
```

Sprites
-------
Like tilemaps, sprites are made of tiles.  Unlike tilemaps, however, there is no "map" associated with a sprite.  Instead, when creating a sprite, you specify the sprite's tile width and height and the location of the first tile in VRAM; the Genesis then reads (tile width * tile height) tiles
beginning with the one you specified.  It expects the tiles to be ordered in VRAM from top to bottom and left to right.

For sprites, GenImageTool breaks an image into tiles and adds them to a tileset in the expected order.  It will attempt to deduplicate tiles, but can only do so if all tiles are already present in the tileset in the required order.

If tilemaps and sprites need to share tiles, it is recommended to specify sprites first in the input file.  If tilemaps are specified first, the tiles will be duplicated for sprites instead of being shared.

**sprite SPRITE_NAME IMAGE_NAME PALETTE(_COLLECTION)_NAME TILESET_NAME X Y TILE_W TILE_H**  
Create a sprite.  X and Y are the location in the source image where the sprite begins.  TILE_W and TILE_H specify the size of the sprite in tiles (size in pixels / 8).  Valid TILE_W and TILE_H values are 1-4.

**sprite_array SPRITEARRAY_NAME TILE_W TILE_H**  
Create a sprite array.  All sprites in the array must be the same width and height.

**sprite_array_entry SPRITEARRAY_NAME IMAGE_NAME PALETTE_NAME TILESET_NAME X Y**  
Add a sprite to a sprite array.  X and Y are the location in the source image where the sprite begins.

```
# Create a player sprite, 16x16 pixels.
sprite SPRITE_PLAYER_1 IMG_PLAYER PAL_PLAYER TILESET_PLAYER 0 0 2 2

# Create a sprite array for holding multiple player sprites.  Each is 32x32 pixels (4x4 tiles).
sprite_array SPRITEARRAY_PLAYER_RUN_RIGHT 4 4
sprite_array_entry SPRITEARRAY_PLAYER_RUN_RIGHT IMG_PLAYER PAL_PLAYER TILESET_PLAYER 0 0
sprite_array_entry SPRITEARRAY_PLAYER_RUN_RIGHT IMG_PLAYER PAL_PLAYER TILESET_PLAYER 32 0
sprite_array_entry SPRITEARRAY_PLAYER_RUN_RIGHT IMG_PLAYER PAL_PLAYER TILESET_PLAYER 64 0
```

Output Files
============
Output files are .h and .c files generated using [Mustache](https://mustache.github.io) templates.  These can then be compiled into your program.

The program defines constants for certain objects in the .h file.  For example, a tilemap will have constants defined for the pixel width, pixel height, tile width, tile height, and tile count (tile width * tile height).  These constants are automatically named based on the object name.

Loading Graphics Data Using SGDK
================================

Palettes
--------
```
PAL_setPalette(PAL0, PAL_FOREST, DMA_QUEUE);
```

Tilesets
--------
```
// Leave tile 0 empty, load tileset starting at index 1.
#define FOREST_TILESET_START_IDX 1

VDP_loadTileData((const uint32_t*)TILESET_FOREST, FOREST_TILESET_START_IDX, TILESET_FOREST_TILE_COUNT, DMA_QUEUE);
```

Tilemaps
--------
```
VDP_setTileMapDataRectEx(BG_A, TILEMAP_FOREST, TILE_ATTR_FULL(PAL0, 0, 0, 0, FOREST_TILESET_START_IDX), 0, 0, TILEMAP_FOREST_TILE_WIDTH, TILEMAP_FOREST_TILE_HEIGHT, TILEMAP_FOREST_TILE_WIDTH, DMA_QUEUE);
```

Tilemap arrays
--------------
```
#define LEVEL_TILESET_START_IDX 33

// Load level 2
VDP_setTileMapDataRectEx(BG_A, TILEMAPARRAY_LEVELS[2], TILE_ATTR_FULL(PAL0, 0, 0, 0, LEVEL_TILESET_START_IDX), 0, 0, TILEMAPARRAY_LEVELS_TILE_WIDTH, TILEMAPARRAY_LEVELS_TILE_HEIGHT, TILEMAPARRAY_LEVELS_TILE_WIDTH, DMA_QUEUE);
```

Blockmaps
---------
```
uint16_t blockX = 0;
uint16_t blockY = 0;
uint16_t tileX = 0;
uint16_t tileY = 0;

for (blockY = 0; blockY < BLOCKMAP_FOREST_BG_BLOCK_HEIGHT; blockY++)
{
    tileX = 0;

    for (blockX = 0; blockX < BLOCKMAP_FOREST_BG_BLOCK_WIDTH; blockX++)
    {
        // Using DMA instead of DMA_QUEUE here -- too many things would be queued up at one time.
        VDP_setTileMapDataRectEx(BG_A, TILEMAPARRAY_FOREST[BLOCKMAP_FOREST_BG[(blockY * BLOCKMAP_FOREST_BG_BLOCK_WIDTH) + blockX]], TILE_ATTR_FULL(PAL0, 0, 0, 0, FOREST_TILESET_START_IDX), tileX, tileY, BLOCKMAP_FOREST_BG_BLOCK_TILE_WIDTH, BLOCKMAP_FOREST_BG_BLOCK_TILE_HEIGHT, BLOCKMAP_FOREST_BG_BLOCK_TILE_WIDTH, DMA);
        tileX += BLOCKMAP_FOREST_BG_BLOCK_TILE_WIDTH;
    }

    tileY += BLOCKMAP_FOREST_BG_BLOCK_TILE_HEIGHT;
}
```

Sprites
-------


Sprite arrays
-------------

