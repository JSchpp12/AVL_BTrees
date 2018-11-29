#pragma once
#include "pch.h"
#include "AVL.h"
#include <io.h>
#include <iostream>
#include <string.h>

class AVL
{
public:
	struct AVL_Node
	{
		int BF = 0; 
		int fileIndex = 0; //where in the file this node is written
		int counter = 1; 
		int numLeftChild = 0; 
		int numRightChild = 0; 
		char key[50]; 
	};



	std::string storageFile = "storage.txt"; //file to store the nodes in 

	//use this to store the 3 nodes that are permitted for the memory restriction
	AVL_Node nodeStorage[2]; 
	int nodeStorage_index = 0; 

	int writeIndex = 0; 
	int numRootNode; 
	AVL_Node *rootNode; 

	AVL();
	~AVL(); 
	void Insert(char in_key[]); 
	void Read(); 
	
private: 
	void LL_Rotate(AVL_Node *rotationPoint,AVL_Node *B ,AVL_Node *C); 
	void RR_Rotate(AVL_Node *rotationPoint, AVL_Node *B, AVL_Node *C);
	int _calculateBalanceFactor(AVL_Node *tippingNode); 
	int _getNodeHeight(AVL_Node *focusNode); 
	void _nodeReader(int index, AVL_Node* retrunedNode); 
	void _nodeWriter(AVL_Node *targetNode); 
	void _readFile();
};

