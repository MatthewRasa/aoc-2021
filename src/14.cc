#include "common.h"
#include <algorithm>
#include <numeric>

using Elem = std::array<char, 2>;

template<>
struct std::hash<Elem> {
	std::size_t operator()(const Elem &elem) const noexcept {
		return (1ul << (8 * sizeof(elem[0]))) * elem[0] + elem[1];
	}
};

using Poly = std::unordered_map<Elem, std::uint64_t>;

class Rule {
	Elem m_input;
	std::array<Elem, 2> m_outputs;
public:

	Rule(const std::string &rule_str) noexcept
			: m_input{rule_str[0], rule_str[1]},
			  m_outputs{Elem{m_input[0], rule_str[6]}, Elem{rule_str[6], m_input[1]}} { }

	const Elem &input() const noexcept {
		return m_input;
	}

	const std::array<Elem, 2> &outputs() const noexcept {
		return m_outputs;
	}

};

static std::pair<Poly, char> read_input_poly() {
	const auto poly_str = read_input_string();
	const auto last_char = poly_str.back();
	Poly poly;
	poly.reserve(poly_str.size() - 1);
	for (std::size_t i = 0; i < poly_str.size() - 1; ++i)
		poly.emplace(Elem{poly_str[i], poly_str[i + 1]}, 1);
	return std::make_pair(std::move(poly), last_char);
}

static Poly apply_step(Poly poly, const std::vector<Rule> &rules) {
	std::vector<std::pair<Rule, std::uint64_t>> changes;
	changes.reserve(rules.size());
	for (const auto &rule : rules) {
		const auto it = poly.find(rule.input());
		if (it != poly.end())
			changes.emplace_back(rule, it->second);
	}

	for (const auto &[change, occurs] : changes) {
		poly[change.input()] -= occurs;
		for (const auto &output : change.outputs())
			poly[output] += occurs;
	}
	return poly;
}

static Poly apply_steps(Poly poly, const std::vector<Rule> &rules, uint nsteps) {
	while (nsteps-- > 0)
		poly = apply_step(std::move(poly), rules);
	return poly;
}

static std::uint64_t poly_score(const Poly &poly, char last_char) {
	std::uint64_t min_quant{-1ul}, max_quant{0};
	for (char c = 'A'; c <= 'Z'; ++c) {
		auto count = std::transform_reduce(poly.cbegin(), poly.cend(), 0ul, std::plus{},
				[c](const auto &pair) { return pair.first[0] == c ? pair.second : 0ul; });
		if (c == last_char)
			++count;
		if (count != 0) {
			min_quant = std::min(min_quant, count);
			max_quant = std::max(max_quant, count);
		}
	}
	return max_quant - min_quant;
}

int main(int argc, char *argv[]) {
	auto [poly, last_char] = read_input_poly();
	skip_input_line();
	std::cout << poly_score(apply_steps(std::move(poly), read_input_objs<Rule>(), select_part(argc, argv) == 1 ? 10 : 40), last_char) << std::endl;
	return 0;
}
