#include "pch.h"
#include "AVL.h"
#include <io.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std; 

AVL::AVL()
{
	AVL_Node blank; 

	remove("storage.txt"); //delete old storage file

	//will write the 0 node, used as NULL -- might not need to do this
	_nodeWriter(blank);

	delete rootNode; 
}

void AVL::Insert(char in_key[])
{
	AVL_Node *leaderNode, *laggerNode, *lastOutOfSpec, *parentOfLastOutSpec;
	int displacement;

	AVL_Node newLeader, newLagger; 

	//numbers for reading
	int numParentLastOutSpec, numOfLastOutSpec; 

	//convert in_key to lowercase for debugging purposes 
	if (in_key[0] < 90 && in_key[0] >= 65) in_key[0] = in_key[0] + 32;

	if (strcmp(in_key, "the") == 0)
	{
		std::cout << "FOCUS"; 
	}

	std::cout << "Inserting " << in_key << "\n"; 
	lastOutOfSpec = rootNode;
	leaderNode = rootNode;

	//read the rootNode from the file
	_nodeReader(numRootNode); 
	numOfLastOutSpec = numRootNode;
	newLeader = newLagger = readReturn;  //set leader and lagger nodes as the rootNode
	delete &readReturn; 

	numParentLastOutSpec = 0; //set parentLastOutSpec to NULL

	//tree is empty 
	//if (rootNode == nullptr)
	if (writeIndex == 0)
	{
		//AVL_Node* newNode = new AVL_Node();
		AVL_Node newNode; 
		std::strcpy(newNode.key, in_key);
		//newNode->leftChild = nullptr;
		//newNode->rightChild = nullptr;
		newNode.numRightChild = 0; 
		newNode.numLeftChild = 0; 
		newNode.fileIndex = 1; 
		newNode.BF = 0;
		newNode.counter = 1;
		numRootNode = 1; 

		//rootNode = newNode; //remove this 

		//write node to the file 
		_nodeWriter(newNode); 

		_nodeReader(1); 
		std::cout << "returned key " << readReturn.key; 
		return;
	}
	//ONLY NEWLEADER and NEWLAGGER should be in memory during this process
	else
	{
		do
		{
			char newString[50]; //blank for comparison
			if (strcmp(newLeader.key, newString) != 0)
			{
				if (newLeader.BF != 0)
				{
					//store last node whose BF != 0 and its parent 
					lastOutOfSpec = leaderNode; 
					parentOfLastOutSpec = laggerNode;

					
					numOfLastOutSpec = leaderNode->fileIndex;
					numParentLastOutSpec = laggerNode->fileIndex; 
				}

				//move laggerNode up to leaderNode
				laggerNode = leaderNode;

				if (strcmp(leaderNode->key, in_key) == 0)
				{
					leaderNode->counter++;
					//_nodeWriter(leaderNode); 

					//delet the nodes from memory 
					delete leaderNode, laggerNode, lastOutOfSpec; 

					return;
				}
				else if (strcmp(leaderNode->key, in_key) > 0)
				{
					if (leaderNode->leftChild != nullptr)
					{
						int tempFileIndex = leaderNode->numLeftChild;
						delete leaderNode, returnedNode; 

						//set the leaderNode as the leftChild
						_nodeReader(tempFileIndex); 
						leaderNode = returnedNode; 

						leaderNode = leaderNode->leftChild; //remove later --
					}
					else leaderNode = nullptr;
				}
				else if (leaderNode->rightChild != nullptr)
				{
					int tempFileIndex = leaderNode->numLeftChild; 
					delete leaderNode, returnedNode; 

					//set the leaderNode as the rightChild 
					_nodeReader(tempFileIndex); 
					leaderNode = returnedNode; 

					leaderNode = leaderNode->rightChild; //remove later --
				}
				else
				{
					delete leaderNode; //delete the previous node from memory 
					leaderNode = nullptr;
				}
			}
			else break;
		} while (leaderNode != nullptr);
		//hit leaf position, lagger node is connection point

		//UP UNTIL THIS POINT, ONLY LEADER AND LAGGER NODES WILL BE IN MEMORY, NEWNODE WILL BE 3RD IN MEMORY

		//create new node 
		AVL_Node* correctionTracker = nullptr;  //use this pointer to correct BFs

		AVL_Node *newNode = new AVL_Node();
		newNode->counter = 1;
		newNode->fileIndex = writeIndex; 
		std::strcpy(newNode->key, in_key);

		writeIndex++;

		//set laggerNode's child pointer to the newNode
		if (laggerNode == nullptr)
		{
			rootNode = newNode; //remove

			numRootNode = newNode->fileIndex; 
		}
		else if (strcmp(laggerNode->key, in_key) > 0)
		{
			laggerNode->leftChild = newNode;  //new node is left child
			
			laggerNode->numLeftChild = newNode->fileIndex; 
		}
		else
		{
			laggerNode->rightChild = newNode;

			laggerNode->numRightChild = newNode->fileIndex; 
		}

		//write nodes to file 
		//_nodeWriter(laggerNode); 
		//_nodeWriter(newNode); 

		delete laggerNode, newNode; //clear the memory 

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
					
					//find what side to put the new root node on the parent node 
					if (lastOutOfSpec == rootNode) rootNode = returnedNode;
					else if (strcmp(returnedNode->key, parentOfLastOutSpec->key) > 0) parentOfLastOutSpec->rightChild = returnedNode;
					else parentOfLastOutSpec->leftChild = returnedNode;
				}
				else
				{
					//LR Rotation -- RR Rotation and then a LL rotation
					std::cout << "LR\n";

					//determine which side of the tree the insertion occured on 
					bool leftChild;
					if ((parentOfLastOutSpec) && (lastOutOfSpec == parentOfLastOutSpec->leftChild)) leftChild = true;
					else leftChild = false;

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

					//find what side to put the new root node on the parent node
					if (lastOutOfSpec == rootNode) rootNode = returnedNode;
					else if (strcmp(returnedNode->key, parentOfLastOutSpec->key) > 0) parentOfLastOutSpec->rightChild = returnedNode;
					else parentOfLastOutSpec->leftChild = returnedNode;
				}
				else
				{
					//determine which side of the tree the insertion occured on 
					bool leftChild;
					if ((parentOfLastOutSpec) && (lastOutOfSpec == parentOfLastOutSpec->leftChild)) leftChild = true;
					else leftChild = false;

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

	//might need to swap these -------------------------------------------------------	
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
	std::ifstream myfile; 
	myfile.open(storageFile, ios::in | ios::binary); 

	if (myfile)
	{
		for (int j = 0; j <= index; j++)
		{
			myfile.read((char*)&readReturn, sizeof(readReturn));
			std::cout << readReturn.key; 
		}
		myfile.close(); 
	}
	else exit(1); //failed to open file
}

void AVL::_nodeWriter(AVL_Node targetNode)
{
	//write targetNode to the end of the file
	std::ofstream myfile; 
	myfile.open(storageFile, ios::app | ios::binary);

	//targetNode->fileIndex = writeIndex; 
	//writeIndex++; 

	//to overwrite a specific node, skip the ones before it by skipping size of the nodes
	myfile.seekp(sizeof(targetNode) * targetNode.fileIndex); 

	myfile.write((char*)&targetNode, sizeof(targetNode)); 

	myfile.close(); 
}