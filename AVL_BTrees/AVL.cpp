#include "pch.h"
#include "AVL.h"
#include <io.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std; 

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

					LL_Rotate(lastOutOfSpec);
					if (lastOutOfSpec == rootNode) rootNode = returnedNode;
					else parentOfLastOutSpec->leftChild = returnedNode;
				}
				else
				{
					//LR Rotation -- RR Rotation and then a LL rotation
					std::cout << "LR\n";

					//determine which side of the tree the insertion occured on 
					bool leftChild;
					if ((parentOfLastOutSpec) && (lastOutOfSpec == parentOfLastOutSpec->leftChild)) leftChild = true;
					else false;

					RR_Rotate(lastOutOfSpec->leftChild);
					lastOutOfSpec->leftChild = returnedNode;

					LL_Rotate(lastOutOfSpec);

					if (lastOutOfSpec == rootNode) rootNode = returnedNode;
					else if (leftChild == true) parentOfLastOutSpec->leftChild = returnedNode;
					else if (leftChild == false) parentOfLastOutSpec->rightChild = returnedNode;
				}
			}
			else if (displacement == -1)
			{
				if (lastOutOfSpec->rightChild->BF == -1)
				{
					//call this method to set returnedNode with the root of the rotated subtree
					RR_Rotate(lastOutOfSpec);

					if (lastOutOfSpec == rootNode) rootNode = returnedNode;
					else parentOfLastOutSpec->rightChild = returnedNode;
				}
				else
				{
					//determine which side of the tree the insertion occured on 
					bool leftChild;
					if ((parentOfLastOutSpec) && (lastOutOfSpec == parentOfLastOutSpec->leftChild)) leftChild = true;
					else false;

					std::cout << "RL rotation\n";
					LL_Rotate(lastOutOfSpec->rightChild);
					lastOutOfSpec->rightChild = returnedNode;

					RR_Rotate(lastOutOfSpec);
					if (lastOutOfSpec == rootNode) rootNode = returnedNode;
					else if (leftChild == true) parentOfLastOutSpec->leftChild = returnedNode;
					else if (leftChild == false) parentOfLastOutSpec->rightChild = returnedNode;
				}
			}
		}
	}
}

void AVL::LL_Rotate(AVL_Node *rotationPoint)
{
	returnedNode = nullptr; //reset return node so that this can be set 

	std::cout << "LL Rotate \n";
	AVL_Node *storage = rotationPoint->leftChild->rightChild;
	AVL_Node *newRoot = rotationPoint->leftChild;

	newRoot->rightChild = rotationPoint;
	rotationPoint->leftChild = storage;

	//update BFs 
	newRoot->BF = _calculateBalanceFactor(newRoot);
	if (newRoot->leftChild) newRoot->leftChild->BF = _calculateBalanceFactor(newRoot->leftChild);
	if (newRoot->rightChild) newRoot->rightChild->BF = _calculateBalanceFactor(newRoot->rightChild);

	//clear nodes out of memory here then attach to parent
	returnedNode = newRoot;
}

void AVL::RR_Rotate(AVL_Node *rotationPoint)
{
	returnedNode = nullptr;
	std::cout << "RR Rotation\n";
	AVL_Node *storage = rotationPoint->rightChild->leftChild;
	AVL_Node *newRoot = rotationPoint->rightChild;

	newRoot->leftChild = rotationPoint;
	rotationPoint->rightChild = storage;

	//update BFs 
	newRoot->BF = _calculateBalanceFactor(newRoot);
	newRoot->leftChild->BF = _calculateBalanceFactor(newRoot->leftChild);
	if (newRoot->rightChild)newRoot->rightChild->BF = _calculateBalanceFactor(newRoot->rightChild);

	//clear nodes out of memory here then attach to parent

	//if (rotationPoint == rootNode) rootNode = newRoot;
	//else parentOfLastOutSpec->rightChild = newRoot;
	returnedNode = newRoot;
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

void AVL::_nodeReader(int index)
{
	std::ofstream myfile; 
	myfile.open(storageFile, ios::out | ios::binary); 


}

void AVL::_nodeWriter()
{

}