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
	int displacement; 

	lastOutOfSpec = nullptr;
	leaderNode = rootNode;
	laggerNode = nullptr;

	//tree is empty 
	if (rootNode == nullptr)
	{
		AVL_Node* newNode = new AVL_Node();
		strcpy(newNode->key, in_key);
		newNode->leftChild = nullptr;
		newNode->rightChild = nullptr; 
		newNode->BF = 0;
		newNode->counter = 1;
		rootNode = newNode;
		return;
	}
	else
	{
		do
		{
			if (leaderNode != nullptr)
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
					if (leaderNode->rightChild != nullptr) leaderNode = leaderNode->leftChild;
					else leaderNode = nullptr; 
				}
				else if (leaderNode->rightChild != nullptr)leaderNode = leaderNode->rightChild;
				else leaderNode = nullptr; 
			}
			else break; 
		} while (leaderNode != nullptr);
		//hit leaf position, lagger node is connection point
		//create new node 

		AVL_Node *newNode = new AVL_Node();
		newNode->counter = 1;
		strcpy(newNode->key, in_key);

		if (laggerNode == nullptr) rootNode = newNode;
		else if (strcmp(laggerNode->key, in_key) > 0) laggerNode->leftChild = newNode;  //new node is left child
		else laggerNode->rightChild = newNode;

		if (lastOutOfSpec != nullptr)
		{
			if (strcmp(in_key, lastOutOfSpec->key) > 0)
			{
				displacement = -1; 
				lastOutOfSpec = lastOutOfSpec->rightChild; 
			}
			else
			{
				displacement = 1; 
				lastOutOfSpec = lastOutOfSpec->leftChild; 
			}

			//adjust BFs
			while (lastOutOfSpec != newNode)
			{
				if (strcmp(in_key, lastOutOfSpec->key) > 0)
				{
					lastOutOfSpec->BF = -1; 
					lastOutOfSpec = lastOutOfSpec->rightChild; 
				}
				else
				{
					lastOutOfSpec->BF = 1; 
					lastOutOfSpec = lastOutOfSpec->leftChild; 
				}
			}
		}
	}
}