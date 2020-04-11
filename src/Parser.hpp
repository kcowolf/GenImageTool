#ifndef PARSER_HPP
#define PARSER_HPP

#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "GenesisObjects.hpp"
#include "Image.hpp"
#include "Palette.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "TileSet.hpp"

namespace GenImageTool
{
	class Parser
	{
	public:
		Parser();

		GenesisObjects parse
			(
			const std::filesystem::path& filename,
			const std::filesystem::path& graphicsDir
			);

	private:
		typedef void (Parser::* CommandHandler) (const std::vector<std::string>& tokens);

		struct CommandTableEntry
		{
			int tokenCount;
			std::string format;
			CommandHandler handler;
		};

		static const char COMMENT_CHAR;

		std::map<std::string, CommandTableEntry> m_commands;

		std::filesystem::path m_inputFile;
		std::filesystem::path m_graphicsDir;
		unsigned int m_lineCounter;

		GenesisObjects m_genesisObjects;

		std::string getErrorPrefix();

		Image& getImage
			(
			const std::string& name
			);

		Palette& getPalette
			(
			const std::string& name
			);

		bool getPaletteCollection
			(
			const std::string& name,
			std::map<std::size_t, Palette&>& paletteCollection
			);

		TileSet& getTileSet
			(
			const std::string& name
			);

		std::vector<std::string> getTokens
			(
			const std::string& line
			);

		void parseImage
			(
			const std::vector<std::string>& tokens
			);

		void parseOutputCFile
			(
			const std::vector<std::string>& tokens
			);

		void parseOutputHFile
			(
			const std::vector<std::string>& tokens
			);

		void parsePalette
			(
			const std::vector<std::string>& tokens
			);

		void parsePaletteColor
			(
			const std::vector<std::string>& tokens
			);

		void parsePaletteCollection
			(
			const std::vector<std::string>& tokens
			);

		void parseTileSet
			(
			const std::vector<std::string>& tokens
			);

		void parseTileMap
			(
			const std::vector<std::string>& tokens
			);

		void parseSprite
			(
			const std::vector<std::string>& tokens
			);

		void parseTileMapArray
			(
			const std::vector<std::string>& tokens
			);

		void parseSpriteArray
			(
			const std::vector<std::string>& tokens
			);

		Sprite readSprite
			(
			Image& image,
			std::map<std::size_t, Palette&>& palettes,
			TileSet& tileSet,
			uint16_t x,
			uint16_t y,
			uint16_t tileW,
			uint16_t tileH,
			bool addColors
			);

		TileMap readTileMap
			(
			Image& image,
			std::map<std::size_t, Palette&>& palettes,
			TileSet& tileSet,
			uint16_t x,
			uint16_t y,
			uint16_t tileW,
			uint16_t tileH,
			bool addColors
			);
	};
}

#endif
