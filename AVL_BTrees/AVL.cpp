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
	AVL_Node *leaderNode, *laggerNode, *lastOutOfSpec, *parentOfLastOutSpec; 
	int displacement; 

	lastOutOfSpec = rootNode;
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
				if (leaderNode->BF != 0)
				{
					//store last node whose BF != 0 and its parent 
					lastOutOfSpec = leaderNode; 
					parentOfLastOutSpec = laggerNode; 
				}
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
		AVL_Node* correctionTracker = nullptr;  //use this pointer to correct BFs

		AVL_Node *newNode = new AVL_Node();
		newNode->counter = 1;
		strcpy(newNode->key, in_key);

		if (laggerNode == nullptr) rootNode = newNode;
		else if (strcmp(laggerNode->key, in_key) > 0) laggerNode->leftChild = newNode;  //new node is left child
		else laggerNode->rightChild = newNode;

		//correct BFs after the insert 
		if (lastOutOfSpec != nullptr)
		{
			if (strcmp(in_key, lastOutOfSpec->key) > 0)
			{
				displacement = -1; 
				correctionTracker = lastOutOfSpec->rightChild; 
			}
			else
			{
				displacement = 1; 
				correctionTracker = lastOutOfSpec->leftChild; 
			}

			while (correctionTracker != newNode)
			{
				if (strcmp(in_key, lastOutOfSpec->key) > 0)
				{
					correctionTracker->BF = -1; 
					correctionTracker = correctionTracker->rightChild; 
				}
				else
				{
					correctionTracker->BF = 1; 
					correctionTracker = correctionTracker->leftChild; 
				}
			}

			//adjust BF of first node that previously had a BF that was + or - 1
			if (lastOutOfSpec->BF == 0)
			{
				//only slightly out of balance but acceptable
				lastOutOfSpec->BF = displacement; 
				return; 
			}
			if (lastOutOfSpec->BF == -displacement)
			{
				//insert returned tree to balance
				lastOutOfSpec->BF = 0; 
				return; 
			}

			//if neither of the two returns above are hit, the tree is now not balanced 
			if (displacement == 1)
			{
				if (lastOutOfSpec->leftChild->BF == 1)
				{
					//LL Rotation
					//change child pointers of lastOutOfSpec and leftChild to reflect rotation
					LL_Rotate(parentOfLastOutSpec); 
				}
				else
				{
					//LR Rotation

				}
			}
		}
	}
}

void AVL::LL_Rotate(AVL_Node *rotationPoint, AVL_Node *parent)
{
	//AVL_Node *movingSubTree = rotationPoint->leftChild->rightChild; 
	//set new rootnode if rotation is occuring at root of tree
	if (rotationPoint == rootNode) rootNode = rotationPoint->leftChild;
	else if (parent->leftChild == rotationPoint) parent->leftChild = rotationPoint->leftChild; 
	else parent->rightChild = rotationPoint->leftChild; 
	
}

void AVL::RR_Rotate()
{

}