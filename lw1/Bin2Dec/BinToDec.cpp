#include <iostream>
#include <cmath>
#include <cinttypes>

std::string docMessage = R"(
Bin2Dec Utility - Version 1.0

Usage:
  bin2dec.exe <binary number>
  replace.exe

Description:
  The utility will convert the number from binary to decimal

Operation Modes:
  1. CLI Args Mode:
     If arguments are provided in the format:
       replace.exe <binary number>
     The utility will perform the following actions:
       - Convert a number from binary to decimal and outputs it to the standard output stream.
       - Output the result to standard output (stdout).
     If the number of arguments is incorrect, the utility:
       - Will output "ERROR" to stdout.
       - Will terminate with a return code of 1.

  2. Stdin Mode:
     If no arguments are provided, the utility will read number from standard input (stdin).
     The utility will perform the following actions:
       - Convert a number from binary to decimal and outputs it to the standard output stream.
       - Output the result to standard output (stdout).

Error Handling:
  - In File Mode:
    - If the number of arguments is incorrect, "ERROR" is output to stdout, and the program terminates with a code of 1.
    - If the string contains non-binary characters, "ERROR" is output to stdout, and the program terminates with a code of 1.
  - In Stdin Mode:
    - If the input is incomplete (e.g., the user presses Ctrl+Z on Windows or Ctrl+D on Linux), "ERROR" is output to stdout, and the program terminates with a code of 0.
    - If the string contains non-binary characters, "ERROR" is output to stdout, and the program terminates with a code of 0.
)";

enum class Mode {
	Input,
	Arguments,
	Help,
};

bool BinToDec(const std::string& binNum, std::uint32_t& resNum);
std::uint32_t CharToNum(char);
bool IsValidBinaryNumber(std::uint32_t);

Mode ParseArgs(int argc, char *argv[]);

int Processing(char *argv[], Mode mode);
int PrintDoc();
int ArgumentsProcessing(char *argv[]);
int InputProcessing();

class InvalidArgumentsNumberException : public std::invalid_argument
{
public:
	InvalidArgumentsNumberException(): std::invalid_argument("Invalid number of arguments"){}
};


int main(int argc, char *argv[])
{
	try {
		Mode mode = ParseArgs(argc, argv);
		return Processing(argv, mode);
	} catch (InvalidArgumentsNumberException*) {
		std::cout << "ERROR" << std::endl;
		return 1;
	}
}


Mode ParseArgs(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-h")
		{
			return Mode::Help;
		}
	}

	if (argc == 2) {
		return Mode::Arguments;
	}
	if (argc == 1) {
		return Mode::Input;
	}
	throw new InvalidArgumentsNumberException();
}

int PrintDoc() {
	std::cout << docMessage << std::endl;
	return 0;
}

int InputProcessing() {
	std::uint32_t resNum;
	std::string binNum;
	if (!getline(std::cin, binNum) || !BinToDec(binNum, resNum)) {
		std::cout << "ERROR" << std::endl;
	}
	else
	{
		std::cout << resNum << std::endl;
	}
	return 0;
}

int ArgumentsProcessing(char *argv[]) {  // TODO std::string
	std::uint32_t resNum;
	if (!BinToDec(argv[1], resNum)) {
		std::cout << "ERROR" << std::endl;
		return 1;
	}
	else
	{
		std::cout << resNum << std::endl;
	}
	return 0;
}

int Processing(char *argv[], Mode mode) {  // TODO argv заменить на структуру
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

bool BinToDec(const std::string& binNum, std::uint32_t& resNum) {
	resNum = 0;
	if (binNum.empty()) {
		return false;
	}
	for (size_t i = 0; i < binNum.length(); i++) {
		if (!IsValidBinaryNumber(binNum[i]) || (resNum >> 31 & 1)) {
			return false;
		}
		resNum <<= 1;
		resNum += CharToNum(binNum[i]);
	}
	return true;
}

std::uint32_t CharToNum(char c) {
	return c - '0';
}

bool IsValidBinaryNumber(std::uint32_t n) {
	return '0' == n || n == '1';
}
