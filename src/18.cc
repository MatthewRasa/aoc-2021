#include "common.h"
#include <memory>
#include <numeric>

class Snailfish_Num {
public:

	using Pair = std::pair<Snailfish_Num, Snailfish_Num>;

	Snailfish_Num(const std::string &line, std::size_t pos = 0, Snailfish_Num *p_parent = nullptr)
			: mp_parent{p_parent} {
		if (line[pos] != '[')
			m_val = line[pos] - '0';
		else
			mp_pair = std::make_unique<Pair>(Snailfish_Num{line, pos + 1, this},
					Snailfish_Num{line, find_comma_pos(line, pos + 1) + 1, this});
	}

	Snailfish_Num(int val, Snailfish_Num *p_parent) noexcept
			: m_val{val},
			  mp_parent{p_parent} { }

	Snailfish_Num(std::unique_ptr<Pair> p_pair, Snailfish_Num *p_parent) noexcept
			: mp_pair{std::move(p_pair)},
			  mp_parent{p_parent} { }

	Snailfish_Num(const Snailfish_Num &other)
			: m_val{other.m_val},
			  mp_pair{other.mp_pair != nullptr ? std::make_unique<Pair>(*other.mp_pair) : nullptr},
			  mp_parent{other.mp_parent} {
		if (mp_pair)
			reset_up_links();
	}

	Snailfish_Num(Snailfish_Num &&other)
			: m_val{std::move(other.m_val)},
			  mp_pair{std::move(other.mp_pair)},
			  mp_parent{std::move(other.mp_parent)} {
		if (mp_pair)
			reset_up_links();
	}

	Snailfish_Num &operator=(const Snailfish_Num &other) {
		m_val = other.m_val;
		mp_pair = other.mp_pair != nullptr ? std::make_unique<Pair>(*other.mp_pair) : nullptr;
		mp_parent = other.mp_parent;
		if (mp_pair)
			reset_up_links();
		return *this;
	}

	Snailfish_Num &operator=(Snailfish_Num &&other) {
		m_val = std::move(other.m_val);
		mp_pair = std::move(other.mp_pair);
		mp_parent = std::move(other.mp_parent);
		if (mp_pair)
			reset_up_links();
		return *this;
	}

	Snailfish_Num operator+(Snailfish_Num other) const {
		return Snailfish_Num{*this} += std::move(other);
	}

	Snailfish_Num &operator+=(Snailfish_Num other) {
		const auto p_parent = mp_parent;
		mp_pair = std::make_unique<Pair>(std::move(*this), std::move(other));
		mp_parent = p_parent;
		reset_up_links();
		reduce();
		return *this;
	}

	int magnitude() const noexcept {
		return is_scalar() ? m_val : 3 * std::get<0>(*mp_pair).magnitude() + 2 * std::get<1>(*mp_pair).magnitude();
	}

	std::string to_string() const {
		return is_scalar() ? std::to_string(m_val) : "[" + mp_pair->first.to_string() + "," + mp_pair->second.to_string() + "]";
	}

private:

	static std::size_t find_comma_pos(const std::string &line, std::size_t start) {
		int level{0};
		for (std::size_t i = start; i < line.size(); ++i) {
			switch (line[i]) {
			case '[':
				++level;
				break;
			case ']':
				--level;
				break;
			case ',':
				if (level == 0)
					return i;
			}
		}
		throw std::runtime_error{"error parsing pair"};
	}

	int m_val;
	std::unique_ptr<Pair> mp_pair;
	Snailfish_Num *mp_parent;

	bool is_scalar() const noexcept {
		return mp_pair == nullptr;
	}

	void reset_up_links() noexcept {
		mp_pair->first.mp_parent = mp_pair->second.mp_parent = this;
	}

	void reduce() {
		while (check_explode() || check_split());
	}

	bool check_explode(int level = 0) {
		if (is_scalar())
			return false;
		if (level < 4)
			return std::get<0>(*mp_pair).check_explode(level + 1) || std::get<1>(*mp_pair).check_explode(level + 1);
		auto p_left_neighbor = left_neighbor();
		if (p_left_neighbor)
			p_left_neighbor->m_val += mp_pair->first.m_val;
		auto p_right_neighbor = right_neighbor();
		if (p_right_neighbor)
			p_right_neighbor->m_val += mp_pair->second.m_val;
		mp_pair.reset();
		m_val = 0;
		return true;
	}

	bool check_split() {
		if (is_scalar()) {
			if (m_val >= 10) {
				mp_pair = std::make_unique<Pair>(Snailfish_Num{m_val / 2, this}, Snailfish_Num{(m_val + 1) / 2, this});
				return true;
			}
			return false;
		}
		return std::get<0>(*mp_pair).check_split() || std::get<1>(*mp_pair).check_split();
	}

	Snailfish_Num *left_neighbor() {
		auto p_ancestor = mp_parent->left_ancestor(this);
		if (p_ancestor) {
			while (!p_ancestor->is_scalar())
				p_ancestor = &p_ancestor->mp_pair->second;
		}
		return p_ancestor;
	}

	Snailfish_Num *right_neighbor() {
		auto p_ancestor = mp_parent->right_ancestor(this);
		if (p_ancestor) {
			while (!p_ancestor->is_scalar())
				p_ancestor = &p_ancestor->mp_pair->first;
		}
		return p_ancestor;
	}

	Snailfish_Num *left_ancestor(const Snailfish_Num *p_child) {
		return &mp_pair->first != p_child ? &mp_pair->first
				: mp_parent != nullptr ? mp_parent->left_ancestor(this)
				: nullptr;
	}

	Snailfish_Num *right_ancestor(const Snailfish_Num *p_child) {
		return &mp_pair->second != p_child ? &mp_pair->second
				: mp_parent != nullptr ? mp_parent->right_ancestor(this)
				: nullptr;
	}

};

static int largest_magnitude(const std::vector<Snailfish_Num> &nums) {
	int max_magnitude{0};
	for (const auto &num1 : nums) {
		for (const auto &num2 : nums) {
			if (&num1 != &num2)
				max_magnitude = std::max(max_magnitude, (num1 + num2).magnitude());
		}
	}
	return max_magnitude;
}

int main(int argc, char *argv[]) {
	auto nums = read_input_objs<Snailfish_Num>();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << std::accumulate(std::next(nums.begin()), nums.end(), std::move(nums.front())).magnitude() << std::endl;
		break;
	case 2:
		std::cout << largest_magnitude(nums) << std::endl;
		break;
	}
	return 0;
}
