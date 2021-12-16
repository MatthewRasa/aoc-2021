#include "common.h"
#include <cstring>
#include <iostream>
#include <limits>

uint select_part(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <1|2>" << std::endl;
		std::exit(1);
	}

	if (strncmp(argv[1], "1", 1) == 0) {
		return 1;
	} else if (strncmp(argv[1], "2", 1) == 0) {
		return 2;
	} else {
		std::cerr << argv[0] << ": invalid argument '" << argv[1] << "'" << std::endl;
		std::exit(1);
	}
}

extern std::string read_input_string() {
	std::string line;
	std::getline(std::cin, line);
	return line;
}

std::vector<uint> read_input_uints() {
	return read_input<uint>([](const auto &line) { return std::stoul(line); });
}

void skip_input_line() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
