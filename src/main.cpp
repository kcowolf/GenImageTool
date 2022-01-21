#include <filesystem>
#include <SDL.h>
#include <stdio.h>
#include "GenesisObjects.hpp"
#include "Parser.hpp"
#include "tclap/ArgException.h"
#include "tclap/CmdLine.h"
#include "Writer.hpp"

constexpr const char* VERSION = "0.1.1";

int main(int argc, char* argv[])
{
    printf("GenImageTool - v%s\n\n", VERSION);

    try
    {
        TCLAP::CmdLine cmdLine{ "", ' ', VERSION };
        TCLAP::ValueArg<std::string> graphicsDirArg{ "g", "graphicsDir", "Directory where image files are located (defaults to same directory as input file)", false, "", "directory" };
        TCLAP::ValueArg<std::string> outputDirArg{ "o", "outputDir", "Directory where files will be written (defaults to same directory as input file)", false, "", "directory" };
        TCLAP::UnlabeledMultiArg<std::string> inputFileArgs{ "inputFile", "One or more text files to be parsed", true, "filename" };

        cmdLine.add(graphicsDirArg);
        cmdLine.add(outputDirArg);
        cmdLine.add(inputFileArgs);
        cmdLine.parse(argc, argv);

        for (std::string inputFileArg : inputFileArgs.getValue())
        {
            printf("Parsing %s\n", inputFileArg.c_str());

            std::filesystem::path inputFile = std::filesystem::absolute(inputFileArg);
            std::filesystem::path graphicsDir = graphicsDirArg.getValue().empty() ? inputFile.parent_path() : graphicsDirArg.getValue();
            std::filesystem::path outputDir = outputDirArg.getValue().empty() ? inputFile.parent_path() : outputDirArg.getValue();

            GenImageTool::Parser parser;
            GenesisObjects genesisObjects = parser.parse(inputFile, graphicsDir);

            GenImageTool::Writer writer;
            writer.write(genesisObjects, outputDir);
        }
    }
    catch (TCLAP::ArgException& e)
    {
        fprintf(stderr, "[ERROR] %s for arg %s\n", e.error().c_str(), e.argId().c_str());
        return EXIT_FAILURE;
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

    printf("Done!\n");
    return EXIT_SUCCESS;
}
