#include <iostream>

using namespace std;

// структура, представляющая ущел в splay дереве

struct Node {

	int data; // ключ

	Node* parent; // ссылка на родителя

	Node* left; // ссылка на правое поддерево

	Node* right; // на левое

};

typedef Node* NodePtr;

// класс реализации splay дерева

class SplayTree {

private:

	NodePtr root;//корень дерева

	//поиск узла в дереве(обычный поиск, как в бинарном дереве)

	NodePtr searchTreeHelper(NodePtr node, int key) {

		if (node == nullptr || key == node->data) {

			return node;

		}

		if (key < node->data) {

			return searchTreeHelper(node->left, key);

		}

		return searchTreeHelper(node->right, key);

	}

	//удаление узла в splay дереве

	void deleteNodeHelper(NodePtr node, int key) {

		NodePtr x = nullptr;//бедующий узел, который нужно будет удалить

		NodePtr t, s;//два поддерева s < t

		//поиск узла для удаления

		while (node != nullptr) {

			if (node->data == key) {

				x = node;

			}

			if (node->data <= key) {

				node = node->right;

			}

			else {

				node = node->left;

			}

		}

		if (x == nullptr) {

			cout << "Данного ключа нет в дереве" << endl;

			return;

		}

		split(x, s, t); // разбиваем дерево на два поддерва s и t

		//удаляем x

		if (s->left) {

			s->left->parent = nullptr;

		}

		root = join(s->left, t);//создаем новое дерево, соединяя два дерева s и t

		delete(s);

		s = nullptr;

	}

	//вывод дерева

	void printHelper(NodePtr root, string indent, bool last) {

		if (root != nullptr) {

			cout << indent;

			if (last) {

				cout << "Right ";

				indent += " ";

			}

			else {

				cout << "Left ";

				indent += "| ";

			}

			cout << root->data << endl;

			printHelper(root->left, indent, false);

			printHelper(root->right, indent, true);

		}

	}

	// поворот дерево с права (относительно x)

	void leftRotate(NodePtr x) {

		NodePtr y = x->right;

		x->right = y->left;

		if (y->left != nullptr) {

			y->left->parent = x;

		}

		y->parent = x->parent;

		if (x->parent == nullptr) {

			this->root = y;

		}

		else if (x == x->parent->left) {

			x->parent->left = y;

		}

		else {

			x->parent->right = y;

		}

		y->left = x;

		x->parent = y;

	}

	// поворот дерево с лева (относительно x)

	void rightRotate(NodePtr x) {

		NodePtr y = x->left;

		x->left = y->right;

		if (y->right != nullptr) {

			y->right->parent = x;

		}

		y->parent = x->parent;

		if (x->parent == nullptr) {

			this->root = y;

		}

		else if (x == x->parent->right) {

			x->parent->right = y;

		}

		else {

			x->parent->left = y;

		}

		y->right = x;

		x->parent = y;

	}

	// реализация splay для узла

	void splay(NodePtr x) {

		while (x->parent) {

			if (!x->parent->parent) {

				if (x == x->parent->left) {

					// случай zig rotation

					rightRotate(x->parent);

				}

				else {

					// случай zag rotation

					leftRotate(x->parent);

				}

			}

			else if (x == x->parent->left && x->parent == x->parent->parent->left) {

				// случай zig-zig rotation

				rightRotate(x->parent->parent);

				rightRotate(x->parent);

			}

			else if (x == x->parent->right && x->parent == x->parent->parent->right) {

				// случай zag-zag rotation

				leftRotate(x->parent->parent);

				leftRotate(x->parent);

			}

			else if (x == x->parent->right && x->parent == x->parent->parent->left) {

				//случай zig-zag rotation

				leftRotate(x->parent);

				rightRotate(x->parent);

			}

			else {

				// случай zag-zig rotation

				rightRotate(x->parent);

				leftRotate(x->parent);

			}

		}

	}

	// операция объединение двух поддеревьев s и t, s < t

	NodePtr join(NodePtr s, NodePtr t) {

		//случаи, когда соединять нечего

		if (!s) {

			return t;

		}

		if (!t) {

			return s;

		}

		//находим наибольший элемен в s, который и соединит деревья s и t

		NodePtr x = maximum(s);

		//соединения деревьев

		splay(x);

		x->right = t;

		t->parent = x;

		return x;

	}

	//операция разбьение от заданного узла x на два поддерева s и t s < t

	void split(NodePtr& x, NodePtr& s, NodePtr& t) {

		splay(x);//поднимаем x к корню чтобы выделить два поддерва включающих все узлы в дереве

		//если правый потомк x не NULL то все узлы больше значения в x находится справа => t = x->right

		if (x->right) {

			t = x->right;

			t->parent = nullptr;

		}

		//иначе x - максимальный узел

		else {

			t = nullptr;

		}

		// опредялем s - левое поддерево x

		s = x;

		s->right = nullptr;

		x = nullptr;

	}

public:

	SplayTree() {

		root = nullptr;

	}

	//поиск узла в splay дереве

	NodePtr searchTree(int k) {

		NodePtr x = searchTreeHelper(this->root, k);//находим узел для вставки

		if (x) {

			splay(x);//"переносим" узел на корень

		}

		return x;

	}

	// поиск максимального ключа

	NodePtr maximum(NodePtr node) {

		while (node->right != nullptr) {

			node = node->right;

		}

		return node;

	}

	// вставка узла

	void insert(int key) {


		NodePtr node = new Node;

		node->parent = nullptr;

		node->left = nullptr;

		node->right = nullptr;

		node->data = key;

		NodePtr y = nullptr;

		NodePtr x = this->root;


		while (x != nullptr) {

			y = x;

			if (node->data < x->data) {

				x = x->left;

			}

			else {

				x = x->right;

			}

		}

		//связываем родителя созданнова узла с найенным местом для вставки

		node->parent = y;

		// случай корня

		if (y == nullptr) {

			root = node;

		}

		//связываем место для вставки с новым узлом соотвествующщиму key

		else if (node->data < y->data) {

			y->left = node;

		}

		else {

			y->right = node;

		}

		// splay только что вставленного узла

		splay(node);

	}
	void Print(NodePtr n, int level = 0)
	{
		if (n)
		{
			Print(n, level + 1);
			for (int i = 0; i < level; i++) std::cout << "   ";
			std::cout << n->data << std::endl;
			Print(n, level + 1);
		}
	}

	
	void deleteNode(int data) {

		deleteNodeHelper(this->root, data);

	}

	
	void prettyPrint() {

		printHelper(this->root, "", true);

	}

};

int main() {

	SplayTree splay;

	splay.insert(33);

	splay.insert(44);

	splay.insert(67);

	splay.insert(5);

	splay.insert(89);

	splay.insert(41);

	splay.prettyPrint();
	cout << endl << endl << endl << endl << endl << endl;
	splay.deleteNode(44);
	splay.prettyPrint();
	cout << endl << endl << endl << endl << endl << endl;
	splay.insert(44);

	splay.prettyPrint();

	return 0;

}
