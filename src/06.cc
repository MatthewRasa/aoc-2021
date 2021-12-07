#include "common.h"
#include <algorithm>
#include <numeric>

std::size_t simulate_laternfish(const std::vector<uint> &fish, uint days) {
	std::vector<std::size_t> fish_timers(9, 0);
	for (auto timer : fish)
		++fish_timers[timer];

	while (days-- > 0) {
		std::rotate(fish_timers.begin(), std::next(fish_timers.begin()), fish_timers.end());
		fish_timers[6] += fish_timers.back();
	}
	return std::accumulate(fish_timers.begin(), fish_timers.end(), std::size_t{0}, std::plus{});
}

int main(int argc, char *argv[]) {
	std::cout << simulate_laternfish(read_input_comma_sep_line<uint>(), select_part(argc, argv) == 1 ? 80 : 256) << std::endl;
	return 0;
}
