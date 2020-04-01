#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

namespace GenImageTool
{
	class Color
	{
	public:
		Color
			(
			uint8_t r,
			uint8_t g,
			uint8_t b,
			bool fix
			);

		bool operator==
			(
			const Color& rhs
			) const;

		std::string getString() const;

	private:
		std::string m_genesisColor;
	};
}

#endif
