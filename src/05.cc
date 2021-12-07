#include "common.h"
#include <array>
#include <numeric>

using Grid = std::vector<std::vector<std::size_t>>;

class Line {
	uint m_x1, m_y1, m_x2, m_y2;
	int m_x_inc, m_y_inc;
public:

	Line(const std::string &line) {
		std::stringstream ss{line};
		std::string val_str;
		std::getline(ss, val_str, ',');
		std::stringstream{val_str} >> m_x1;
		std::getline(ss, val_str, ' ');
		std::stringstream{val_str} >> m_y1;
		std::getline(ss, val_str, ' ');
		std::getline(ss, val_str, ',');
		std::stringstream{val_str} >> m_x2;
		std::getline(ss, val_str, ' ');
		std::stringstream{val_str} >> m_y2;

		m_x_inc = m_x1 == m_x2 ? 0 : (m_x1 < m_x2 ? 1 : -1);
		m_y_inc = m_y1 == m_y2 ? 0 : (m_y1 < m_y2 ? 1 : -1);
	}

	bool diagonal() const noexcept {
		return m_x1 != m_x2 && m_y1 != m_y2;
	}

	void trace(Grid &grid) const noexcept {
		for (auto x = m_x1, y = m_y1; x != m_x2 || y != m_y2; x += m_x_inc, y += m_y_inc)
			++grid[y][x];
		++grid[m_y2][m_x2];
	}

};

template<class Trace_Func>
static std::size_t count_overlaps(const std::vector<Line> &lines, const Trace_Func &trace) {
	Grid grid(1000, std::vector<std::size_t>(1000, 0));
	for (const auto &line : lines)
		trace(grid, line);
	return std::accumulate(grid.begin(), grid.end(), std::size_t{0}, [](auto sum, const auto &row) {
		return sum + std::accumulate(row.begin(), row.end(), std::size_t{0}, [](auto sum, auto count) { return sum + (count >= 2); });
	});
}

int main(int argc, char *argv[]) {
	const auto lines = read_input_objs<Line>();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << count_overlaps(lines, [](auto &grid, const auto &line) { if (!line.diagonal()) line.trace(grid); }) << std::endl;
		break;
	case 2:
		std::cout << count_overlaps(lines, [](auto &grid, const auto &line) { line.trace(grid); }) << std::endl;
		break;
	}
	return 0;
}
