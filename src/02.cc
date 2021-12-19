#include "common.h"
#include <sstream>

class Command {
public:

	enum class Type { FORWARD, DOWN, UP };

	Command(const std::string &line) {
		std::stringstream ss{line};
		std::string type_str, val_str;
		ss >> type_str >> val_str;
		m_type = parse_type(type_str);
		m_val = std::stoul(val_str);
	}

	Type type() const noexcept { return m_type; }
	int val() const noexcept { return m_val; }

private:

	static Type parse_type(const std::string &type_str) {
		if (type_str == "forward")
			return Type::FORWARD;
		if (type_str == "down")
			return Type::DOWN;
		if (type_str == "up")
			return Type::UP;
		throw std::runtime_error{std::string{"unexpected type: "} + type_str};
	}

	Type m_type;
	int m_val;
};

static std::pair<int, int> final_position1(const std::vector<Command> &commands) {
	int pos{0}, depth{0};
	for (const auto &command : commands) {
		switch (command.type()) {
		case Command::Type::FORWARD:
			pos += command.val();
			break;
		case Command::Type::DOWN:
			depth += command.val();
			break;
		case Command::Type::UP:
			depth -= command.val();
			break;
		}
	}
	return std::make_pair(pos, depth);
}

static std::pair<int, int> final_position2(const std::vector<Command> &commands) {
	int pos{0}, depth{0}, aim{0};
	for (const auto &command : commands) {
		switch (command.type()) {
		case Command::Type::FORWARD:
			pos += command.val();
			depth += aim * command.val();
			break;
		case Command::Type::DOWN:
			aim += command.val();
			break;
		case Command::Type::UP:
			aim -= command.val();
			break;
		}
	}
	return std::make_pair(pos, depth);
}

int main(int argc, char *argv[]) {
	const auto commands = read_input_objs<Command>();
	int pos, depth;
	switch (select_part(argc, argv)) {
	case 1:
		std::tie(pos, depth) = final_position1(commands);
		break;
	case 2:
	default:
		std::tie(pos, depth) = final_position2(commands);
		break;
	}
	std::cout << pos * depth << std::endl;
	return 0;
}
