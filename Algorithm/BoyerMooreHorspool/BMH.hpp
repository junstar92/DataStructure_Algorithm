#ifndef __BMH_HPP__
#define __BMH_HPP__
/* Boyer-Moore-Horspool Algorithm */

#include <string>
#include <vector>
#include <map>

#define TABLE_SIZE 128

std::vector<int> createRtOccurrencesTable(std::string pattern);
std::vector<int> boyerMooreHorspool(std::string pattern, std::string text);

std::map<char, int> createRtOccurrencesTableUsingHash(std::string pattern);
std::vector<int> boyerMooreHorspoolUsingHash(std::string pattern, std::string text);

#endif