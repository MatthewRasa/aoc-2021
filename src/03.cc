#include "common.h"
#include <bitset>
#include <functional>
#include <list>
#include <numeric>

using Code = std::bitset<12>;

template<class It>
static bool most_common_bit(It begin, It end, std::size_t pos) {
	return std::accumulate(begin, end, 0ul,
				[pos](auto sum, const auto &code) { return sum + code[pos]; }) >= static_cast<std::size_t>(std::distance(begin, end) + 1)/2;
}

static std::pair<Code, Code> gamma_epsilon(const std::vector<Code> &codes) {
	Code gamma;
	for (std::size_t pos = 0; pos < codes[0].size(); ++pos)
		gamma[pos] = most_common_bit(codes.begin(), codes.end(), pos);
	return std::make_pair(gamma, ~gamma);
}

static auto power_consumption(const std::vector<Code> &codes) {
	const auto [gamma, epsilon] = gamma_epsilon(codes);
	return gamma.to_ulong() * epsilon.to_ulong();
}

static void filter_bit_pos(std::list<Code> &codes_list, std::size_t pos, bool filter_bit) {
	for (auto it = codes_list.begin(); it != codes_list.end() && codes_list.size() > 1; ) {
		if ((*it)[pos] != filter_bit)
			it = codes_list.erase(it);
		else
			++it;
	}
}

template<class Filter_Func>
static Code filter_search(const std::vector<Code> &codes, const Filter_Func &filter) {
	std::list<Code> codes_list{codes.begin(), codes.end()};
	for (ssize_t pos = codes[0].size() - 1; pos >= 0; --pos) {
		filter_bit_pos(codes_list, pos, filter(codes_list.begin(), codes_list.end(), pos));
		if (codes_list.size() == 1)
			return codes_list.front();
	}
	throw std::runtime_error{"No match found"};
}

static auto life_support_rating(const std::vector<Code> &codes) {
	return filter_search(codes, [](auto begin, auto end, auto pos) { return most_common_bit(begin, end, pos); }).to_ulong()
			* filter_search(codes, [](auto begin, auto end, auto pos) { return !most_common_bit(begin, end, pos); }).to_ulong();
}

int main(int argc, char *argv[]) {
	const auto codes = read_input_objs<Code>();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << power_consumption(codes) << std::endl;
		break;
	case 2:
		std::cout << life_support_rating(codes) << std::endl;
		break;
	}
	return 0;
}
