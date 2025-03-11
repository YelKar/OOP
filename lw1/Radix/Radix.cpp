#include <cmath>
#include <iostream>
#include "SoftNumber.h"

std::string docMessage = R"(
Radix Utility - Version 1.0

Usage:
  radix <source notation> <destination notation> <value>

Description:
  The utility converts a number from one numeral system to another.
  Supported numeral systems have bases ranging from 2 to 36. For bases from 11 to 36,
  uppercase Latin letters (A-Z) are used to represent values from 10 to 35.

Parameters:
  <source notation>    - the source numeral system (from 2 to 36).
  <destination notation> - the target numeral system (from 2 to 36).
  <value>              - the number in the source numeral system to be converted.

Examples:
  1. Convert the hexadecimal number 1F to decimal:
     radix 16 10 1F
     Result: 31

  2. Convert the binary number 1010 to octal:
     radix 2 8 1010
     Result: 12

  3. Convert the decimal number -255 to hexadecimal:
     radix 10 16 -255
     Result: -FF

Error handling:
  - If the source or target numeral system is outside the valid range (2-36),
    the program will display an error message and terminate.
  - If the input value contains invalid characters for the specified numeral system,
    the program will display an error message and terminate.
  - If an overflow occurs during conversion, the program will display an error message
    and terminate.

To display this help, use the -h or --help option:
  radix -h
  radix --help
)";

class RadixLimit {
public:
	static const unsigned short MIN = 2;
	static const unsigned short MAX = 36;
};

enum class Mode
{
	Arguments,
	Help,
};

std::string IntToString(int n, unsigned short base);
int StringToInt(const std::string& str, SoftNumber<unsigned short> base);
unsigned short CharToNum(char c, SoftNumber<unsigned short> base);
char NumToChar(int n, unsigned short base);

Mode ParseArgs(int argc, char* argv[]);

void Processing(char* argv[], Mode mode);
void PrintDoc();
void ArgumentsProcessing(const std::string& n, SoftNumber<unsigned short> fromBase, SoftNumber<unsigned short> toBase);
bool IsValidBase(unsigned short base);

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
		Processing(argv, mode);
		return 0;
	}
	catch (std::exception&)
	{
		std::cout << "ERROR" << std::endl;
		return 1;
	}
}

Mode ParseArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
		{
			return Mode::Help;
		}
	}

	if (argc == 4)
	{
		return Mode::Arguments;
	}
	throw InvalidArgumentsNumberException();
}

void PrintDoc()
{
	std::cout << docMessage << std::endl;
}

void ArgumentsProcessing(const std::string& n, SoftNumber<unsigned short> fromBase, SoftNumber<unsigned short> toBase)
{
	int intValue = StringToInt(n, fromBase);
	std::string result = IntToString(intValue, toBase);
	std::cout << result << std::endl;
}

void Processing(char* argv[], Mode mode)
{
	switch (mode)
	{
	case Mode::Help:
		PrintDoc();
		break;
	case Mode::Arguments:
		SoftNumber<unsigned short>
		    fromBase = StringToInt(argv[1], 10),
			toBase = StringToInt(argv[2], 10);
		ArgumentsProcessing(argv[3], fromBase, toBase);
		break;
	}
}

std::string IntToString(int n, unsigned short base)
{
	if (IsValidBase(base))
	{
		throw std::overflow_error("base overflow");
	}
	std::string resultNum;
	bool isNegative = (n < 0);
	for (; n; n /= base)
	{
		resultNum.insert(0, 1, NumToChar(std::abs(n % base), base));
	}
	if (isNegative)
	{
		resultNum.insert(0, 1, '-');
	}
	return !resultNum.empty() ? resultNum : "0";
}

int StringToInt(const std::string& str, SoftNumber<unsigned short> base)
{
	if (IsValidBase(base))
	{
		throw std::overflow_error("base overflow");
	}
	SoftNumber<int> result = 0;
	bool isNegative = str[0] == '-';
	for (size_t i = isNegative; i < str.length(); i++)
	{
		result = result * SoftNumber<int>(base) + SoftNumber<int>(CharToNum(str[i], base)) * SoftNumber(isNegative ? -1 : 1);
	}
	return result.number;
}

unsigned short CharToNum(char c, SoftNumber<unsigned short> base)
{
	if (base > RadixLimit::MAX)
	{
		throw std::overflow_error("base overflow");
	}
	if ('0' <= c && c < '0' + (base <= 10 ? base : SoftNumber(10)))
	{
		return c - '0';
	}
	if ('A' <= c && c < 'A' + base - 10)
	{
		return c - 'A' + 10;
	}
	throw std::overflow_error("base overflow");
}

char NumToChar(int n, unsigned short base)
{
	if (base > RadixLimit::MAX)
	{
		throw std::overflow_error("base overflow");
	}
	if (n >= base)
	{
		throw std::overflow_error("small base");
	}
	if (n < 0 || n >= RadixLimit::MAX) {
		throw std::overflow_error("wrong number to char transformation");
	}
	if (n < 10)
	{
		return n + SoftNumber('0');
	}
	return n + SoftNumber('A') - SoftNumber(10);
}

bool IsValidBase(unsigned short base) {
	return RadixLimit::MIN > base || base > RadixLimit::MAX;
}
