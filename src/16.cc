#include "common.h"
#include <algorithm>
#include <bitset>
#include <limits>
#include <numeric>

using Transmission = std::bitset<5440>;

static std::uint8_t parse_hex(char hex) {
	return '0' <= hex && hex <= '9' ? hex - '0' : 10 + (hex - 'A');
}

static const Transmission &read_transmission() {
	static Transmission transmission;
	const auto hex_str = read_input_string();
	for (std::size_t pos = 0; pos < hex_str.size(); ++pos) {
		std::bitset<4> bin(parse_hex(hex_str[pos]));
		for (std::size_t i = 0; i < 4; ++i)
			transmission[4 * (pos + 1) - 1 - i] = bin[i];
	}
	return transmission;
}

template<std::size_t n>
static std::uint64_t read_bits(const Transmission &transmission, std::size_t &pos) {
	std::bitset<n> num;
	for (std::int64_t i = n - 1; i >= 0; --i)
		num[i] = transmission[pos++];
	return num.to_ullong();
}

static std::uint64_t read_literal(const Transmission &transmission, std::size_t &pos) {
	std::uint64_t lit{0};
	bool more_groups;
	do {
		more_groups = read_bits<1>(transmission, pos);
		lit = (lit << 4) | read_bits<4>(transmission, pos);
	} while (more_groups);
	return lit;
}

template<class Func>
std::vector<std::uint64_t> read_subpackets(const Transmission &transmission, std::size_t &pos, const Func &func) {
	std::vector<std::uint64_t> sub_packets;
	if (read_bits<1>(transmission, pos) == 0) {
		const auto sub_length = read_bits<15>(transmission, pos);
		const auto end_pos = pos + sub_length;
		while (pos != end_pos)
			sub_packets.push_back(func(transmission, pos));
	} else {
		const auto sub_count = read_bits<11>(transmission, pos);
		sub_packets.reserve(sub_count);
		for (std::size_t i = 0; i < sub_count; ++i)
			sub_packets.push_back(func(transmission, pos));
	}
	return sub_packets;
}

static std::uint64_t sum_version_nums(const Transmission &transmission, std::size_t &pos) {
	auto vsum = read_bits<3>(transmission, pos);
	switch (read_bits<3>(transmission, pos)) {
	case 4:
		read_literal(transmission, pos);
		break;
	default:
		const auto sub_packets = read_subpackets(transmission, pos, [](const auto &t, auto &p) { return sum_version_nums(t, p); });
		vsum += std::accumulate(sub_packets.begin(), sub_packets.end(), 0ul, std::plus{});
		break;
	}
	return vsum;
}

static std::uint64_t sum_version_nums(const Transmission &transmission) {
	std::size_t pos{0};
	return sum_version_nums(transmission, pos);
}

static std::uint64_t evaluate_transmission(const Transmission &transmission, std::size_t &pos) {
	read_bits<3>(transmission, pos);
	const auto type_id = read_bits<3>(transmission, pos);
	switch (type_id) {
	case 4:
		return read_literal(transmission, pos);
	default:
		const auto sub_packets = read_subpackets(transmission, pos, [](const auto &t, auto &p) { return evaluate_transmission(t, p); });
		switch (type_id) {
		case 0:
			return std::accumulate(sub_packets.begin(), sub_packets.end(), 0ul, std::plus{});
		case 1:
			return std::accumulate(sub_packets.begin(), sub_packets.end(), 1ul, std::multiplies{});
		case 2:
			return *std::min_element(sub_packets.begin(), sub_packets.end());
		case 3:
			return *std::max_element(sub_packets.begin(), sub_packets.end());
		case 5:
			return sub_packets[0] > sub_packets[1];
		case 6:
			return sub_packets[0] < sub_packets[1];
		case 7:
		default:
			return sub_packets[0] == sub_packets[1];
		}
	}
}

static std::uint64_t evaluate_transmission(const Transmission &transmission) {
	std::size_t pos{0};
	return evaluate_transmission(transmission, pos);
}

int main(int argc, char *argv[]) {
	const auto &transmission = read_transmission();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << sum_version_nums(transmission) << std::endl;
		break;
	case 2:
		std::cout << evaluate_transmission(transmission) << std::endl;
		break;
	}
	return 0;
}
