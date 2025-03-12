#include "Matrix.h"
#include <iostream>
#include "CLIParser.h"

int main(int argc, char *argv[])
{
		tools::CLI::Parser parser({
			.positionalArgs={
				{ "input", 1 },
				{ "output", 2 },
			}
		});
		parser.Parse(argc, argv);

		std::cout << "INPUT: " << (parser.Contains("input") ? parser["input"] : parser.Contains("pos-input") ? parser["pos-input"] : "error") << std::endl;
		std::cout << "OUTPUT: " << (parser.Contains("output") ? parser["output"] : parser.Contains("pos-output") ? parser["pos-output"] : "error") << std::endl;
}
