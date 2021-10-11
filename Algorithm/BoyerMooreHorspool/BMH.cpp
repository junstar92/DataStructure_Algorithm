#include "BMH.hpp"

#include <algorithm>

std::vector<int> createRtOccurrencesTable(std::string pattern)
{
	std::vector<int> table(TABLE_SIZE);

	const size_t m = pattern.length();

	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i] = m;
	}

	for (int i = m - 2; i >= 0; i--) {
		if (table[pattern[i]] == m) {
			table[pattern[i]] = m - i - 1;
		}
	}

	return table;
}

std::vector<int> boyerMooreHorspool(std::string pattern, std::string text)
{
	// to lower
	std::transform(pattern.begin(), pattern.end(), pattern.begin(), tolower);
	std::transform(text.begin(), text.end(), text.begin(), tolower);

	std::vector<int> ret;

	const size_t m = pattern.length();
	const size_t n = text.length();
	
	std::vector<int> rt = createRtOccurrencesTable(pattern);

	int i = 0;
	while (i <= n - m) {
		int j = m - 1;

		while (j >= 0 && text[i + j] == pattern[j]) {
			j -= 1;

			if (j < 0)
				ret.push_back(i);
		}

		i += rt[text[i + m - 1]];
	}

	return ret;
}

std::map<char, int> createRtOccurrencesTableUsingHash(std::string pattern)
{
	std::map<char, int> table;

	const size_t m = pattern.length();

	for (int i = m - 2; i >= 0; i--) {
		if (table.find(pattern[i]) == table.end()) {
			table[pattern[i]] = m - i - 1;
		}
	}

	return table;
}

std::vector<int> boyerMooreHorspoolUsingHash(std::string pattern, std::string text)
{
	// to lower
	std::transform(pattern.begin(), pattern.end(), pattern.begin(), tolower);
	std::transform(text.begin(), text.end(), text.begin(), tolower);

	std::vector<int> ret;

	const size_t m = pattern.length();
	const size_t n = text.length();

	std::map<char, int> rt = createRtOccurrencesTableUsingHash(pattern);

	int i = 0;
	while (i <= n - m) {
		int j = m - 1;

		while (j >= 0 && text[i + j] == pattern[j]) {
			j -= 1;

			if (j < 0)
				ret.push_back(i);
		}

		if (rt.find(text[i + m - 1]) == rt.end()) {
			i += m;
		}
		else {
			i += rt[text[i + m - 1]];
		}
	}

	return ret;
}