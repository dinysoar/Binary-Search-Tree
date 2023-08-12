#include <iostream>
#include <string>
using namespace std;

template <typename K, typename V>
class Entry {
public:
	typedef K Key;
	typedef V Value;
	// constructor
	Entry(const K& k = K(), const V& v = V()) : _key(k), _value(v) {}
	//accessors
	const K& key() const { return _key; }
	const V& value() const { return _value; }
	//mutators
	void setKey(const K& k) { _key = k; }
	void setValue(const V& v) { _value = v; }
private:
	K _key;
	V _value;

};



template <typename K, typename V>
class BinaryTree {
protected:
	struct Node {
		Entry<K, V> _entry;
		Node* _parent;
		Node* _left;
		Node* _right;
		Node(const Entry<K, V>& e = Entry<K, V>(), Node* p = nullptr, Node* l = nullptr, Node* r = nullptr) : _entry(e), _parent(p), _left(l), _right(r) {}
	};
	Node* _root;
	int n;

public:

	typedef K Key;
	typedef V Value;
	class Iterator {
	public:
		typedef typename BinaryTree<K, V>::Node Node;
		Iterator(Node* u = nullptr) : _node(u) {}
		const Entry<K, V>& operator*() const { return _node->_entry; }
		const Entry<K, V>& operator*() { return _node->_entry; }
		bool operator==(const Iterator& p) const { return _node == p._node; }
		bool operator!=(const Iterator& p) const { return _node != p._node; }
		Iterator& operator++();
		Iterator& operator--();
		friend class BinaryTree<K, V>;
	private:
		Node* _node;
	};

public:

	BinaryTree();
	int size() const;
	bool empty() const;
	Iterator begin() const;
	Iterator end() const;
	void addRoot();
	void expandExternal(const Iterator& p);
	Iterator removeAboveExternal(const Iterator& p);
	Iterator root() const;
	void preorder(Node* v, int depth) const;
	void inorder(Node* v, int depth) const;
	void postorder(Node* v, int depth) const;
	void print() const;
	void print(Node* v, int depth) const;
	void insert(const K& k, const V& v);
	void insert(const Entry<K, V>& e);
	Iterator find(const K& k) const;
	Iterator find(const K& k);
	void erase(const K& k);
	void findPrint(const K& k);
};

//constructor
template <typename K, typename V>
BinaryTree<K, V>::BinaryTree() : _root(nullptr), n(0) {}
//size
template <typename K, typename V>
int BinaryTree<K, V>::size() const { return n; }
//empty
template <typename K, typename V>
bool BinaryTree<K, V>::empty() const { return size() == 0; }
//begin
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator BinaryTree<K, V>::begin() const { return Iterator(_root); }

//end
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator BinaryTree<K, V>::end() const { return Iterator(nullptr); }
//addRoot
template <typename K, typename V>
void BinaryTree<K, V>::addRoot() {
	_root = new Node;
	n = 1;
}



//expandExternal
template <typename K, typename V>
void BinaryTree<K, V>::expandExternal(const Iterator& p) {
	Node* v = p._node;
	v->_left = new Node;
	v->_left->_parent = v;
	v->_right = new Node;
	v->_right->_parent = v;
	n += 2;
}
//removeAboveExternal
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator BinaryTree<K, V>::removeAboveExternal(const Iterator& p) {
	Node* w = p._node;
	Node* v = w->_parent;
	Node* sib = (w == v->_left ? v->_right : v->_left);
	if (v == _root) {
		_root = sib;
		sib->_parent = nullptr;
	}
	else {
		Node* gpar = v->_parent;
		if (v == gpar->_left) gpar->_left = sib;
		else gpar->_right = sib;
		sib->_parent = gpar;
	}
	delete w;
	delete v;
	n -= 2;
	return Iterator(sib);
}

// Iterator
//operator++
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator& BinaryTree<K, V>::Iterator::operator++() {
	Node* w = _node->_right;
	if (w == nullptr) {
		w = _node;
		Node* p = w->_parent;
		while (w == p->_right) {
			w = p;
			p = w->_parent;
		}
		w = p;
	}
	else {
		while (w->_left != nullptr) w = w->_left;
	}
	_node = w;
	return *this;
}

//operator--
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator& BinaryTree<K, V>::Iterator::operator--() {
	Node* w = _node->_left;
	if (w == nullptr) {
		w = _node;
		Node* p = w->_parent;
		while (w == p->_left) {
			w = p;
			p = w->_parent;
		}
		w = p;
	}
	else {
		while (w->_right != nullptr) w = w->_right;
	}
	_node = w;
	return *this;
}


//root
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator BinaryTree<K, V>::root() const { return Iterator(_root); }

//preorder
template <typename K, typename V>
void BinaryTree<K, V>::preorder(Node* v, int depth) const {
	for (int i = 0; i < depth; i++) cout << " ";
	cout << v->_entry.key() << endl;
	if (v->_left != nullptr) preorder(v->_left, depth + 1);
	if (v->_right != nullptr) preorder(v->_right, depth + 1);
}
//inorder
template <typename K, typename V>
void BinaryTree<K, V>::inorder(Node* v, int depth) const {
	if (v->_left != nullptr) inorder(v->_left, depth + 1);
	for (int i = 0; i < depth; i++) cout << " ";
	cout << v->_entry.key() << endl;
	if (v->_right != nullptr) inorder(v->_right, depth + 1);
}
//postorder
template <typename K, typename V>
void BinaryTree<K, V>::postorder(Node* v, int depth) const {
	if (v->_left != nullptr) postorder(v->_left, depth + 1);
	if (v->_right != nullptr) postorder(v->_right, depth + 1);
	for (int i = 0; i < depth; i++) cout << " ";
	cout << v->_entry.key() << endl;
}
//print
template <typename K, typename V>
void BinaryTree<K, V>::print() const {
	if (_root != nullptr) print(_root, 0);
}
//print
template <typename K, typename V>
void BinaryTree<K, V>::print(Node* v, int depth) const {
	if (v->_left != nullptr) print(v->_left, depth + 1);
	for (int i = 0; i < depth; i++) cout << " ";
	cout << v->_entry.key() << endl;
	if (v->_right != nullptr) print(v->_right, depth + 1);
}
//insert
template <typename K, typename V>
void BinaryTree<K, V>::insert(const K& k, const V& v) {
	Entry<K, V> e(k, v);
	insert(e);
}
//insert
template <typename K, typename V>
void BinaryTree<K, V>::insert(const Entry<K, V>& e) {
	Node* newNode = new Node;
	newNode->_entry = e;
	if (_root == nullptr) {
		_root = newNode;
		n = 1;
	}
	else {
		Node* curr = _root;
		while (true) {
			if (e.key() < curr->_entry.key()) {
				if (curr->_left == nullptr) {
					curr->_left = newNode;
					newNode->_parent = curr;
					n++;
					break;
				}
				else curr = curr->_left;
			}
			else {
				if (curr->_right == nullptr) {
					curr->_right = newNode;
					newNode->_parent = curr;
					n++;
					break;
				}
				else curr = curr->_right;
			}
		}
	}
}

// now what?
template <typename K, typename V>
void BinaryTree<K, V>::erase(const K& k) {
	Node* curr = _root;
	while (curr != nullptr) {
		if (k < curr->_entry.key()) curr = curr->_left;
		else if (k > curr->_entry.key()) curr = curr->_right;
		else break;
	}
	if (curr == nullptr) return;
	if (curr->_left == nullptr && curr->_right == nullptr) {
		if (curr == _root) _root = nullptr;
		else {
			Node* par = curr->_parent;
			if (curr == par->_left) par->_left = nullptr;
			else par->_right = nullptr;
		}
		delete curr;
		n--;
	}
	else if (curr->_left == nullptr || curr->_right == nullptr) {
		Node* child = (curr->_left == nullptr ? curr->_right : curr->_left);
		if (curr == _root) {
			_root = child;
			child->_parent = nullptr;
		}
		else {
			Node* par = curr->_parent;
			if (curr == par->_left) par->_left = child;
			else par->_right = child;
			child->_parent = par;
		}
		delete curr;
		n--;
	}
	else {
		Node* w = curr->_right;
		while (w->_left != nullptr) w = w->_left;
		curr->_entry = w->_entry;
		Node* par = w->_parent;
		if (w == par->_left) par->_left = w->_right;
		else par->_right = w->_right;
		if (w->_right != nullptr) w->_right->_parent = par;
		delete w;
		n--;
	}
}

//find position of key
//find and return position of key
template <typename K, typename V>
typename BinaryTree<K, V>::Iterator BinaryTree<K, V>::find(const K& k) {
	Node* curr = _root;
	while (curr != nullptr) {
		if (k < curr->_entry.key()) curr = curr->_left;
		else if (k > curr->_entry.key()) curr = curr->_right;
		else break;
	}
	return Iterator(curr);
}
//find and print if a key is left or right child
template <typename K, typename V>
void BinaryTree<K, V>::findPrint(const K& k) {
	Node* curr = _root;
	while (curr != nullptr) {
		if (k < curr->_entry.key()) {
			cout << "left" << endl;
			curr = curr->_left;
		}
		else if (k > curr->_entry.key()) {
			cout << "right" << endl;
			curr = curr->_right;
		}
		else break;
	}
}



int main() {
	BinaryTree<int, string> tree;


	tree.insert(5, "five");
	tree.insert(3, "three");
	tree.insert(7, "seven");
	tree.insert(2, "two");
	tree.insert(4, "four");
	tree.insert(6, "six");
	tree.insert(8, "eight");
	//tree.erase(5);
	//return tree node position
	// find if node is left or right child
	tree.findPrint(4);
	//print tree
	//tree.print();


	return 0;
}