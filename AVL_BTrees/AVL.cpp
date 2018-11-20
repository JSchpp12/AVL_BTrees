#include "pch.h"
#include "AVL.h"
#include <io.h>
#include <string.h>


AVL::AVL()
{
	rootNode = nullptr; 
}

void AVL::Insert(char in_key[])
{
	AVL_Node *leaderNode, *laggerNode, *lastOutOfSpec; 

	lastOutOfSpec = nullptr; 
	leaderNode = rootNode;
	laggerNode = nullptr; 

	while (leaderNode != nullptr)
	{
		laggerNode = leaderNode;
		if (leaderNode->BF != 0) lastOutOfSpec = leaderNode; //store last node whose BF != 0

		if (strcmp(leaderNode->key, in_key) == 0)
		{
			leaderNode->counter++;
			return;
		}
		else if (strcmp(leaderNode->key, in_key) > 0)
		{
			leaderNode = leaderNode->leftChild;
		}
		else leaderNode = leaderNode->rightChild;
	}
	//hit leaf position, lagger node is connection point
	//create new node 

	AVL_Node newNode;
	newNode.counter = 1;
	strcpy(newNode.key, in_key);

	if (laggerNode == nullptr) rootNode = &newNode; 
	else if (strcmp(laggerNode->key, in_key) > 0) laggerNode->leftChild = &newNode;  //new node is left child
	else laggerNode->rightChild = &newNode; 

	if (lastOutOfSpec != nullptr)
	{

	}
}