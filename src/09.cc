#include "common.h"
#include <algorithm>
#include <numeric>

using Heightmap = std::vector<std::vector<std::uint8_t>>;

static Heightmap read_input() {
	Heightmap heightmap;
	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<std::uint8_t> row;
		row.reserve(line.size());
		std::transform(line.begin(), line.end(), std::back_inserter(row), [](auto c) { return c - '0'; });
		heightmap.push_back(std::move(row));
	}
	return heightmap;
}

static bool is_low_point(const Heightmap &heightmap, std::size_t r, std::size_t c) {
	const auto height = heightmap[r][c];
	if (r > 0 && heightmap[r - 1][c] <= height)
		return false;
	if (r < heightmap.size() - 1 && heightmap[r + 1][c] <= height)
		return false;
	if (c > 0 && heightmap[r][c - 1] <= height)
		return false;
	if (c < heightmap[r].size() - 1 && heightmap[r][c + 1] <= height)
		return false;
	return true;
}

static std::vector<std::pair<std::size_t, std::size_t>> find_low_points(const Heightmap &heightmap) {
	std::vector<std::pair<std::size_t, std::size_t>> low_points;
	for (std::size_t r = 0; r < heightmap.size(); ++r) {
		for (std::size_t c = 0; c < heightmap[r].size(); ++c) {
			if (is_low_point(heightmap, r, c))
				low_points.push_back(std::make_pair(r, c));
		}
	}
	return low_points;
}

static uint risk_level(const Heightmap &heightmap) {
	const auto low_points = find_low_points(heightmap);
	return std::transform_reduce(low_points.begin(), low_points.end(), 0u, std::plus{},
			[&heightmap](const auto &pair) { return heightmap[pair.first][pair.second] + 1; });
}

static uint calc_basin_size_impl(Heightmap &heightmap, std::size_t r, std::size_t c) {
	uint size{1};
	heightmap[r][c] = 9;
	if (r > 0 && heightmap[r - 1][c] != 9)
		size += calc_basin_size_impl(heightmap, r - 1, c);
	if (r < heightmap.size() - 1 && heightmap[r + 1][c] != 9)
		size += calc_basin_size_impl(heightmap, r + 1, c);
	if (c > 0 && heightmap[r][c - 1] != 9)
		size += calc_basin_size_impl(heightmap, r, c - 1);
	if (c < heightmap[r].size() - 1 && heightmap[r][c + 1] != 9)
		size += calc_basin_size_impl(heightmap, r, c + 1);
	return size;
}

static uint calc_basin_size(Heightmap heightmap, std::size_t r, std::size_t c) {
	return calc_basin_size_impl(heightmap, r, c);
}

static uint basin_sizes(const Heightmap &heightmap) {
	const auto low_points = find_low_points(heightmap);
	std::vector<uint> sizes;
	sizes.reserve(low_points.size());
	std::transform(low_points.begin(), low_points.end(), std::back_inserter(sizes),
			[&heightmap](const auto &pair) { return calc_basin_size(heightmap, pair.first, pair.second); });

	std::sort(sizes.begin(), sizes.end());
	return std::reduce(sizes.end() - 3, sizes.end(), 1u, std::multiplies{});
}

int main(int argc, char *argv[]) {
	const auto heightmap = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << risk_level(heightmap) << std::endl;
		break;
	case 2:
		std::cout << basin_sizes(heightmap) << std::endl;
		break;
	}
	return 0;
}
