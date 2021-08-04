#include "Huffman.h"

bool Node::operator()(const Node* lhs, const Node* rhs) const {
	return lhs->cnt > rhs->cnt;
}

Huffman::~Huffman() {
	std::stack<Node*> st;
	st.push(binTree);

	while (!st.empty()) {
		Node* cur = st.top();
		st.pop();

		if (cur->left)
			st.push(cur->left);
		if (cur->right)
			st.push(cur->right);

		delete cur;
	}

	binTree = nullptr;
}

void Huffman::_getCode(const std::string& input) {
	std::string code = "";
	std::map<char, std::string> m;

	std::stack<std::pair<Node*, std::string>> st;
	st.push({ binTree, "" });

	while (!st.empty()) {
		Node* cur = st.top().first;
		std::string tmp = st.top().second;
		st.pop();

		if (cur->right != nullptr) {
			st.push({ cur->right, tmp + "1" });
		}
		if (cur->left != nullptr) {
			st.push({ cur->left, tmp + "0" });
		}

		if (cur->left == nullptr && cur->right == nullptr) {
			m[cur->ch] = tmp;
		}
	}

	for (const char& c : input) {
		code += m[c];
	}
	this->code = code;

	return;
}

std::string Huffman::getCode() const {
	if (code == "") {
		std::cout << "Encoding First !\n";
	}

	return code;
}

void Huffman::encoding(const std::string& input) {
	std::cout << "Input String : " << input << "\n";

	// count
	std::map<char, int> m;
	
	for (auto& c : input) {
		m[c]++;
	}

	// push into pq
	std::priority_queue<Node*, std::vector<Node*>, Node> pq;
	for (auto& e : m) {
		pq.push(new Node(e.first, e.second));
	}

	while (pq.size() > 1) {
		Node* left = pq.top();
		pq.pop();
		Node* right = pq.top();
		pq.pop();

		Node* newNode = new Node(left, right);
		pq.push(newNode);
	}

	binTree = pq.top();
	pq.pop();

	//convert tree to huffman code
	_getCode(input);

	return;
}

std::string Huffman::decoding() const {
	if (binTree == nullptr) {
		std::cout << "Encoding First !\n";

		return "";
	}

	std::string ret = "";

	const Node* cur = binTree;
	for (const char& c : code) {
		if (c == '0') {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}

		if (cur->left == nullptr && cur->right == nullptr) {
			ret += cur->ch;
			cur = binTree;
		}
	}

	return ret;
}