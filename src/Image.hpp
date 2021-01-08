#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <filesystem>
#include <SDL.h>
#include <SDL_image.h>
#include "Color.hpp"
#include "Palette.hpp"
#include "TileSet.hpp"

namespace GenImageTool
{
	class Image
	{
	public:
		Image
			(
			const std::filesystem::path& filename,
			bool fixColors
			);

		~Image();

		Image
			(
			const Image& image
			);

		Image& operator=
			(
			const Image& rhs
			);

		void readColors
			(
			unsigned int x,
			unsigned int y,
			unsigned int w,
			unsigned int h,
			Palette& palette
			) const;

		void readTile
			(
			unsigned int x,
			unsigned int y,
			Palette& palette,
			std::string& tile
			) const;

	private:
		Color getPixel
			(
			unsigned int x,
			unsigned int y
			) const;

		SDL_Surface* m_surface;
		bool m_fixColors;
	};
}

#endif
