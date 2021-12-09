#include "common.h"
#include <algorithm>
#include <numeric>

static uint alignment_fuel_constant(const std::vector<int> &crabs) {
	return std::accumulate(crabs.begin(), crabs.end(), 0u,
			[align_pos = crabs[crabs.size() / 2]](auto sum, auto crab) { return sum + std::abs(align_pos - crab); });
}

static uint distance_fuel(uint distance) {
	uint fuel{0};
	for (uint i = 1; i <= distance; ++i)
		fuel += i;
	return fuel;
}

static uint alignment_fuel_linear(const std::vector<int> &crabs) {
	uint fuel = -1;
	for (int align_pos = 0; align_pos <= crabs.back(); ++align_pos)
		fuel = std::min(fuel, std::accumulate(crabs.begin(), crabs.end(), 0u,
					[align_pos](auto sum, auto crab) { return sum + distance_fuel(std::abs(align_pos - crab)); }));
	return fuel;
}

int main(int argc, char *argv[]) {
	auto crabs = read_input_comma_sep_line<int>();
	std::sort(crabs.begin(), crabs.end());
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << alignment_fuel_constant(std::move(crabs)) << std::endl;
		break;
	case 2:
		std::cout << alignment_fuel_linear(std::move(crabs)) << std::endl;
		break;
	}
}
