/* BTree.h */
#ifndef BTREE_H
#define BTREE_H
#include <queue>

const int max_degree = 2;

template<typename T>
class Node {
public:
	Node(const T& data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {}
	~Node();

	T data;
	Node *parent, *left, *right;
};

template<typename T>
Node<T>::~Node()
{
	delete child;
}

template<typename T>
class BTree
{
public:
	BTree() : count(0), root(nullptr) {}
	void insert(const T& data);
	bool empty() const;
	int size() const;
	void Traversal() const;
	void DFS() const;
	void BFS() const;
	
private:
	void _insert(Node<T>* root, Node<T>* newNode);
	void _preOrder(Node<T>* root) const;
	void _inOrder(Node<T>* root) const;
	void _postOrder(Node<T>* root) const;
	void _dfs(Node<T>* root) const;

	int count;
	Node<T>* root;
};

template<typename T>
void BTree<T>::insert(const T& data)
{
	if (empty())
	{
		root = new Node<T>(data);
		++count;
	}
	else
	{
		Node<T>* newNode = new Node<T>(data);
		_insert(root, newNode);
	}
}

template<typename T>
void BTree<T>::_insert(Node<T>* root, Node<T>* newNode)
{
	std::queue<Node<T>*> q;
	
	if (!root)
		return;

	//search node position by using BFS
	q.push(root);

	while (!q.empty())
	{
		Node<T>* curNode = q.front();
		q.pop();

		if (curNode->left == nullptr)
		{
			curNode->left = newNode;
			return;
		}
		else
			q.push(curNode->left);

		if (curNode->right == nullptr)
		{
			curNode->right = newNode;
			return;
		}
		else
			q.push(curNode->right);
	}

	return;
}

template<typename T>
bool BTree<T>::empty() const
{
	return (count == 0);
}

template<typename T>
int BTree<T>::size() const
{
	return count;
}

template<typename T>
void BTree<T>::Traversal() const
{
	if (root)
	{
		//PreOrder
		std::cout << "Preorder : ";
		_preOrder(root);
		std::cout << "\n";
		//InOrder
		std::cout << "Inorder : ";
		_inOrder(root);
		std::cout << "\n";
		//PostOrder
		std::cout << "Preorder : ";
		_postOrder(root);
		std::cout << "\n";
	}
}

template<typename T>
void BTree<T>::_preOrder(Node<T>* root) const
{
	std::cout << root->data << " ";
	if (root->left)
		_preOrder(root->left);
	if (root->right)
		_preOrder(root->right);
}

template<typename T>
void BTree<T>::_inOrder(Node<T>* root) const
{
	if (root->left)
		_inOrder(root->left);
	std::cout << root->data << " ";
	if (root->right)
		_inOrder(root->right);
}

template<typename T>
void BTree<T>::_postOrder(Node<T>* root) const
{
	if (root->left)
		_postOrder(root->left);
	if (root->right)
		_postOrder(root->right);
	std::cout << root->data << " ";
}

template<typename T>
void BTree<T>::DFS() const
{
	if (root)
	{
		std::cout << "DFS : ";
		_dfs(root);
		std::cout << "\n";
	}

	return;
}

template<typename T>
void BTree<T>::_dfs(Node<T>* root) const
{
	std::cout << root->data << " ";
	if (root->left)
		_dfs(root->left);
	if (root->right)
		_dfs(root->right);
}

template<typename T>
void BTree<T>::BFS() const
{
	std::queue<Node<T>*> q;

	if (!root)
		return;

	std::cout << "BFS : ";
	//search node position by using BFS
	q.push(root);

	while (!q.empty())
	{
		Node<T>* curNode = q.front();
		q.pop();
		std::cout << curNode->data << " ";
		if (curNode->left)
			q.push(curNode->left);

		if (curNode->right)
			q.push(curNode->right);
	}

	std::cout << "\n";
}

#endif