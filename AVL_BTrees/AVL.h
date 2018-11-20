#pragma once
class AVL
{
public:
	struct AVL_Node
	{
		int BF; 
		int fileIndex;   //where in the file this node is written
		int counter; 
		AVL_Node *leftChild = nullptr; 
		AVL_Node *rightChild = nullptr; 
		char key[50]; 
	};

	int writeIndex = 0;
	AVL_Node *rootNode; 

	AVL();
	void Insert(char in_key[]); 

private: 
	AVL_Node* _readFile(); 
};

