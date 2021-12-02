#ifndef COMMON_H_
#define COMMON_H_

#include <stdexcept>
#include <tuple>
#include <unordered_set>

uint select_part(int argc, char *argv[]);

template<class T, class Iter>
std::pair<T, T> two_sum(Iter begin, Iter end, T total) {
	std::unordered_set<T> diffs;
	for (auto it = begin; it != end; ++it) {
		const auto diff = total - *it;
		if (diffs.find(*it) != diffs.end())
			return std::make_pair(diff, *it);
		diffs.insert(diff);
	}
	throw std::logic_error{"no match found"};
}

#endif /* COMMON_H_ */
