#ifndef HUFFMAN
#define HUFFMAN

#include <string>
#include <queue>
#include <stack>
#include <map>
#include <iostream>

class Node {
public:
	Node() : left(nullptr), right(nullptr), cnt(0), ch(0) {}
	Node(char ch, int cnt) : left(nullptr), right(nullptr), cnt(cnt), ch(ch) {}
	Node(Node* left, Node* right) : left(left), right(right), cnt(left->cnt + right->cnt), ch(0) {}

	bool operator()(const Node* lhs, const Node* rhs) const;

	Node* left;
	Node* right;
	uint32_t cnt;
	char ch;
};

class Huffman {
private:
	Node* binTree;
	std::string code;

	void _getCode(const std::string& input);

public:
	Huffman() : binTree(nullptr), code("") {}
	~Huffman();

	void encoding(const std::string& input);
	std::string getCode() const;
	std::string decoding() const;
};

#endif