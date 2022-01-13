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

**block** - A small tilemap used as part of a blockmap.  For example, an image containing repeated 16x16-pixel patterns could be processed into a blockmap with blocks 2 tiles wide x 2 tiles high.

**blockmap** - Map made up of block indexes.  A tilemap_array is used to store blocks.  Indexes may be 16 bits (blockmap) or 8 bits (blockmap_8) each.

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

palette PAL_FOREST_BG  # Forest map background palette.
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
image IMG_FOREST_MAP "forest.png" FIX_COLORS
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
palette PAL_FOREST_BG
palette_color PAL_FOREST_BG 224 0 224
palette_colors PAL_FOREST_BG IMG_FOREST_MAP 0 0 320 224
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
palette_collection PAL_FOREST_COLLECTION ? ? PAL_FOREST_BG PAL_FOREST_FG
```

This might cause the following tile indexes to be written:
```
    16385, # palette 2, tile 1 (0b100000000000001)
    24576  # palette 3, tile 0 (0b110000000000000)
```
