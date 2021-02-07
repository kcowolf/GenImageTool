#include <filesystem>
#include <SDL.h>
#include <stdio.h>
#include "GenesisObjects.hpp"
#include "Parser.hpp"
#include "tclap/ArgException.h"
#include "tclap/CmdLine.h"
#include "Writer.hpp"

constexpr const char* VERSION = "0.1b";

int main(int argc, char* argv[])
{
    printf("GenImageTool - v%s\n\n", VERSION);

    std::filesystem::path graphicsDir;
    std::filesystem::path outputDir;
    std::filesystem::path inputFile;

    try
    {
        TCLAP::CmdLine cmdLine{ "", ' ', VERSION };
        TCLAP::ValueArg<std::string> graphicsDirArg{ "g", "graphicsDir", "Directory where image files are located (defaults to same directory as input file)", false, "", "directory" };
        TCLAP::ValueArg<std::string> outputDirArg{ "o", "outputDir", "Directory where files will be written (defaults to same directory as input file)", false, "", "directory" };
        TCLAP::UnlabeledValueArg<std::string> inputFileArg{ "inputFile", "Text file to be parsed", true, "", "filename" };

        cmdLine.add(graphicsDirArg);
        cmdLine.add(outputDirArg);
        cmdLine.add(inputFileArg);
        cmdLine.parse(argc, argv);

        inputFile = std::filesystem::absolute(inputFileArg.getValue());

        graphicsDir = graphicsDirArg.getValue().empty() ? inputFile.parent_path() : graphicsDirArg.getValue();
        outputDir = outputDirArg.getValue().empty() ? inputFile.parent_path() : outputDirArg.getValue();
    }
    catch (TCLAP::ArgException& e)
    {
        fprintf(stderr, "[ERROR] %s for arg %s\n", e.error().c_str(), e.argId().c_str());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        fprintf(stderr, "[ERROR] An unknown exception occurred!\n");
        return EXIT_FAILURE;
    }

    try
    {
        GenImageTool::Parser parser;
        GenesisObjects genesisObjects = parser.parse(inputFile, graphicsDir);

        GenImageTool::Writer writer;
        writer.write(genesisObjects, outputDir);

        printf("Done!\n");
    }
    catch (const std::runtime_error& err)
    {
        fprintf(stderr, "[ERROR] %s\n", err.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        fprintf(stderr, "[ERROR] An unknown exception occurred!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
