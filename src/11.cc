#include "common.h"

static void increment_levels(std::vector<std::vector<std::uint8_t>> &levels) {
	for (auto &row : levels)
		for (auto &level : row)
			++level;
}

static void flash_adjacent(std::vector<std::vector<std::uint8_t>> &levels, std::vector<std::vector<std::uint8_t>> &flash_map,
		std::size_t r, std::size_t c) {
	if (r > 0) {
		if (!flash_map[r - 1][c])
			++levels[r - 1][c];
		if (c > 0 && !flash_map[r - 1][c - 1])
			++levels[r - 1][c - 1];
		if (c < levels[r].size() - 1 && !flash_map[r - 1][c + 1])
			++levels[r - 1][c + 1];
	}
	if (r < levels.size() - 1) {
		if (!flash_map[r + 1][c])
			++levels[r + 1][c];
		if (c > 0 && !flash_map[r + 1][c - 1])
			++levels[r + 1][c - 1];
		if (c < levels[r].size() - 1 && !flash_map[r + 1][c + 1])
			++levels[r + 1][c + 1];
	}
	if (c > 0 && !flash_map[r][c - 1])
		++levels[r][c - 1];
	if (c < levels[r].size() - 1 && !flash_map[r][c + 1])
		++levels[r][c + 1];
}

static uint count_step_flashes(std::vector<std::vector<std::uint8_t>> &levels) {
	std::vector<std::vector<std::uint8_t>> flash_map(levels.size(), std::vector<std::uint8_t>(levels[0].size(), false));
	uint num_flashes{0}, prev_flashes;
	do {
		prev_flashes = num_flashes;
		for (std::size_t r = 0; r < levels.size(); ++r) {
			for (std::size_t c = 0; c < levels[r].size(); ++c) {
				if (levels[r][c] > 9) {
					++num_flashes;
					flash_map[r][c] = true;
					levels[r][c] = 0;
					flash_adjacent(levels, flash_map, r, c);
				}
			}
		}
	} while (num_flashes != prev_flashes);
	return num_flashes;
}

static uint count_flashes(std::vector<std::vector<std::uint8_t>> levels, uint steps) {
	uint num_flashes{0};
	while (steps-- > 0) {
		increment_levels(levels);
		num_flashes += count_step_flashes(levels);
	}
	return num_flashes;
}

static uint first_simultaneous_flash(std::vector<std::vector<std::uint8_t>> levels) {
	const auto max_flashes = levels.size() * levels[0].size();
	for (uint step = 1; ; ++step) {
		increment_levels(levels);
		if (count_step_flashes(levels) == max_flashes)
			return step;
	}
	throw std::runtime_error{"no solution found"};
}

int main(int argc, char *argv[]) {
	auto levels = read_input<std::vector<std::uint8_t>>([](const auto &line) { return parse_input_line_no_delim<std::uint8_t>(line); });
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << count_flashes(std::move(levels), 100) << std::endl;
		break;
	case 2:
		std::cout << first_simultaneous_flash(std::move(levels)) << std::endl;
		break;
	}
	return 0;
}
