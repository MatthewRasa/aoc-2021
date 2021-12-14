#include "common.h"
#include <algorithm>
#include <numeric>

enum class Fold_Direction { LEFT, UP };

using Paper = std::vector<std::vector<std::uint8_t>>;
using Fold = std::pair<Fold_Direction, uint>;

static std::tuple<std::vector<std::pair<uint, uint>>, uint, uint> read_coords() {
	std::vector<std::pair<uint, uint>> coords;
	uint max_x{0}, max_y{0};
	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		const auto coord = parse_input_line_comma_delim<uint>(line);
		coords.push_back(std::make_pair(coord[0], coord[1]));
		max_x = std::max(max_x, coord[0]);
		max_y = std::max(max_y, coord[1]);
	}
	return std::make_tuple(coords, max_x, max_y);
}

static Paper read_input_paper() {
	const auto [coords, max_x, max_y] = read_coords();
	Paper paper(max_y + 1, std::vector<std::uint8_t>(max_x + 1, false));
	for (const auto &[x, y] : coords)
		paper[y][x] = true;
	return paper;
}

static std::vector<Fold> read_folds() {
	std::vector<Fold> folds;
	for (std::string line; std::getline(std::cin, line); ) {
		const auto eq_pos = line.find('=');
		folds.emplace_back(line.substr(eq_pos - 1, 1) == "x" ? Fold_Direction::LEFT : Fold_Direction::UP, std::stoul(line.substr(eq_pos + 1)));
	}
	return folds;
}

static Paper fold_paper(Paper paper, Fold_Direction direction, uint fold_line) {
	switch (direction) {
	case Fold_Direction::LEFT:
		for (uint line = fold_line + 1; line < paper[0].size(); ++line)
			for (std::size_t row = 0; row < paper.size(); ++row)
				paper[row][2 * fold_line - line] = paper[row][2 * fold_line - line] || paper[row][line];
		for (auto &row : paper)
			row.resize(fold_line);
		break;
	case Fold_Direction::UP:
		for (uint line = fold_line + 1; line < paper.size(); ++line)
			for (std::size_t col = 0; col < paper[line].size(); ++col)
				paper[2 * fold_line - line][col] = paper[2 * fold_line - line][col] || paper[line][col];
		paper.resize(fold_line);
		break;
	}
	return paper;
}

static uint count_dots(const Paper &paper) {
	return std::transform_reduce(paper.begin(), paper.end(), 0u, std::plus{},
			[](const auto &row) { return std::reduce(row.begin(), row.end()); });
}

static void print_paper(const Paper &paper) {
	for (const auto &row : paper) {
		for (auto dot : row)
			std::cout << (dot ? '#' : '.');
		std::cout << std::endl;
	}
}

int main(int argc, char *argv[]) {
	auto paper = read_input_paper();
	const auto folds = read_folds();
	switch (select_part(argc, argv)) {
	case 1:
		paper = fold_paper(std::move(paper), folds[0].first, folds[0].second);
		std::cout << count_dots(paper) << std::endl;
		break;
	case 2:
		for (const auto &[direction, fold_line] : folds)
			paper = fold_paper(std::move(paper), direction, fold_line);
		print_paper(paper);
		break;
	}
	return 0;
}
