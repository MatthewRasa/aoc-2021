#include "common.h"
#include <algorithm>
#include <deque>
#include <limits>

using Cavern = std::vector<std::vector<std::uint64_t>>;
using Neighbor = std::pair<std::size_t, std::size_t>;

static constexpr std::uint64_t INF{std::numeric_limits<std::uint64_t>::max()};

static std::uint64_t lowest_neighbor_cost(Cavern &costs, const std::vector<Neighbor> &neighbors) {
	std::vector<std::uint64_t> neighbor_costs(neighbors.size());
	std::transform(neighbors.begin(), neighbors.end(), neighbor_costs.begin(),
			[&costs](const auto &n) { return costs[n.first][n.second]; });
	return *std::min_element(neighbor_costs.begin(), neighbor_costs.end());
}

static std::uint64_t lowest_risk_path(const Cavern &cavern) {
	Cavern costs(cavern.size(), std::vector<std::uint64_t>(cavern.size(), INF));
	costs[costs.size() - 1][costs.size() - 1] = cavern[cavern.size() - 1][cavern.size() - 1];

	std::deque<Neighbor> neighbors{
		{cavern.size() - 2, cavern.size() - 1},
		{cavern.size() - 1, cavern.size() - 2},
	};
	for (; !neighbors.empty(); neighbors.pop_front()) {
		const auto &[r, c] = neighbors.front();
		std::vector<Neighbor> local_neighbors;
		local_neighbors.reserve(4);
		if (r > 0)
			local_neighbors.emplace_back(r - 1, c);
		if (c > 0)
			local_neighbors.emplace_back(r, c - 1);
		if (r < costs.size() - 1)
			local_neighbors.emplace_back(r + 1, c);
		if (c < costs[0].size() - 1)
			local_neighbors.emplace_back(r, c + 1);

		const auto min_cost = lowest_neighbor_cost(costs, local_neighbors);
		if (cavern[r][c] + min_cost < costs[r][c]) {
			costs[r][c] = cavern[r][c] + min_cost;
			std::move(local_neighbors.begin(), local_neighbors.end(), std::back_inserter(neighbors));
		}
	}

	return costs[0][0] - cavern[0][0];
}

static void inc_subcavern_risk(Cavern &cavern, std::size_t r_start, std::size_t c_start, std::size_t subsize, std::uint64_t risk_inc) {
	for (std::size_t r = 0; r < subsize; ++r) {
		for (std::size_t c = 0; c < subsize; ++c) {
			cavern[r_start + r][c_start + c] = cavern[r][c] + risk_inc;
			while (cavern[r_start + r][c_start + c] > 9)
				cavern[r_start + r][c_start + c] -= 9;
		}
	}
}

static Cavern expand_cavern(Cavern cavern) {
	const auto old_size = cavern.size();
	const auto new_size = old_size * 5;
	cavern.resize(new_size);
	for (auto &row : cavern)
		row.resize(new_size);

	for (std::size_t r = 0; r < cavern.size(); r += old_size)
		for (std::size_t c = 0, risk_add = r; c < cavern[r].size(); c += old_size, ++risk_add)
			inc_subcavern_risk(cavern, r, c, old_size, risk_add);

	return cavern;
}

int main(int argc, char *argv[]) {
	auto cavern = read_input<std::vector<std::uint64_t>>([](const auto &line) { return parse_input_line_no_delim<std::uint64_t>(line); });
	if (select_part(argc, argv) == 2)
		cavern = expand_cavern(std::move(cavern));
	std::cout << lowest_risk_path(cavern) << std::endl;
	return 0;
}
