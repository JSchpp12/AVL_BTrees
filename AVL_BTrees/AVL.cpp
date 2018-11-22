#include "pch.h"
#include "AVL.h"
#include <io.h>
#include <iostream>
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
	parentOfLastOutSpec = laggerNode = nullptr;

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
				if (leaderNode->BF != 0)
				{
					//store last node whose BF != 0 and its parent 
					lastOutOfSpec = leaderNode; 
					parentOfLastOutSpec = laggerNode; 
				}

				//move laggerNode up to leaderNode
				laggerNode = leaderNode;

				if (strcmp(leaderNode->key, in_key) == 0)
				{
					leaderNode->counter++;
					return;
				}
				else if (strcmp(leaderNode->key, in_key) > 0)
				{
					if (leaderNode->leftChild != nullptr) leaderNode = leaderNode->leftChild;
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
				if (strcmp(in_key, correctionTracker->key) > 0)
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

			//if neither of the two returns above are hit, the tree is not balanced 
			if (displacement == 1)
			{
				if (lastOutOfSpec->leftChild->BF == 1)
				{
					//LL Rotation
					//change child pointers of lastOutOfSpec and leftChild to reflect rotation

					//TO MEET STORAGE REQUIRENMENTS ONLY FOCUS ON THE NODES DIRECTLY INVOLVED IN THE ROTATIONM, THEN CLEAR THOSE FROM MEMORY, THEN LOAD IN PARENT TO ATTACKED ROTATED TREE TOO
					std::cout << "LL Rotate \n"; 
					AVL_Node *storage = lastOutOfSpec->leftChild->rightChild;
					AVL_Node *newRoot = lastOutOfSpec->leftChild; 

					newRoot->rightChild = lastOutOfSpec; 
					lastOutOfSpec->leftChild = storage; 

					//update BFs 
					newRoot->BF = _calculateBalanceFactor(newRoot);
					newRoot->leftChild->BF = _calculateBalanceFactor(newRoot->leftChild);
					newRoot->rightChild->BF = _calculateBalanceFactor(newRoot->rightChild);

					//clear nodes out of memory here then attach to parent
					if (lastOutOfSpec == rootNode) rootNode = newRoot; 
					else parentOfLastOutSpec->leftChild = newRoot; 
				}
				else
				{
					//LR Rotation -- RR Rotation and then a LL rotation
					std::cout << "LR\n";
					//temporary fix --------- for the first rotation need to move down to child node 
					parentOfLastOutSpec = lastOutOfSpec; 
					lastOutOfSpec = lastOutOfSpec->leftChild; 

					std::cout << "RR Rotation\n";
					AVL_Node *storage = lastOutOfSpec->rightChild->leftChild;
					AVL_Node *newRoot = lastOutOfSpec->rightChild;

					newRoot->leftChild = lastOutOfSpec;
					lastOutOfSpec->rightChild = storage;

					//clear nodes out of memory here then attach to parent
					if (lastOutOfSpec == rootNode) rootNode = newRoot;
					else parentOfLastOutSpec->rightChild = newRoot;

					//LL rotate, need to clear memory and get new nodes from file ---- LATER
					std::cout << "LL Rotation\n"; 
					storage = lastOutOfSpec->leftChild->rightChild;
					newRoot = lastOutOfSpec->leftChild;

					newRoot->rightChild = lastOutOfSpec;
					lastOutOfSpec->leftChild = storage;

					//update BFs 
					newRoot->BF = _calculateBalanceFactor(newRoot);
					newRoot->leftChild->BF = _calculateBalanceFactor(newRoot->leftChild);
					newRoot->rightChild->BF = _calculateBalanceFactor(newRoot->rightChild);

					//clear nodes out of memory here then attach to parent
					if (lastOutOfSpec == rootNode) rootNode = newRoot;
					else parentOfLastOutSpec->leftChild = newRoot;
				}
			}
		else if (displacement == -1)
			{
				if (lastOutOfSpec->rightChild->BF == -1)
				{
					std::cout << "RR Rotation\n";
					AVL_Node *storage = lastOutOfSpec->rightChild->leftChild; 
					AVL_Node *newRoot = lastOutOfSpec->rightChild; 

					newRoot->leftChild = lastOutOfSpec;
					lastOutOfSpec->rightChild = storage;

					//update BFs 
					newRoot->BF = _calculateBalanceFactor(newRoot); 
					newRoot->leftChild->BF = _calculateBalanceFactor(newRoot->leftChild); 
					newRoot->rightChild->BF = _calculateBalanceFactor(newRoot->rightChild); 

					//clear nodes out of memory here then attach to parent
					if (lastOutOfSpec == rootNode) rootNode = newRoot;
					else parentOfLastOutSpec->rightChild = newRoot;
				}
				else
				{
					std::cout << "RL rotation\n"; 
				}
			}
		}
	}
}

struct AVL_Node
{
	int BF;
	int fileIndex;   //where in the file this node is written
	int counter;
	AVL_Node *leftChild = nullptr;
	AVL_Node *rightChild = nullptr;
	char key[50];
};

AVL_Node* AVL::LL_Rotate(AVL_Node *rotationPoint, AVL_Node *parent)
{

}

int AVL::_calculateBalanceFactor(AVL_Node* tippingNode)
{
	int rightHeight, leftHeight;
	if (tippingNode->rightChild != nullptr)
	{
		rightHeight = (_getNodeHeight(tippingNode->rightChild) + 1);
	}
	else rightHeight = 0;

	if (tippingNode->leftChild != nullptr)
	{
		leftHeight = (_getNodeHeight(tippingNode->leftChild) + 1);
	}
	else leftHeight = 0;

	_getNodeHeight(tippingNode);
	return (leftHeight - rightHeight);
}


int AVL::_getNodeHeight(AVL_Node* focusNode)
{
	int ret1, ret2;
	//calculate the height of the node 
	if ((focusNode->rightChild == nullptr) && (focusNode->leftChild == nullptr))
	{
		return 0;
	}
	else
	{
		if (focusNode->rightChild != nullptr)
		{
			ret1 = _getNodeHeight(focusNode->rightChild);
		}
		else ret1 = 0;

		if (focusNode->leftChild != nullptr)
		{
			ret2 = _getNodeHeight(focusNode->leftChild);
		}
		else ret2 = 0;

		if (ret1 > ret2)
		{
			return (ret1 + 1);
		}
		else
		{
			return (ret2 + 1);
		}
	}
}