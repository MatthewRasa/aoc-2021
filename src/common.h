#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
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

#endif /* COMMON_H_ */
