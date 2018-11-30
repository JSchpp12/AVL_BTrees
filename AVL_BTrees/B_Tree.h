#pragma once
#include "B_Node.h"
#include "B_Node.h"
#include <iostream>
#include <string.h>


class B_Tree
{
	struct BNode
	{
		static const int t = 2; 

		int fileIndex; 
		char key[2 * t][50]; //keys
		int child[2*t]; //pointers to children

		int maxNumKey = 2*t + 1; //max number of keys allowed 
		bool leaf = false; //is this node a leaf
	};
public:
	B_Tree();

	int numRootNode = 0; 
	int writeIndex = 1; 

	void Insert(char in_key[]);

private: 
	void _traverse(char in_key[]); 
	bool _search(BNode *X, BNode *Y, char in_key[]); 
	void _readFile(int location, BNode *X); 
	void _writeFile(BNode *X); 
};

