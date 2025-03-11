#include <fstream>
#include <iostream>

std::string docMessage = R"(
Replace Utility - Version 1.0

Usage:
  replace.exe <input file> <output file> <search string> <replace string>
  replace.exe

Description:
  This utility replaces all occurrences of <search string> with <replace string>
  in the provided data and writes the result to the specified output file or outputs it.

Operation Modes:
  1. File Mode:
     If arguments are provided in the format:
       replace.exe <input file> <output file> <search string> <replace string>
     the utility will perform the following actions:
       - Read data from <input file>.
       - Replace all occurrences of <search string> with <replace string>.
       - Write the result to <output file>.
     If the number of arguments is incorrect, the utility:
       - Will output "ERROR" to stdout.
       - Will terminate with a return code of 1.

  2. Stdin Mode:
     If no arguments are provided, the utility will read data from standard input (stdin)
     in the following format:
       <search string>
       <replace string>
       <input text>
     The utility will perform the following actions:
       - Replace all occurrences of <search string> with <replace string> in <input text>.
       - Output the result to standard output (stdout).
     If the input is incomplete (e.g., <search string> or <replace string> is missing),
     the utility:
       - Will output "ERROR" to stdout.
       - Will terminate with a return code of 0.

Error Handling:
  - In File Mode:
    - If the number of arguments is incorrect, "ERROR" is output to stdout, and the program terminates with a code of 1.
    - If the input file cannot be read or the output file cannot be written, "ERROR" is output to stdout, and the program terminates with a code of 1.
  - In Stdin Mode:
    - If the input is incomplete (e.g., the user presses Ctrl+Z on Windows or Ctrl+D on Linux), "ERROR" is output to stdout, and the program terminates with a code of 0.
)";

enum class Mode
{
	Input,
	Arguments,
	Help,
};

Mode ParseArgs(int argc, char* argv[]);
int Processing(char* argv[], Mode mode);
int PrintDoc();
int ArgumentsProcessing(char* argv[]);
int InputProcessing();

bool CopyStreamWithReplace(
	std::istream& inStream,
	std::ostream& outStream,
	const std::string& searchString,
	const std::string& replaceString);

std::string ReplaceString(
	const std::string& string,
	const std::string& searchString,
	const std::string& replaceString);
bool FindSubString(const std::string& subString, const std::string& string, size_t& pos);

class InvalidArgumentsNumberException : public std::invalid_argument
{
public:
	InvalidArgumentsNumberException()
		: std::invalid_argument("Invalid number of arguments")
	{
	}
};

int main(int argc, char* argv[])
{
	try
	{
		Mode mode = ParseArgs(argc, argv);
		return Processing(argv, mode);
	}
	catch (InvalidArgumentsNumberException*)
	{
		std::cout << "ERROR" << std::endl;
		return 1;
	}
}

bool CopyStreamWithReplace(
	std::istream& inStream,
	std::ostream& outStream,
	const std::string& searchString,
	const std::string& replaceString)
{
	std::string line;
	bool isEmpty = true;
	while (getline(inStream, line))
	{
		isEmpty = false;
		outStream << ReplaceString(line, searchString, replaceString) << std::endl;
	}
	return !isEmpty;
}

bool FindSubString(const std::string& subString, const std::string& string, size_t& pos)
{
	return !subString.empty() && (pos = string.find(subString, pos)) < string.length();
}

std::string ReplaceString(
	const std::string& string,
	const std::string& searchString,
	const std::string& replaceString)
{
	size_t prevPos = 0,
		   pos = 0;
	std::string resLine;
	while (FindSubString(searchString, string, pos))
	{
		resLine.append(string, prevPos, pos - prevPos);
		resLine.append(replaceString);
		pos += searchString.length();
		prevPos = pos;
	}
	resLine.append(string, prevPos, string.length() - prevPos);
	return resLine;
}

Mode ParseArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-h")
		{
			return Mode::Help;
		}
	}

	if (argc == 5)
	{
		return Mode::Arguments;
	}
	if (argc == 1)
	{
		return Mode::Input;
	}
	throw new InvalidArgumentsNumberException();
}

int Processing(char* argv[], Mode mode)
{
	switch (mode)
	{
	case Mode::Help:
		return PrintDoc();
	case Mode::Input:
		return InputProcessing();
	case Mode::Arguments:
		return ArgumentsProcessing(argv);
	}
	return 0;
}

int PrintDoc()
{
	std::cout << docMessage << std::endl;
	return 0;
}

int ArgumentsProcessing(char* argv[])
{
	std::ifstream inFile(argv[1]);
	if (!inFile.is_open())
	{
		std::cout << "ERROR" << std::endl;
		return 1;
	}
	std::ofstream outFile(argv[2]);
	if (!outFile.is_open())
	{
		std::cout << "ERROR" << std::endl;
		return 1;
	}
	CopyStreamWithReplace(inFile, outFile, argv[3], argv[4]);
	return 0;
}

int InputProcessing()
{
	std::string searchString;
	std::string replaceString;
	if (
		!getline(std::cin, searchString)
		|| !getline(std::cin, replaceString)
		|| !CopyStreamWithReplace(std::cin, std::cout, searchString, replaceString))
	{
		std::cout << "ERROR" << std::endl;
	}
	return 0;
}
