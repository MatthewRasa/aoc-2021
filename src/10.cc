#include "common.h"
#include <algorithm>
#include <numeric>
#include <unordered_map>

static const std::unordered_map<char, char> OPEN_TOKEN_MATCHES{{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
static const std::unordered_map<char, uint> CLOSE_TOKEN_POINTS{{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
static const std::unordered_map<char, std::uint64_t> AUTOCOMPLETE_POINTS{{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};

static uint line_error_score(const std::string &line) {
	std::vector<char> stack;
	stack.reserve(line.size());
	for (auto c : line) {
		auto close_it = CLOSE_TOKEN_POINTS.find(c);
		if (close_it == CLOSE_TOKEN_POINTS.cend()) {
			stack.push_back(OPEN_TOKEN_MATCHES.at(c));
		} else {
			if (c != stack.back())
				return close_it->second;
			stack.pop_back();
		}
	}
	return 0;
}

static uint total_error_score(const std::vector<std::string> &lines) {
	return std::transform_reduce(lines.begin(), lines.end(), 0u, std::plus{}, [](const auto &line) { return line_error_score(line); });
}

static std::vector<char> line_autocomplete_sequence(const std::string &line) {
	std::vector<char> stack;
	stack.reserve(line.size());
	for (auto c : line) {
		if (CLOSE_TOKEN_POINTS.find(c) == CLOSE_TOKEN_POINTS.cend())
			stack.push_back(OPEN_TOKEN_MATCHES.at(c));
		else
			stack.pop_back();
	}
	std::reverse(stack.begin(), stack.end());
	return stack;
}

static std::uint64_t line_autocomplete_score(const std::string &line) {
	std::uint64_t score{0};
	for (auto c : line_autocomplete_sequence(line))
		score = score * 5 + AUTOCOMPLETE_POINTS.at(c);
	return score;
}

static std::uint64_t autocomplete_score(const std::vector<std::string> &lines) {
	std::vector<std::uint64_t> scores;
	scores.reserve(lines.size());
	std::transform(lines.begin(), lines.end(), std::back_inserter(scores), [](const auto &line) { return line_autocomplete_score(line); });
	std::sort(scores.begin(), scores.end());
	return scores[scores.size() / 2];
}

static std::uint64_t total_autocomplete_score(std::vector<std::string> lines) {
	lines.erase(std::remove_if(lines.begin(), lines.end(), [](const auto &line) { return line_error_score(line) != 0; }), lines.cend());
	return autocomplete_score(lines);
}

int main(int argc, char *argv[]) {
	auto lines = read_input_objs<std::string>();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << total_error_score(std::move(lines)) << std::endl;
		break;
	case 2:
		std::cout << total_autocomplete_score(std::move(lines)) << std::endl;
		break;
	}
	return 0;
}
