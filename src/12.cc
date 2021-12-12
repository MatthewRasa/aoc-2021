#include "common.h"
#include <algorithm>
#include <execution>
#include <numeric>
#include <unordered_map>

class Cave {
	std::string m_name;
	bool m_small;
	std::vector<const Cave *> m_paths;
public:

	Cave(std::string name)
			: m_name(std::move(name)),
			  m_small{std::all_of(m_name.begin(), m_name.end(), [](auto c) { return c == std::tolower(c); })} { }

	const std::string &name() const noexcept {
		return m_name;
	}

	bool is_start() const noexcept {
		return m_name == "start";
	}

	bool is_end() const noexcept {
		return m_name == "end";
	}

	bool is_small() const noexcept {
		return m_small;
	}

	const std::vector<const Cave *> &paths() const noexcept {
		return m_paths;
	}

	void add_path(const Cave &path) {
		m_paths.push_back(&path);
	}

};

static std::unordered_map<std::string, Cave> read_input_caves() {
	std::unordered_map<std::string, Cave> caves;
	for (std::string line; std::getline(std::cin, line); ) {
		const auto delim_pos = line.find('-');
		const auto cave1_name = line.substr(0, delim_pos);
		const auto cave2_name = line.substr(delim_pos + 1);
		auto &cave1 = caves.try_emplace(cave1_name, cave1_name).first->second;
		auto &cave2 = caves.try_emplace(cave2_name, cave2_name).first->second;
		cave1.add_path(cave2);
		cave2.add_path(cave1);
	}
	return caves;
}

static uint count_paths1(const Cave &cave, std::unordered_set<std::string> visited = {}) {
	if (cave.is_end())
		return 1u;
	if (cave.is_small() && !visited.emplace(cave.name()).second)
		return 0u;
	return std::transform_reduce(std::execution::par_unseq, cave.paths().begin(), cave.paths().end(), 0u, std::plus{},
			[&visited](const auto &p_path) { return count_paths1(*p_path, visited); });
}

static uint count_paths2(const Cave &cave, std::unordered_set<std::string> visited = {}, std::string double_visit = "") {
	if (cave.is_end())
		return 1u;
	if (cave.is_small() && !visited.emplace(cave.name()).second) {
		if (cave.is_start() || !double_visit.empty())
			return 0u;
		double_visit = cave.name();
	}
	return std::transform_reduce(std::execution::par_unseq, cave.paths().begin(), cave.paths().end(), 0u, std::plus{},
			[&visited, &double_visit](const auto &p_path) { return count_paths2(*p_path, visited, double_visit); });
}

int main(int argc, char *argv[]) {
	const auto caves = read_input_caves();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << count_paths1(caves.at("start")) << std::endl;
		break;
	case 2:
		std::cout << count_paths2(caves.at("start")) << std::endl;
		break;
	}
	return 0;
}
