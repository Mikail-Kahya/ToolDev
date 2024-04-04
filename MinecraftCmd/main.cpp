#include <iostream>
#include <string>
#include <vector>

#include <fstream>
#include <regex>

#include "MinecraftParser.h"

using Arguments = std::vector<std::string>;

bool FindFile(std::string& out, const Arguments& args);
bool FindInput(std::string& out, const std::string& argumentType, const Arguments& args);
bool IsANumber(const std::string& text);
void PrintCommandTemplate();

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		PrintCommandTemplate();
		return -1;
	}

	Arguments arguments{};
	for (int idx{}; idx < argc; ++idx)
		arguments.emplace_back(argv[idx]);

	std::string inputFileName{};
	std::string outputFileName{};
	std::string blockSizeInput{};
	float blockSize{};

	// Input checking
	if (!FindInput(inputFileName, "-i", arguments))
	{
		PrintCommandTemplate();
		return -1;
	}

	if (!FindInput(outputFileName, "-o", arguments))
	{
		PrintCommandTemplate();
		return -1;
	}

	if (FindInput(blockSizeInput, "-s", arguments))
	{
		if (!IsANumber(blockSizeInput))
		{
			std::cerr << "Block size is not a number!\n";
			PrintCommandTemplate();
			return -1;
		}

		blockSize = std::stof(blockSizeInput);
	}

	
	// File has right extension
	const std::regex objRegex{ ".json$" };
	if (!std::regex_search(inputFileName, objRegex))
	{
		PrintCommandTemplate();
		std::cerr << inputFileName << " does not have the .json file extension\n\n";
		return -1;
	}

	MinecraftParser parser{ blockSize };
	parser.ParseJson(inputFileName);
	parser.SaveObj(outputFileName);

	return 0;
}

bool FindFile(std::string& out, const Arguments& args)
{
	const auto foundIt = std::find(args.begin(), args.end(), out);
	out = *foundIt;
	return foundIt != args.end();
}

bool FindInput(std::string& out, const std::string& argumentType, const Arguments& args)
{
	if (std::count(args.begin(), args.end(), argumentType) != 1)
		return false;

	const auto foundIt = std::find(args.begin(), args.end(), argumentType);
	if (foundIt == args.end() || foundIt == args.end() - 1)
		return false;

	out = *std::next(foundIt);
	return true;
}

bool IsANumber(const std::string& text)
{
	const std::regex floatingPointRegex{ "^-?\\d*\\.?\\d+$" };
	std::smatch matches{};
	return !std::regex_match(text, matches, floatingPointRegex);
}

void PrintCommandTemplate()
{
	std::cerr << "\nInvalid arguments given\n";
	std::cout
		<< "Command:\n"
		<< "-----    MinecraftCmd -i inputName.json -o outputName    -----\n"
		<< "-----    MinecraftCmd -o outputName.json -i inputName    -----\n"
		<< "\nOptional block size:\n"
		<< "-----    ... -s 1    -----\n";
}
