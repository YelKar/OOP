#include "CLIParser.h"
#include "Matrix.h"
#include <fstream>
#include <iostream>

enum class Mode {
	CLI,
	STDIN,
	HELP,
};

struct CLIInfo {
	std::string matrix1;
	std::string matrix2;
};

struct ModeInfo {
	Mode mode{};
	CLIInfo cliInfo;
};

using matrixNumberType = double;
constexpr struct {
	std::size_t M;
	std::size_t N;
} MATRIX_SIZE {3, 3};

using MatrixType = Matrix<MATRIX_SIZE.M, MATRIX_SIZE.N, matrixNumberType>;

class InvalidArgumentsNumberException : public std::invalid_argument
{
public:
	InvalidArgumentsNumberException(): std::invalid_argument("Invalid number of arguments"){}
};

class FileNotFoundException: public std::invalid_argument
{
public:
	explicit FileNotFoundException(const std::string& fileName): std::invalid_argument("File '" + fileName + "' is not found"){}
};

class InvalidMatrixException : public std::invalid_argument
{
public:
	explicit InvalidMatrixException(const std::string& error): std::invalid_argument(error){}
};

class StdInException : public std::invalid_argument
{
public:
	explicit StdInException(const std::string& text): std::invalid_argument(text){}
};

ModeInfo ParseArgs(int argc, char *argv[]);
void ProcessMode(const ModeInfo& mode);
void ProcessCLI(const CLIInfo& info);
void ProcessStdIn();
void ProcessHelp();
MatrixType ParseMatrix(std::istream& mtxStream);
void PrintMatrix(const MatrixType& mtx);


int main(int argc, char *argv[])
{
	try
	{
		ModeInfo mode = ParseArgs(argc, argv);
		ProcessMode(mode);
		return 0;
	}
	catch (StdInException&) {
		std::cout << "ERROR" << std::endl;
		return 0;
	} catch (std::exception&) {
		std::cout << "ERROR" << std::endl;
		return 1;
	}
}

ModeInfo ParseArgs(int argc, char *argv[]) {
	tools::CLI::Parser parser({
		.positionalArgs={
			{ "input", 1 },
			{ "output", 2 },
		}
	});
	parser.Parse(argc, argv);
	switch (parser.CountOfParsedArgs())
	{
	case 0:
		return { Mode::STDIN };
	}
	return {
		Mode::CLI,
		parser["input"],
		parser["output"],
	};
}

void ProcessMode(const ModeInfo& mode) {
	switch (mode.mode)
	{
	case Mode::CLI:
		ProcessCLI(mode.cliInfo);
		break;
	case Mode::STDIN:
		ProcessStdIn();
		break;
	case Mode::HELP:
		ProcessHelp();
		break;
	}
}

void ProcessCLI(const CLIInfo& info) {
	std::string inputFileName = info.matrix1;
	std::string outputFileName = info.matrix2;

	std::ifstream mtx1File(inputFileName);
	if (!mtx1File.is_open()) {
		throw FileNotFoundException(inputFileName);
	}

	std::ifstream mtx2File(outputFileName);
	if (!mtx2File.is_open()) {
		throw FileNotFoundException(outputFileName);
	}

	auto mtx1 = ParseMatrix(mtx1File);
	auto mtx2 = ParseMatrix(mtx2File);

	PrintMatrix(mtx1 * mtx2);
}

void ProcessStdIn() {
	try {
		auto mtx1 = ParseMatrix(std::cin);
		auto mtx2 = ParseMatrix(std::cin);

		PrintMatrix(mtx1 * mtx2);
	}
	catch (std::exception& e)
	{
		throw StdInException(e.what());
	}
}

void ProcessHelp() {

}

MatrixType ParseMatrix(std::istream& mtxStream) {
	MatrixType matrix;

	std::string line;

	for (int i = 0; i < MATRIX_SIZE.M; i++) {
		if (!getline(mtxStream, line)) {
			throw InvalidMatrixException("Too few lines for matrix 3x3.");
		}
		if (line.empty()) {
			i--;
			continue;
		}

		std::stringstream lineStream(line);
		int j = 0;
		for (matrixNumberType value; lineStream >> value; j++) {
			if (j >= MATRIX_SIZE.N) {
				throw InvalidMatrixException("Too much elements in row for matrix 3x3.");
			}
			matrix[i][j] = value;
		}

		if (j != 3) {
			throw InvalidMatrixException("Too few elements in row for matrix 3x3.");
		}
	}

	return matrix;
}

void PrintMatrix(const MatrixType& mtx) {
	for (const auto& row : mtx) {
		bool isFirstElt = true;
		for (matrixNumberType elt : row) {
			if (!isFirstElt) {
				std::cout << " ";
			}
			std::printf("%.3f", elt);
			isFirstElt = false;
		}
		std::cout << std::endl;
	}
}
