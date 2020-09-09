/* BST.h : Binary Search Tree */
#ifndef BST_H
#define BST_H
template<typename T>
class NODE
{
public:
	NODE(T data) : key(data), right(nullptr), left(nullptr) {}

	T key;
	NODE* right;
	NODE* left;
};

int compare(int& arg1, int& arg2)
{
	if (arg1 > arg2)
		return 1;
	else if (arg1 == arg2)
		return 0;
	else
		return -1;
}

template<typename T>
class BST
{
public:
	BST(int(*compare)(T& arg1, T& arg2)) : size(0), root(nullptr), compare(compare) {}
	~BST();

	bool empty() const;
	bool full() const;
	bool insert(T data);
	bool erase(T key);

	void traversals() const;
private:
	int size;
	NODE<T>* root;
	int(*compare)(T& arg1, T& arg2);

	NODE<T>* _insert(NODE<T>* root, NODE<T>* newNode);
	NODE<T>* _delete(NODE<T>* root, T key, bool* success);
	void _destroy(NODE<T>* root);

	void _preOrder(NODE<T>* root) const;
	void _inOrder(NODE<T>* root) const;
	void _postOrder(NODE<T>* root) const;
};

template<typename T>
BST<T>::~BST()
{
	size = 0;
	_destroy(root);
}

template<typename T>
bool BST<T>::empty() const
{
	return (size == 0);
}

template<typename T>
bool BST<T>::full() const
{
	NODE<T>* newNode = new NODE<T>(0);

	if (!newNode)
	{
		return true;
	}
	else
	{
		delete newNode;
		return false;
	}
}

template<typename T>
bool BST<T>::insert(T data)
{
	if (full())
	{
		std::cout << "BST is full\n";
		return false;
	}

	NODE<T>* newNode = new NODE<T>(data);
	
	if (size == 0)
		root = newNode;
	else
		_insert(root, newNode);

	size++;

	return true;
}

template<typename T>
NODE<T>* BST<T>::_insert(NODE<T>* root, NODE<T>* newNode)
{
	if (!root)
		return newNode;

	if (compare(root->key, newNode->key) > 0)
		root->left = _insert(root->left, newNode);
	else
		root->right = _insert(root->right, newNode);

	return root;
}

template<typename T>
bool BST<T>::erase(T key)
{
	if (empty())
		return false;

	bool success;

	root = _delete(root, key, &success);

	if (success)
		--size;

	return success;
}

template<typename T>
NODE<T>* BST<T>::_delete(NODE<T>* root, T key, bool* success)
{
	if (!root)
	{
		*success = false;
		return root;
	}

	if (compare(root->key, key) == 0)
	{
		NODE<T>* delNode = root;

		if (root->left == nullptr)
		{
			root = root->right;
			delete delNode;
		}
		else if (root->right == nullptr)
		{
			root = root->left;
			delete delNode;
		}
		else
		{
			NODE<T>* exchNode = root->left;
			while (exchNode->right)
				exchNode = exchNode->right;

			root = new NODE<T>(exchNode->key);
			root->left = delNode->left;
			root->right = delNode->right;
			delete delNode;
			root->left = _delete(root->left, exchNode->key, success);
		}

		*success = true;
	}
	else if (compare(root->key, key) > 0)
	{
		root->left = _delete(root->left, key, success);
	}
	else
	{
		root->right = _delete(root->right, key, success);
	}

	return root;
}

template<typename T>
void BST<T>::_destroy(NODE<T>* root)
{
	if (!root)
		return;

	if (root->left)
		_destroy(root->left);
	if (root->right)
		_destroy(root->right);

	delete root;
}

template<typename T>
void BST<T>::traversals() const
{
	std::cout << "Preorder : ";
	_preOrder(root);
	std::cout << "\nInorder : ";
	_inOrder(root);
	std::cout << "\nPostorder : ";
	_postOrder(root);
}

template<typename T>
void BST<T>::_preOrder(NODE<T>* root) const
{
	if (!root)
		return;

	std::cout << root->key << " ";
	_preOrder(root->left);
	_preOrder(root->right);
}

template<typename T>
void BST<T>::_inOrder(NODE<T>* root) const
{
	if (!root)
		return;

	_preOrder(root->left);
	std::cout << root->key << " ";
	_preOrder(root->right);
}

template<typename T>
void BST<T>::_postOrder(NODE<T>* root) const
{
	if (!root)
		return;

	_preOrder(root->left);
	_preOrder(root->right);
	std::cout << root->key << " ";
}
#endif