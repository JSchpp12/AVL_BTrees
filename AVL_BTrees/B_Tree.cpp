#include "pch.h"
#include "B_Tree.h"
#include <string.h>


B_Tree::B_Tree()
{
	//create B_Node stuff
}

void B_Tree::Insert(char in_key[])
{

}

//private

void B_Tree::_traverse(char in_key[])
{

}

bool B_Tree::_search(BNode *X, BNode *Y, char in_key[])
{
	bool found = false; 
	int i = 1; 
	while ((i < X->maxNumKey) && (strcmp(in_key, X->key[i]) > 0)) i++; 

	//If found key is the same as input, return node
	if (strcmp(in_key, X->key[i]) == 0) return true; 

	if (X->leaf == true) return false; 
	else
	{
		_readFile(X->child[i], X); 

	}
}