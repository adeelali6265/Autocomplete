/*
Project 1
Name: Adeel Ali
Description: A search completion program using Trie structure
*/

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

//nodes will contain information on strings of letters
class Node {
	friend class Trie;
	
	private:
		string word; //string of letters for each node
		Node **children; //array of pointers
		int size; //size of array of pointers
	public:
		string getWord() {
			return word; //return string of letters
		};
		Node **getChildren() {
			return children; //return the array
		}
		int getSize() {
			return size; //return size of array
		}
		bool isWord; //record whether a string is an actual word
		
		//constructors and destructors
		Node(void): word(NULL), children(NULL), size(0) {};
		Node(string w, Node **c, int sz): word(w), children(c), size(sz){};
		~Node();
};

//trie will consist of nodes to be traversed to find actual words
class Trie {
	
	private:
		Node *root;
		
	public:
		//Trie Creation
		Node *checkNode(string s, Node *c);
		int increaseSize(Node *parent, Node *child);
		Node **insertChild(Node *parent, Node *child);
		void insert(string s);
		int j; //iterator
		
		//Trie Traversal
		Node *getRoot() {
			return root;
		}
		Node *depthSearch(string s, Node *c);
		void traverseRealWords(Node *c);
		Trie();
		~Trie();
};

//constructor
Trie::Trie() {
	root = new Node("", NULL, 0); //avoiding using NULL as name of string
	j = 0; //initialize iterator to 0
}

//destructor
Trie::~Trie() {
}

//checks whether a node is in the Trie or not
Node *Trie::checkNode(string s, Node *c) {
	
	//determine whether a child node has the desired string
	for (int i = 0; i < c -> getSize(); i++) {
		//if existing node has desired string, return that node
		if (c -> getChildren()[i] -> getWord() == s) {
			c = c -> getChildren()[i];
			return c;
		}
	}
	
	//if desired string could not be found,
	//return a new node with the desired string
	return new Node(s, NULL, 0);
}

//determines whether size of child array needs to be increased or not
int Trie::increaseSize(Node *parent, Node *child) {
	
	for (int i = 0; i < parent -> getSize(); i++) {
		//if the child is an existing node in the parent, do not increment 
		if (parent -> getChildren()[i] -> getWord() == child -> getWord()) {
			return parent -> getSize();
		}
	}
	
	//if child is not in parent node, increment child array size by 1
	return parent -> getSize() + 1;
}

//properly updates the children of a node
Node **Trie::insertChild(Node *parent, Node *child) {

	//assuming child is not in parent node, increment child array size by 1
	int d = parent -> getSize() + 1;
	int index = d - 1;
	for (int i = 0; i < parent -> getSize(); i++) {
		
		//if child already exists, do not increment array size
		if (parent -> getChildren()[i] -> getWord() == child -> getWord()) {
			
			//if child already exists, preserve if it is a real word or not
			if (parent -> getChildren()[i] -> isWord == true) {
				child -> isWord = true;
			}
			d = parent -> getSize();
			//re-insert child at original index
			index = i;
		}
	}
	Node **temp = new Node *[d];
	
	for (int i = 0; i < parent -> getSize(); i++) {
		temp[i] = parent -> getChildren()[i];
	}
	
	//index will either be original or last in array
	temp[index] = child;
	return temp; //return new or original array	
}

//function to insert nodes into a Trie from an input word
void Trie::insert(string s) {
	 if (j < s.length()) {
		Node *pRoot = root; //parent root
		string letters = s.substr(0, j); //substring of letters
		j++; //increment substring recursively
		root = checkNode(s.substr(0,j), root); //determine whether node exists
		insert(s);
		root = new Node(letters, insertChild(pRoot, root), increaseSize(pRoot, root)); //update parent root with new child root
	} else {
		root = new Node(s.substr(0,j), NULL, 0); //last child node is full word
		root -> isWord = true; //mark as actual word
		j = 0; //reset iterator to 0 after leaf has been added
	}
}

//function to find desired node based on input string
Node *Trie::depthSearch(string s, Node *c){
	//depth first searches for node until it matches input string
	if (c-> getWord() != s) {
		j++;
		string letters = s.substr(0, j); //substring will increment by 1
		Node *pHolder = c; //placeholder root
		
		//retrieve a child from root node that matches substring
		for (int i = 0; i < c -> getSize(); i++) {
			if (c -> getChildren()[i] -> getWord() == letters) {
				c = c -> getChildren()[i];
			}
		}
		//if no child matches, it cannot be autocompleted
		if (pHolder -> getWord() == c -> getWord()) {
			cout << "No valid word completions found." << endl;
			j = 0; //reset iterator
			return NULL;
		}
		depthSearch(s, c);
	} else {
		//once node is found, search for possible autocomplete entries
		j = 0; //reset iterator after depth search
		traverseRealWords(c);
		j = 0; //reset iterator after breadth search
	}
}

//part of depthSearch() that prints out actual autocomplete entries
void Trie::traverseRealWords(Node *c) {
	int count = 5; //number of autocomplete entries
	if (j < count) {
		Node *temp = c; //placeholder node for Trie traversal
		for (int i = 0; i < c -> getSize(); i++) {
			temp = c -> getChildren()[i];
			
			//if placeholder is an actual word, print it
			//and increment count by 1
			if ((temp -> isWord == true) && (j < count)) {
				cout << ">> " << temp -> getWord() << endl;
				j++;
				traverseRealWords(temp);
			} else if ((temp -> getSize() != 0) && (j < count)) {
				//if placeholder is not an actual word,
				//but has children, then repeat code for children
				traverseRealWords(temp);
			}
		}
	}
}

void createTrie(Trie &t);
void traverseTrie(Trie &t);

int main() {
	Trie T;
	createTrie(T);
	traverseTrie(T);
	return 0;
}

//creates the Trie with nodes based on file to be read
void createTrie(Trie &t) {
	string s; //placeholder string for reading/insertion
	ifstream pRead; //file handle
	pRead.open("Dictionary.txt"); //file to be read
	
	//read and insert words in Trie until end of file
	while (!pRead.eof()) {
		getline(pRead, s);
		t.insert(s);
	}
	pRead.close(); //close file when complete
}

//searches the Trie and prints autocomplete entries
void traverseTrie(Trie &t) {
	string y; //user input string
	
	while (true) {
		cout << "Please type search queries: (Type 0 to Exit)" << endl;
		cin >> y;
		if (y == "0") {
			//exit loop
			break;
		} else {
			//traverse until user input string is encountered and output NULL otherwise
			t.depthSearch(y, t.getRoot());
			cout << "\n";
		}
	} 
}
