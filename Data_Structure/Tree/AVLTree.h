/* AVLTree.h */
#ifndef AVLTREE_H
#define AVLTREE_H
#define EH  0
#define LH +1
#define RH -1

template<typename T>
class NODE
{
public:
	NODE(T data) : data(data), right(nullptr), left(nullptr), bal(EH) {}

	T data;
	NODE* right;
	NODE* left;
	int bal;
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
class AVLTree
{
public:
	AVLTree(int(*compare)(T& arg1, T& arg2)) : count(0), root(nullptr), compare(compare) {};
	~AVLTree();

	bool insert(T data);
	bool erase(T data);
	int size() const;
private:
	NODE<T>* _insert(NODE<T>* root, NODE<T>* newNode, bool& taller);
	NODE<T>* _delete(NODE<T>* root, T dltKey, bool& shorter, bool& success);

	NODE<T>* insLeftBalancing(NODE<T>* root, bool& taller);
	NODE<T>* insRightBalancing(NODE<T>* root, bool& taller);

	NODE<T>* dltLeftBalancing(NODE<T>* root, bool& shorter);
	NODE<T>* dltRightBalancing(NODE<T>* root, bool& shorter);

	NODE<T>* rotateLeft(NODE<T>* root);
	NODE<T>* rotateRight(NODE<T>* root);

	void _destroy(NODE<T>* root);

	int count;
	NODE<T>* root;
	int(*compare)(T& arg1, T& arg2);
};

template<typename T>
AVLTree<T>::~AVLTree()
{
	if (root)
		_destroy(root);
}

template<typename T>
void AVLTree<T>::_destroy(NODE<T>* root)
{
	if (root->left)
		_destroy(root->left);
	if (root->right)
		_destroy(root->right);

	delete root;
}

template<typename T>
int AVLTree<T>::size() const
{
	return count;
}

template<typename T>
bool AVLTree<T>::insert(T data)
{
	NODE<T>* newNode = new NODE<T>(data);
	if (!newNode)
		return false;

	if (!root)
	{
		root = newNode;
	}
	else
	{
		bool taller;
		root = _insert(root, newNode, taller);
	}

	count++;

	return true;
}

template<typename T>
NODE<T>* AVLTree<T>::_insert(NODE<T>* root, NODE<T>* newNode, bool& taller)
{
	if (!root)
	{
		root = newNode;
		taller = true;

		return root;
	}

	if (compare(root->data, newNode->data) > 0)
	{
		root->left = _insert(root->left, newNode, taller);

		if (taller)
		{
			switch (root->bal)
			{
			case LH:
				root = insLeftBalancing(root, taller);
				break;

			case EH:
				root->bal = LH;
				break;

			case RH:
				root->bal = EH;
				taller = false;
				break;
			}
		}
	}
	else
	{
		root->right = _insert(root->right, newNode, taller);

		if (taller)
		{
			switch (root->bal)
			{
			case LH:
				root->bal = EH;
				taller = false;
				break;

			case EH:
				root->bal = RH;
				break;

			case RH:
				root = insRightBalancing(root, taller);
				break;
			}
		}
	}

	return root;
}

template<typename T>
NODE<T>* AVLTree<T>::insLeftBalancing(NODE<T>* root, bool& taller)
{
	NODE<T>* left, *right;

	left = root->left;

	switch (left->bal)
	{
	case LH:
		root->bal = EH;
		left->bal = EH;

		//rotate
		root = rotateRight(root);
		taller = false;
		break;

	case EH:
		//impossible case
		break;

	case RH:
		right = left->right;
		switch (right->bal)
		{
		case LH:
			root->bal = RH;
			left->bal = EH;
			break;

		case EH:
			root->bal = EH;
			left->bal = EH;
			break;

		case RH:
			root->bal = EH;
			left->bal = LH;
			break;
		}
		right->bal = EH;

		//roate Left
		root->left = rotateLeft(left);

		//rotate Right
		root = rotateRight(root);
		taller = false;

		break;
	}

	return root;
}

template<typename T>
NODE<T>* AVLTree<T>::insRightBalancing(NODE<T>* root, bool& taller)
{
	NODE<T> *left, *right;

	right = root->right;

	switch (right->bal)
	{
	case LH:
		left = right->left;

		switch (left->bal)
		{
		case LH:
			root->bal = EH;
			right->bal = RH;
			break;

		case EH:
			root->bal = EH;
			right->bal = EH;
			break;

		case RH:
			root->bal = LH;
			right->bal = EH;
			break;
		}
		left->bal = EH;

		//rotate Right
		root->right = rotateRight(right);

		//rotate Left
		root = rotateLeft(root);
		taller = false;
		break;

	case EH:
		//impossible case
		break;

	case RH:
		root->bal = EH;
		right->bal = EH;

		//rotate Left
		root = rotateLeft(root);
		taller = false;
		break;
	}

	return root;
}

template<typename T>
NODE<T>* AVLTree<T>::rotateLeft(NODE<T>* root)
{
	NODE<T>* temp;

	temp = root->right;
	root->right = temp->left;
	temp->left = root;

	return temp;
}

template<typename T>
NODE<T>* AVLTree<T>::rotateRight(NODE<T>* root)
{
	NODE<T>* temp;

	temp = root->left;
	root->left = temp->right;
	temp->right = root;

	return temp;
}

template<typename T>
bool AVLTree<T>::erase(T dltKey)
{
	bool success;
	bool shorter;
	NODE<T>* newRoot;

	newRoot = _delete(root, dltKey, shorter, success);

	if (success)
	{
		root = newRoot;
		count--;
		return true;
	}

	return false;
}

template<typename T>
NODE<T>* AVLTree<T>::_delete(NODE<T>* root, T dltKey, bool& shorter, bool& success)
{
	if (!root)
	{
		success = false;
		shorter = false;

		return nullptr;
	}

	if (compare(root->data, dltKey) > 0)
	{
		root->left = _delete(root->left, dltKey, shorter, success);

		if (shorter)
			root = dltRightBalancing(root, shorter);
	}
	else if (compare(root->data, dltKey) < 0)
	{
		root->right = _delete(root->right, dltKey, shorter, success);

		if (shorter)
			root = dltLeftBalancing(root, shorter);
	}
	else
	{
		NODE<T>* dltNode, *newRoot;
		dltNode = root;
		if (!root->right)
		{
			newRoot = root->left;
			success = true;
			shorter = true;

			free(dltNode);
			return newRoot;
		}
		else if (!root->left)
		{
			newRoot = root->right;
			success = true;
			shorter = true;

			free(dltNode);
			return newRoot;
		}
		else
		{
			NODE<T>* exchNode = root->left;

			//find max value of left subtree
			while (exchNode->right)
				exchNode = exchNode->right;

			//exchange data
			root->data = exchNode->data;

			//delete exchange Node
			root->left = _delete(root->left, exchNode->data, shorter, success);

			//rebalancing Right
			if (shorter)
				root = dltRightBalancing(root, shorter);
		}
	}

	return root;
}

template<typename T>
NODE<T>* AVLTree<T>::dltRightBalancing(NODE<T>* root, bool& shorter)
{
	NODE<T> *left, *right;

	switch (root->bal)
	{
	case LH:
		root->bal = EH;
		break;

	case EH:
		root->bal = RH;
		shorter = false;
		break;

	case RH:
		right = root->right;

		if (right->bal == LH)
		{
			left = right->left;

			switch (left->bal)
			{
			case LH:
				root->bal = EH;
				right->bal = RH;
				break;

			case EH:
				root->bal = EH;
				right->bal = EH;
				break;

			case RH:
				root->bal = LH;
				right->bal = EH;

				break;
			}
			left->bal = EH;

			//rotate Right
			root->right = rotateRight(right);
			//rotate Left
			root = rotateLeft(root);
		}
		else
		{
			switch (right->bal)
			{
			case EH:
				root->bal = RH;
				right->bal = LH;
				shorter = false;
				break;

			case RH:
				root->bal = EH;
				right->bal = EH;
				break;
			}

			root = rotateLeft(root);
		}
		break;
	}

	return root;
}

template<typename T>
NODE<T>* AVLTree<T>::dltLeftBalancing(NODE<T>* root, bool& shorter)
{
	NODE<T>* left, *right;

	switch (root->bal)
	{
	case LH:
		left = root->left;

		if (left->bal == RH)
		{
			right = left->right;

			switch (right->bal)
			{
			case LH:
				root->bal = RH;
				left->bal = EH;
				break;

			case EH:
				root->bal = EH;
				left->bal = EH;
				shorter = false;
				break;

			case RH:
				root->bal = EH;
				left->bal = LH;
				break;
			}
			right->bal = EH;

			//rotate Left
			root->left = rotateLeft(left);
			//rotate Right
			root = rotateRight(root);
		}
		else
		{
			switch (left->bal)
			{
			case LH:
				root->bal = EH;
				left->bal = EH;
				break;

			case EH:
				root->bal = LH;
				left->bal = RH;
				shorter = false;
				break;
			}

			root = rotateRight(root);
		}
		break;

	case EH:
		root->bal = LH;
		shorter = false;
		break;

	case RH:
		root->bal = EH;
		break;
	}

	return root;
}
#endif