
#include <format>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace tools::CLI
{
class ArgsSpecification
{
public:
	std::string shortArgs{};
	std::string shortKeyArgs{};
	std::set<std::string> longArgs{};
	std::set<std::string> longKeyArgs{};
	std::map<std::string, std::size_t> positionalArgs{};
};

class ParsingResult
{
public:
	std::set<char> shortArgs{};
	std::map<char, std::string> shortKeyArgs{};
	std::set<std::string> longArgs{};
	std::map<std::string, std::string> longKeyArgs{};
	std::map<std::string, std::string> positionalArgs{};
};

class Parser
{
public:
	explicit Parser();
	explicit Parser(tools::CLI::ArgsSpecification);
	void SetSpecification(tools::CLI::ArgsSpecification);
	void Parse(const std::vector<std::string>&);
	void Parse(int argc, char *argv[]);
	std::string operator[](const std::string&) const;
	std::string operator[](char) const;
	std::string operator[](std::size_t) const;
	bool operator<<(const std::string&) const;
	bool operator<<(char) const;
	[[nodiscard]] bool Contains(char) const;
	[[nodiscard]] bool Contains(const std::string&) const;
	bool Get(char key, std::string& value) const;
	bool Get(const std::string& key, std::string& value) const;
	[[nodiscard]] std::size_t Size() const;
	[[nodiscard]] std::size_t CountOfParsedArgs() const;
private:
	ArgsSpecification argsSpecification;
	ParsingResult result;
	void ValidateSpecification();
	void ValidateShortArgsSpecification();
	void ValidateLongArgsSpecification() const;
	void ValidatePositionalArgsSpecification();
	void ProcessArgs(const std::vector<std::string>&);
	bool ProcessShortArg(const std::string&);
	bool ProcessLongArg(const std::string&);
	bool ProcessShortKeyArg(std::vector<std::string>::const_iterator& keyIt, const std::vector<std::string>::const_iterator& end);
	bool ProcessLongKeyArg(const std::string&);
	bool ProcessPositionalArg(std::size_t index, const std::string& value);
	std::vector<std::string> allArgs;
};

class ArgumentNotFoundException : public std::out_of_range {
public:
	explicit ArgumentNotFoundException(char key);
	explicit ArgumentNotFoundException(const std::string& key);
	explicit ArgumentNotFoundException(std::size_t key);
};
} // namespace tools::CLI
