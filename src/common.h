#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <unordered_set>
#include <vector>

uint select_part(int argc, char *argv[]);

template<class T, class Iter>
std::pair<T, T> two_sum(Iter begin, Iter end, T total) {
	std::unordered_set<T> diffs;
	for (auto it = begin; it != end; ++it) {
		const auto diff = total - *it;
		if (diffs.find(*it) != diffs.end())
			return std::make_pair(diff, *it);
		diffs.insert(diff);
	}
	throw std::logic_error{"no match found"};
}

template<class T>
std::enable_if_t<std::is_integral_v<T>, T> parse_char(char c) {
	return c - '0';
}

template<class T>
std::enable_if_t<!std::is_integral_v<T>, T> parse_char(char c) {
	return static_cast<T>(c);
}

template<class T>
std::vector<T> parse_input_line_no_delim(const std::string &line) {
	std::vector<T> vals;
	vals.reserve(line.size());
	for (auto c : line)
		vals.push_back(parse_char<T>(c));
	return vals;
}

template<class T>
std::vector<T> parse_input_line_space_delim(const std::string &line) {
	std::vector<T> vals;
	std::stringstream line_stream{line};
	for (T val; line_stream >> val; )
		vals.push_back(val);
	return vals;
}

template<class T>
std::vector<T> parse_input_line_comma_delim(const std::string &line) {
	std::stringstream line_stream{line};
	std::vector<T> vals;
	T val;
	for (std::string str_val; std::getline(line_stream, str_val, ','); ) {
		if (!(std::stringstream{str_val} >> val))
			throw std::runtime_error{"parsing error"};
		vals.push_back(val);
	}
	return vals;
}

std::vector<uint> read_input_uints();

template<class T>
std::vector<T> read_input_objs() {
	std::vector<T> input;
	for (std::string line; std::getline(std::cin, line); )
		input.emplace_back(std::move(line));
	return input;
}

template<class T, class Conv>
std::vector<T> read_input(Conv &&conv) {
	std::vector<T> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(conv(std::move(line)));
	return input;
}

template<class T>
std::vector<T> read_input_line_comma_delim() {
	std::string line;
	std::getline(std::cin, line);
	return parse_input_line_comma_delim<T>(line);
}

#endif /* COMMON_H_ */
