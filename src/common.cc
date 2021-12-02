#include "common.h"
#include <iostream>
#include <cstring>

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

std::vector<uint> read_input_uints() {
	std::vector<uint> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(std::stoul(line));
	return input;
}
