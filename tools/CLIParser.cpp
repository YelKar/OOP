#include "CLIParser.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace tools::CLI
{
Parser::Parser() = default;

Parser::Parser(tools::CLI::ArgsSpecification argsSpecification_)
{
	SetSpecification(std::move(argsSpecification_));
}

void Parser::SetSpecification(tools::CLI::ArgsSpecification argsSpecification_) {
	argsSpecification = std::move(argsSpecification_);
	ValidateSpecification();
}

void Parser::ValidateSpecification() {
	ValidateShortArgsSpecification();
	ValidateLongArgsSpecification();
	ValidatePositionalArgsSpecification();
}

void Parser::ValidateShortArgsSpecification() {
	std::set<char> argsIntersection;
	std::set_intersection(
		argsSpecification.shortArgs.begin(), argsSpecification.shortArgs.end(),
		argsSpecification.shortKeyArgs.begin(), argsSpecification.shortKeyArgs.end(),
		std::inserter(argsIntersection, argsIntersection.begin())
	);
	if (!argsIntersection.empty()) {
		throw std::invalid_argument("Short args have same names");
	}
}

void Parser::ValidateLongArgsSpecification() const {
	std::set<std::string> argsIntersection;
	std::set_intersection(
		argsSpecification.longArgs.begin(), argsSpecification.longArgs.end(),
		argsSpecification.longKeyArgs.begin(), argsSpecification.longKeyArgs.end(),
		std::inserter(argsIntersection, argsIntersection.begin())
	);
	if (!argsIntersection.empty()) {
		throw std::invalid_argument("Long args have same names");
	}
}

void Parser::ValidatePositionalArgsSpecification() {
	std::set<std::string> argsIntersection;
	std::set<std::string> allLongArgs;
	std::set<std::string> positionalArgNames;

	std::set_union(
		argsSpecification.longArgs.begin(), argsSpecification.longArgs.end(),
		argsSpecification.longKeyArgs.begin(), argsSpecification.longKeyArgs.end(),
		std::inserter(allLongArgs, allLongArgs.begin())
	);
	std::transform(
		argsSpecification.positionalArgs.begin(), argsSpecification.positionalArgs.end(),
		std::inserter(positionalArgNames, positionalArgNames.begin()),
		[] (auto arg) {
			return arg.first;
		}
	);
	std::set_intersection(
		allLongArgs.begin(), allLongArgs.end(),
		positionalArgNames.begin(), positionalArgNames.end(),
		std::inserter(argsIntersection, argsIntersection.begin())
	);
	if (!argsIntersection.empty()) {
		throw std::invalid_argument("Long and positional args have same names");
	}
}

void Parser::Parse(int argc, char** argv) {
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++) {
		args.emplace_back(argv[i]);
	}
	Parse(args);
}

void Parser::Parse(const std::vector<std::string>& args) {
	allArgs = args;
	ProcessArgs(args);
}

void Parser::ProcessArgs(const std::vector<std::string>& args) {
	for (auto [argsIt, index] = std::tuple{ args.begin(), std::size_t(0) }; argsIt != args.end(); argsIt++, index++) {
		ProcessPositionalArg(index, *argsIt);

		argsIt + 1 != args.end() && ProcessShortKeyArg(argsIt, args.end())
			|| ProcessLongKeyArg(*argsIt)
			|| ProcessShortArg(*argsIt)
			|| ProcessLongArg(*argsIt);
	}
}

bool Parser::ProcessShortArg(const std::string& arg) {
	if (!(arg.length() > 1 && arg[0] == '-' && arg[1] != '-')) {
		return false;
	}
	bool isFound = false;
	for (char flag : arg) {
		if (flag == '-') {
			continue;
		}
		if (argsSpecification.shortArgs.find(flag) != std::string::npos) {
			result.shortArgs.insert(flag);
			isFound = true;
		}
	}
	return isFound;
}

bool Parser::ProcessLongArg(const std::string& arg)
{
	if (
		arg.length() > 2
		&& arg.substr(0, 2) == "--" && arg[2] != '-'
		&& argsSpecification.longArgs.contains(arg.substr(2))
	) {
		result.longArgs.insert(arg.substr(2));
		return true;
	}
	return false;
}

bool Parser::ProcessShortKeyArg(std::vector<std::string>::const_iterator& keyIt, const std::vector<std::string>::const_iterator& end) {
	if (keyIt + 1 == end) {
		return false;
	}
	const std::string& value = *keyIt;
	if (
		value.length() == 2 && value[0] == '-' && value[1] != '-'
		&& argsSpecification.shortKeyArgs.find(value[1]) != std::string::npos
	) {
		result.shortKeyArgs[value[1]] = *(++keyIt);
		return true;
	}
	return false;
}

bool Parser::ProcessLongKeyArg(const std::string& key) {
	auto eqPos = key.find('=');
	if (
		key.length() > 2
		&& key[0] == '-' && key[1] == '-' && key[2] != '-'
		&& eqPos != std::string::npos
		&& argsSpecification.longKeyArgs.contains(key.substr(2, eqPos - 2))
	) {
		result.longKeyArgs[key.substr(2, eqPos - 2)] = key.substr(eqPos + 1);
		return true;
	}
	return false;
}

bool Parser::ProcessPositionalArg(std::size_t index, const std::string& value){
	auto pair = std::find_if(argsSpecification.positionalArgs.begin(), argsSpecification.positionalArgs.end(), [index](auto pair) {
		return pair.second == index;
	});
	if (pair != argsSpecification.positionalArgs.end()) {
		result.positionalArgs[pair->first] = value;
		return true;
	}
	return false;
}

std::string Parser::operator[](char key) const {
	try {
		return result.shortKeyArgs.at(key);
	} catch (std::out_of_range&) {
		throw ArgumentNotFoundException(key);
	}
}

std::string Parser::operator[](const std::string& key) const
{
	try {
		if (result.longKeyArgs.contains(key)) {
			return result.longKeyArgs.at(key);
		}
		return result.positionalArgs.at(key);
	} catch (std::out_of_range&) {
		throw ArgumentNotFoundException(key);
	}
}


std::string Parser::operator[](std::size_t key) const
{
	try {
		return allArgs[key];
	} catch (std::out_of_range&) {
		throw ArgumentNotFoundException(key);
	}
}

bool Parser::operator<<(const std::string& arg) const
{
	return Contains(arg);
}

bool Parser::operator<<(char arg) const
{
	return Contains(arg);
}

bool Parser::Contains(const std::string& arg) const {
	return result.longArgs.contains(arg) || result.longKeyArgs.contains(arg) || result.positionalArgs.contains(arg);
}

bool Parser::Contains(char arg) const {
	return result.shortArgs.contains(arg) || result.shortKeyArgs.contains(arg);
}

bool Parser::Get(char key, std::string& value) const
{
	if (!result.shortKeyArgs.contains(key)) {
		return false;
	}
	value = result.shortKeyArgs.at(key);
	return true;
}

bool Parser::Get(const std::string& key, std::string& value) const
{
	if (result.longKeyArgs.contains(key)) {
		value = result.longKeyArgs.at(key);
		return true;
	}
	if (result.positionalArgs.contains(key)) {
		value = result.positionalArgs.at(key);
		return true;
	}
	return false;
}
std::size_t Parser::Size() const
{
	return allArgs.size();
}
std::size_t Parser::CountOfParsedArgs() const
{
	return
		+ result.shortKeyArgs.size()
		+ result.longKeyArgs.size()
		+ result.shortArgs.size()
		+ result.longArgs.size()
		+ result.positionalArgs.size();
}

ArgumentNotFoundException::ArgumentNotFoundException(std::size_t key): std::out_of_range(std::format("Argument `number({})` is not found", key)) {};
ArgumentNotFoundException::ArgumentNotFoundException(char key): std::out_of_range(std::format("Argument `char({})` is not found", key)) {};
ArgumentNotFoundException::ArgumentNotFoundException(const std::string& key): std::out_of_range("Argument `string(" + key + ")` is not found") {};
}

