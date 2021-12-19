#include "common.h"
#include <cmath>

class Target_Area {
	int m_x1, m_y1, m_x2, m_y2;
public:

	Target_Area(const std::string line) {
		auto x1_start = line.find('=') + 1;
		auto x2_start = line.find('.', x1_start) + 2;
		auto x2_end = line.find(',', x2_start);
		m_x1 = std::stol(line.substr(x1_start, x2_start - x1_start));
		m_x2 = std::stol(line.substr(x2_start, x2_end - x2_start));

		auto y1_start = line.find('=', x2_end) + 1;
		auto y2_start = line.find('.', y1_start) + 2;
		m_y1 = std::stol(line.substr(y1_start, y2_start - y1_start));
		m_y2 = std::stol(line.substr(y2_start));
	}

	bool in_area(int x, int y) const noexcept {
		return m_x1 <= x && x <= m_x2 && m_y1 <= y && y <= m_y2;
	}

	bool in_bounds(int x, int y) const noexcept {
		return x <= m_x2 && y >= m_y1;
	}

	int max_xv() const noexcept {
		return m_x2;
	}

	int max_yv() const noexcept {
		return -m_y1 - 1;
	}

	int min_yv() const noexcept {
		return m_y1;
	}

};

static bool hits_target(int xv, int yv, const Target_Area &target) {
	for (int x = 0, y = 0; target.in_bounds(x, y); ) {
		if (target.in_area(x, y))
			return true;
		x += xv;
		y += yv;
		if (xv != 0)
			xv = std::copysign(std::abs(xv) - 1, xv);
		--yv;
	}
	return false;
}

static int max_height(int yv) {
	int y;
	for (y = 0; yv > 0; --yv)
		y += yv;
	return y;
}

static int highest_height(const Target_Area &target) {
	for (int yv = target.max_yv(); yv >= target.min_yv(); --yv) {
		for (int xv = 0; xv <= target.max_xv(); ++xv) {
			if (hits_target(xv, yv, target))
				return max_height(yv);
		}
	}
	throw std::runtime_error{"target never hit"};
}

static int possible_init_velocities(const Target_Area &target) {
	int count{0};
	for (int yv = target.max_yv(); yv >= target.min_yv(); --yv) {
		for (int xv = 0; xv <= target.max_xv(); ++xv) {
			if (hits_target(xv, yv, target))
				++count;
		}
	}
	return count;
}

int main(int argc, char *argv[]) {
	const auto target = Target_Area{read_input_string()};
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << highest_height(target) << std::endl;
		break;
	case 2:
		std::cout << possible_init_velocities(target) << std::endl;
		break;
	}
	return 0;
}
