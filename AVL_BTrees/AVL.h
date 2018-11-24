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
		int BF; 
		int fileIndex;   //where in the file this node is written
		int counter; 
		AVL_Node *leftChild = nullptr; 
		AVL_Node *rightChild = nullptr; 
		char key[50]; 
	};

	int writeIndex = 0;
	AVL_Node *rootNode; 
	AVL_Node *returnedNode; //using this pointer to return a node to the insert methods from rotation methods 
	AVL();
	void Insert(char in_key[]); 
	
private: 
	AVL_Node* _readFile();
	void LL_Rotate(AVL_Node *rotationPoint); 
	void RR_Rotate(AVL_Node *rotationPoint);
	int _calculateBalanceFactor(AVL_Node *tippingNode); 
	int _getNodeHeight(AVL_Node *focusNode); 
	void _nodeReader(); 
};

