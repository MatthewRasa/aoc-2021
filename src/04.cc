#include "common.h"
#include <functional>
#include <iomanip>
#include <list>
#include <numeric>
#include <unordered_map>

static constexpr std::size_t LENGTH{5};

class Board {
	std::vector<std::vector<std::uint8_t>> m_rows;
	std::unordered_map<uint, std::pair<std::size_t, std::size_t>> m_mapping;
public:

	Board(const std::vector<std::string> &rows)
			 : m_rows(LENGTH, std::vector<std::uint8_t>(LENGTH, false)) {
		m_mapping.reserve(LENGTH * LENGTH);
		for (std::size_t r = 0; r < LENGTH; ++r) {
			const auto nums = parse_input_line_space_delim<uint>(rows[r]);
			for (std::size_t c = 0; c < LENGTH; ++c)
				m_mapping.emplace(nums[c], std::make_pair(r, c));
		}
	}

	bool check_win() const {
		for (const auto &row : m_rows) {
			if (std::all_of(row.begin(), row.end(), [](auto val) { return val; }))
				return true;
		}
		for (std::size_t c = 0; c < LENGTH; ++c) {
			if (std::all_of(m_rows.begin(), m_rows.end(), [c](const auto &row) { return row[c]; }))
				return true;
		}
		return false;
	}

	uint calc_score() const {
		return std::accumulate(m_mapping.begin(), m_mapping.end(), 0u, [this](auto sum, const auto &pair) {
			auto [row, col] = pair.second;
			return sum + (!m_rows[row][col] ? pair.first : 0u);
		});
	}

	void mark_number(uint called) {
		auto it = m_mapping.find(called);
		if (it != m_mapping.end()) {
			auto [row, col] = it->second;
			m_rows[row][col] = true;
		}
	}

};

static std::vector<Board> read_boards() {
	std::vector<Board> boards;
	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<std::string> rows;
		rows.reserve(5);
		for (std::size_t i = 0; i < LENGTH; ++i) {
			std::getline(std::cin, line);
			rows.push_back(std::move(line));
		}
		boards.emplace_back(std::move(rows));
	}
	return boards;
}

static uint winning_board_score(const std::vector<uint> &nums, std::vector<Board> boards) {
	for (auto num : nums) {
		for (auto &board : boards) {
			board.mark_number(num);
			if (board.check_win())
				return num * board.calc_score();
		}
	}
	throw std::runtime_error{"no winner"};
}

static uint losing_board_score(const std::vector<uint> &nums, const std::vector<Board> &board_vector) {
	std::list<Board> boards{board_vector.begin(), board_vector.end()};
	for (auto num : nums) {
		for (auto it = boards.begin(); it != boards.end(); ) {
			it->mark_number(num);
			if (it->check_win()) {
				if (boards.size() == 1)
					return num * it->calc_score();
				it = boards.erase(it);
			} else {
				++it;
			}
		}
	}
	throw std::runtime_error{"no loser"};
}

int main(int argc, char *argv[]) {
	const auto nums = read_input_line_comma_delim<uint>();
	auto boards = read_boards();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << winning_board_score(nums, std::move(boards)) << std::endl;
		break;
	case 2:
		std::cout << losing_board_score(nums, std::move(boards)) << std::endl;
		break;
	}
	return 0;
}
