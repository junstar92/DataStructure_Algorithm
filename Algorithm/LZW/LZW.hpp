#ifndef __LZW_HPP__
#define __LZW_HPP__

#include <string>
#include <list>
#include <unordered_map>
#include <iostream>
#include <type_traits>

template <size_t _Bits>
class LZW {
	using _Ty = std::conditional_t<_Bits <= sizeof(unsigned long) * CHAR_BIT, 
		std::conditional_t<_Bits <= sizeof(unsigned char), unsigned char, unsigned long>, unsigned long long>;

private:
	size_t bits;
	_Ty max_code;
	void print(std::list<_Ty> compressed) {
		for (auto& val : compressed) {
			std::cout << val << " ";
		}
		std::cout << "\n";
	}

public:
	LZW() {
		bits = _Bits;
		if (bits > sizeof(unsigned long long) * CHAR_BIT)
		{
			bits = sizeof(unsigned long long) * CHAR_BIT;
			std::cout << "bit size is changed to " << bits << "\n";
		}
		max_code = (_Ty{ 1 } << bits) - 1;
	}

	std::list<_Ty> compress(std::string input) {
		std::cout << "input string : " << input << "\n";
		std::unordered_map<std::string, _Ty> table;
		_Ty code = _Ty{ 128 };

		for (char i = 0; i < code; i++) {
			std::string tmp(1, i);
			table[tmp] = i;
		}

		std::list<_Ty> compressed;
		std::string w = "";

		for (auto& c : input) {
			std::string wc = w + c;

			if (table.find(wc) != table.end()) {
				w = wc;
			}
			else {
				compressed.push_back(table[w]);
				w = c;

				if (code > max_code) {
					std::cout << "max_code overflow!\n";
					compressed.clear();
					return compressed;
				}

				table[wc] = code;
				code += 1;
			}
		}

		if (w != "") {
			compressed.push_back(table[w]);
		}

		std::cout << "Compression Fishished : ";
		print(compressed);

		return compressed;
	}

	std::string decompress(std::list<_Ty> compressed) {
		std::unordered_map<_Ty, std::string> table;
		_Ty code = 128;

		for (char i = 0; i < code; i++) {
			std::string tmp(1, i);
			table[i] = tmp;
		}

		std::string v = table[compressed.front()];
		compressed.pop_front();

		std::string decompressed = "";
		decompressed += v;
		std::string pv = v;
		for (auto& c : compressed) {
			if (table.find(c) == table.end()) {
				v = pv + pv[0];
			}
			else {
				v = table[c];
			}

			decompressed += v;

			if (code > max_code) {
				std::cout << "max_code overflow!\n";
				return "";
			}

			table[code] = pv + v[0];
			code += 1;
			pv = v;
		}

		std::cout << "Decompression Finished : " << decompressed << "\n";

		return decompressed;
	}
};

#endif