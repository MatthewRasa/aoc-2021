#include "common.h"
#include <algorithm>
#include <numeric>

using Entry = std::pair<std::vector<std::string>, std::vector<std::string>>;

static std::vector<Entry> read_input() {
	std::vector<Entry> entries;
	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<std::string> line_patterns, line_output;
		line_patterns.reserve(10);
		line_output.reserve(4);

		std::stringstream ss{line};
		std::string token;
		while (line_patterns.size() != line_patterns.capacity()) {
			ss >> token;
			line_patterns.push_back(std::move(token));
		}
		ss >> token;
		while (line_output.size() != line_output.capacity()) {
			ss >> token;
			line_output.push_back(std::move(token));
		}

		entries.push_back(std::make_pair(std::move(line_patterns), std::move(line_output)));
	}
	return entries;
}

static uint count_1478(const std::vector<Entry> &entries) {
	return std::transform_reduce(entries.begin(), entries.end(), 0u, std::plus{},
			[](const auto &entry) {
				return std::transform_reduce(entry.second.begin(), entry.second.end(), 0u, std::plus{},
						[](const auto &digit) { return digit.size() == 2 || digit.size() == 4 || digit.size() == 3 || digit.size() == 7; });
			});
}

static bool contains(const std::string &outer, const std::string &inner) {
	return std::all_of(inner.begin(), inner.end(), [&outer](auto c) { return std::find(outer.begin(), outer.end(), c) != outer.end(); });
}

template<class It>
static std::string remove(std::vector<std::string> &signals, It it) {
	const auto val = *it;
	signals.erase(it);
	return val;
}

static std::string grab_equal_length(std::vector<std::string> &signals, std::size_t length) {
	return remove(signals, std::find_if(signals.begin(), signals.end(), [length](const auto &signal) { return signal.size() == length; }));
}

static std::string grab_contains(std::vector<std::string> &signals, const std::string &inner) {
	return remove(signals, std::find_if(signals.begin(), signals.end(), [&inner](const auto &signal) { return contains(signal, inner); }));
}

static std::unordered_map<std::string, uint> deduce_digits(std::vector<std::string> signals) {
	std::for_each(signals.begin(), signals.end(), [](auto &signal) { std::sort(signal.begin(), signal.end()); });

	const auto digit1 = grab_equal_length(signals, 2);
	const auto digit4 = grab_equal_length(signals, 4);
	const auto digit7 = grab_equal_length(signals, 3);
	const auto digit8 = grab_equal_length(signals, 7);
	const auto digit9 = grab_contains(signals, digit4);

	auto digit0 = grab_equal_length(signals, 6);
	auto digit6 = grab_equal_length(signals, 6);
	if (contains(digit6, digit1))
		std::swap(digit0, digit6);

	auto digit2 = grab_equal_length(signals, 5);
	auto digit3 = grab_equal_length(signals, 5);
	auto digit5 = grab_equal_length(signals, 5);
	if (contains(digit2, digit1))
		std::swap(digit2, digit3);
	else if (contains(digit5, digit1))
		std::swap(digit3, digit5);
	if (contains(digit6, digit2))
		std::swap(digit2, digit5);

	return {{digit0, 0u}, {digit1, 1u}, {digit2, 2u}, {digit3, 3u}, {digit4, 4u}, {digit5, 5u}, {digit6, 6u}, {digit7, 7u}, {digit8, 8u}, {digit9, 9u}};
}

static uint count_all(const std::vector<Entry> &entries) {
	return std::transform_reduce(entries.begin(), entries.end(), 0u, std::plus{},
			[](const auto &entry) {
				const auto digit_map = deduce_digits(entry.first);
				return std::accumulate(entry.second.begin(), entry.second.end(), 0u, [&digit_map, place = 1000](auto sum, auto digit) mutable {
						std::sort(digit.begin(), digit.end());
						const auto val = place * digit_map.at(digit);
						place /= 10;
						return sum + val;
					});
			});
}

int main(int argc, char *argv[]) {
	auto entries = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << count_1478(entries) << std::endl;
		break;
	case 2:
		std::cout << count_all(entries) << std::endl;
		break;
	}
	return 0;
}
