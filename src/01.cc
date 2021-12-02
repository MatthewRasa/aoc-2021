#include "common.h"
#include <numeric>

static std::size_t count_increases(const std::vector<uint> &measurements) noexcept {
	std::size_t count{0};
	for (auto it = std::next(measurements.begin()); it != measurements.cend(); ++it)
		count += *it > *std::prev(it);
	return count;
}

static std::size_t count_increases_window(const std::vector<uint> &measurements) noexcept {
	std::size_t count{0};
	auto window = std::accumulate(measurements.cbegin(), measurements.cbegin() + 3, 0u);
	for (auto it = std::next(measurements.begin()); it != measurements.cend() - 2; ++it) {
		const auto prev_window = window;
		window = window - *std::prev(it) + *std::next(it, 2);
		count += window > prev_window;
	}
	return count;
}

int main(int argc, char *argv[]) {
	const auto measurements = read_input_uints();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << count_increases(measurements) << std::endl;
		break;
	case 2:
		std::cout << count_increases_window(measurements) << std::endl;
		break;
	}
	return 0;
}
