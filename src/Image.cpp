#include <sstream>
#include "Genesis.hpp"
#include "Image.hpp"

namespace GenImageTool
{
	Image::Image
		(
		const std::filesystem::path& filename,
		bool fixColors
		)
		: m_fixColors(fixColors)
	{
		if (filename.empty() || !std::filesystem::exists(filename) || !std::filesystem::is_regular_file(filename))
		{
			throw std::runtime_error("Image filename is not valid: " + filename.string());
		}

		m_surface = IMG_Load(filename.string().c_str());
		if (m_surface == NULL)
		{
			throw std::runtime_error("Failed to load image: " + filename.string());
		}
	}

	Image::~Image()
	{
		SDL_FreeSurface(m_surface);
	}

	Image::Image
		(
		const Image& image
		)
	{
		m_surface = SDL_CreateRGBSurface(image.m_surface->flags, image.m_surface->w, image.m_surface->h, image.m_surface->format->BitsPerPixel, image.m_surface->format->Rmask, image.m_surface->format->Gmask, image.m_surface->format->Bmask, image.m_surface->format->Amask);
		SDL_SetSurfacePalette(m_surface, image.m_surface->format->palette);
		if (m_surface == NULL)
		{
			throw std::runtime_error("Image::Image() -- Unable to create SDL_Surface for copy.");
		}
		else
		{
			SDL_BlitSurface(image.m_surface, NULL, m_surface, NULL);
		}

		m_fixColors = image.m_fixColors;
	}

	Image& Image::operator=
		(
		const Image& rhs
		)
	{
		if (this != &rhs)
		{
			if (m_surface != NULL)
			{
				SDL_FreeSurface(m_surface);
				m_surface = NULL;
			}

			m_surface = SDL_CreateRGBSurface(rhs.m_surface->flags, rhs.m_surface->w, rhs.m_surface->h, rhs.m_surface->format->BitsPerPixel, rhs.m_surface->format->Rmask, rhs.m_surface->format->Gmask, rhs.m_surface->format->Bmask, rhs.m_surface->format->Amask);
			SDL_SetSurfacePalette(m_surface, rhs.m_surface->format->palette);
			if (m_surface == NULL)
			{
				throw std::runtime_error("Image::operator=() -- Unable to create SDL_Surface for copy.");
			}
			else
			{
				SDL_BlitSurface(rhs.m_surface, NULL, m_surface, NULL);
			}

			m_fixColors = rhs.m_fixColors;
		}

		return *this;
	}

	bool Image::readTile
		(
		unsigned int x,
		unsigned int y,
		Palette& palette,
		bool addColors,
		std::string& tile
		) const
	{
		if (x + TILE_PIXEL_WIDTH - 1 >= (unsigned int)m_surface->w || y + TILE_PIXEL_HEIGHT - 1 >= (unsigned int)m_surface->h)
		{
			throw std::runtime_error("Invalid coordinates specified");
		}

		std::stringstream stringstream;

		for (unsigned int j = y; j < y + TILE_PIXEL_HEIGHT; j++)
		{
			for (unsigned int i = x; i < x + TILE_PIXEL_WIDTH; i++)
			{
				Color color = getPixel(i, j);
				std::size_t colorIndex;
				if (!palette.find(color, colorIndex))
				{
					if (addColors)
					{
						colorIndex = palette.addColor(color);
					}
					else
					{
						return false;
					}
				}

				stringstream << std::hex << colorIndex;
			}
		}

		tile = stringstream.str();
		return true;
	}

	Color Image::getPixel
		(
		unsigned int x,
		unsigned int y
		) const
	{
		SDL_Color sdlColor;

		if (x >= (unsigned int)m_surface->w || y >= (unsigned int)m_surface->h)
		{
			throw std::runtime_error("Invalid coordinates specified");
		}

		SDL_PixelFormat* fmt = m_surface->format;
		int bpp = fmt->BytesPerPixel;

		Uint32 pixel;

		Uint8* p = (Uint8*)m_surface->pixels + y * m_surface->pitch + x * bpp;

		switch (bpp)
		{
		case 1:
			pixel = *p;
			break;
		case 2:
			pixel = *((Uint16*)p);
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				pixel = p[0] << 16 | p[1] << 8 | p[2];
			}
			else
			{
				pixel = p[0] | p[1] << 8 | p[2] << 16;
			}
			break;
		case 4:
			pixel = *(Uint32*)p;
			break;
		default:
			throw std::runtime_error("Invalid pixel format.");
		}

		if (bpp == 1)
		{
			sdlColor = fmt->palette->colors[pixel];
		}
		else
		{
			Uint32 temp;
			Uint8 red;
			Uint8 green;
			Uint8 blue;

			temp = pixel & fmt->Rmask;
			temp = temp >> fmt->Rshift;
			temp = temp << fmt->Rloss;
			red = (Uint8)temp;

			temp = pixel & fmt->Gmask;
			temp = temp >> fmt->Gshift;
			temp = temp << fmt->Gloss;
			green = (Uint8)temp;

			temp = pixel & fmt->Bmask;
			temp = temp >> fmt->Bshift;
			temp = temp << fmt->Bloss;
			blue = (Uint8)temp;

			sdlColor.r = red;
			sdlColor.g = green;
			sdlColor.b = blue;
		}

		return Color{ sdlColor.r, sdlColor.g, sdlColor.b, m_fixColors };
	}
}
