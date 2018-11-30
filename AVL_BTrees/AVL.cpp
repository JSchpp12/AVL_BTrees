#include "pch.h"
#include "AVL.h"
#include <io.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

fstream file;

AVL::AVL()
{
	//FIND NEW WAY OF WRITING A BLANK NODE TO THE FILE
	/*
	AVL_Node blank;
	blank.BF = 123;
	blank.fileIndex = 0;
	blank.numLeftChild = 0;
	blank.numRightChild = 0;
	blank.counter = 0;
	strcpy(blank.key, "NULLNODE");
	*/
	if (remove("storage.txt") != 0)
	{
		std::cout << "cannot delete";
	}
	if (file.fail())
	{
		std::cout << "Filed to open file \n";
	}
	if (!file || !file.good())
	{
		std::cout << "could not open file!\n";
		return;
	}
	//file.close(); 
	//file.open(storageFile, ios::out | ios::in | ios::binary); 
	if (!file.is_open())  exit(1);

}

AVL::~AVL()
{
	file.close();
}

void AVL::Read()
{
	_readFile();
}

void AVL::Insert(char in_key[])
{
	AVL_Node *leaderNode, *laggerNode, *lastOutOfSpec, *parentOfLastOutSpec;
	int displacement;


	bool leaderNodeExsist = true; //use this to exit insertion loop
	bool laggerNodeExsist;
	AVL_Node A, B, C;

	file.clear(); //reset eof and fail flags

	//numbers for reading
	int numParentLastOutSpec, numOfLastOutSpec;

	//convert in_key to lowercase for debugging purposes 
	if (in_key[0] < 90 && in_key[0] >= 65) in_key[0] = in_key[0] + 32;

	if (strcmp(in_key, "the") == 0)
	{
		std::cout << "FOCUS";
	}

	std::cout << "Inserting " << in_key << "\n";

	//read the rootNode from the file
	if (numRootNode != 0)
	{
		_nodeReader(numRootNode, &A);
		B = A; //set leader and lagger nodes as the rootNode
		numOfLastOutSpec = numRootNode;
	}

	numParentLastOutSpec = 0; //set both last out of spec to NULL

	//tree is empty 
	//if (rootNode == nullptr)
	if (writeIndex == 0)
	{
		//AVL_Node* newNode = new AVL_Node();
		std::strcpy(C.key, in_key);
		//newNode->leftChild = nullptr;
		//newNode->rightChild = nullptr;
		C.numRightChild = 0;
		C.numLeftChild = 0;
		C.fileIndex = 1;
		C.BF = 0;
		C.counter = 1;
		writeIndex = 2;
		numRootNode = 1;

		//rootNode = newNode; //remove this 

		//write node to the file 
		_nodeWriter(&C);
		//delete &A; 
		return;
	}
	//ONLY A and B should be in memory during this process
	else
	{
		do
		{
			char newString[50]; //blank for comparison
			if (strcmp(A.key, newString) != 0)
			{
				if (A.BF != 0)
				{
					//these are for memory restriction
					numOfLastOutSpec = A.fileIndex;
					numParentLastOutSpec = B.fileIndex;
				}

				//move laggerNode up to leaderNode 
				B = A;

				//if (strcmp(leaderNode->key, in_key) == 0)
				if (strcmp(A.key, in_key) == 0)
				{
					//leaderNode->counter++;
					A.counter++;

					_nodeWriter(&A); //update A 

					leaderNodeExsist = false; //continue loop

					return;
				}
				//else if (strcmp(leaderNode->key, in_key) > 0)
				else if (strcmp(A.key, in_key) > 0)
				{
					//if (leaderNode->leftChild != nullptr)
					if (A.numLeftChild != 0)
					{
						//set the leaderNode as the leftChild
						_nodeReader(A.numLeftChild, &A);
						leaderNodeExsist = true; //continue loop
					}
					//else leaderNode = nullptr;
					else leaderNodeExsist = false;
				}
				//else if (leaderNode->rightChild != nullptr)
				else if (A.numRightChild != 0)
				{
					//set the leaderNode as the rightChild 
					_nodeReader(A.numRightChild, &A);
					leaderNodeExsist = true;
				}
				else
				{
					//delete A; //delete the previous node from memory 
					leaderNode = nullptr;

					leaderNodeExsist = false; //exit the loop - hit a leaf
				}
			}
			else break;
		} while (leaderNodeExsist != false);
		//hit leaf position, lagger node is connection point

		//UP UNTIL THIS POINT, ONLY LEADER AND LAGGER NODES WILL BE IN MEMORY, NEWNODE WILL BE 3RD IN MEMORY

		//create new node 
		int correctionTracker = 0;  //use this pointer to correct BFs

		int newNodeIndex = writeIndex; //save index where new leaf is written

		//C is set as the newNode
		C.counter = 1;
		C.BF = 0;
		C.fileIndex = writeIndex;
		C.numLeftChild = 0;
		C.numRightChild = 0;
		std::strcpy(C.key, in_key);

		_nodeWriter(&C);  //write newNode to file for storage 
		writeIndex++;

		if (strcmp(B.key, in_key) > 0)
		{
			//set laggerNodes new child
			B.numLeftChild = C.fileIndex;
			_nodeWriter(&B);
		}
		else
		{
			//set laggerNode's new child
			B.numRightChild = C.fileIndex;
			_nodeWriter(&B); //This breaks file ----------------------------------------------------------------------------------------
			//_readFile(); 
		}

		//correct BFs after the insert 
		//if (lastOutOfSpec != nullptr)
		if (numOfLastOutSpec != 0)
		{
			//A is set as lastOutOfSpec
			_nodeReader(numOfLastOutSpec, &A);

			if (strcmp(in_key, A.key) > 0)
			{
				displacement = -1;
				correctionTracker = A.numRightChild;
			}
			else
			{
				displacement = 1;
				correctionTracker = A.numLeftChild;
			}
			//WHEN inserting n, y is read in here and it does not have the correct BF - it is read in as 204
			_nodeReader(correctionTracker, &A); //read in correctionTracker -> A
			if ((A.BF > 3 || A.BF < -3))
			{
				int tempStorage = A.fileIndex;
				int tempBF = _calculateBalanceFactor(&A);
				_nodeReader(tempStorage, &A);
				A.BF = tempBF;
				_nodeWriter(&A);
			}
			while (correctionTracker != newNodeIndex)
			{
				if (strcmp(in_key, A.key) > 0)
				{
					int childIndex = A.numRightChild;
					A.BF = -1;
					_nodeWriter(&A);
					//might need to check if child index is null -- NEED TO DO THIS ACTUALLY
					_nodeReader(childIndex, &A);

					correctionTracker = childIndex;
				}
				else
				{
					int childIndex = A.numLeftChild;
					A.BF = 1;
					_nodeWriter(&A);

					_nodeReader(childIndex, &A); //read in child node
					correctionTracker = childIndex; //set correctionTracker to tell when hit newNode
				}
			}

			//A will be newNode at this point 

			_nodeReader(numOfLastOutSpec, &A); 	//read in lastOutOfSpec -> A 

			//adjust BF of first node that previously had a BF that was + or - 1
			if (A.BF == 0)
			{
				//only slightly out of balance but acceptable
				A.BF = displacement;
				_nodeWriter(&A);

				return;
			}
			if (A.BF == -displacement)
			{
				//insert returned tree to balance
				A.BF = 0;
				_nodeWriter(&A);

				return;
			}

			//if neither of the two returns above are hit, the tree is not balanced 
			if (displacement == 1)
			{
				_nodeReader(A.numLeftChild, &C);
				if (C.BF == 1)
				{
					//A will be rotationCenter -- return root of rotated subtree in B

					LL_Rotate(&A, &B, &C);

					_nodeReader(numParentLastOutSpec, &C);

					//find what side to put the new root node on the parent node 
					if (numOfLastOutSpec == numRootNode) numRootNode = B.fileIndex;
					else if (strcmp(B.key, C.key) > 0)
					{
						C.numRightChild = B.fileIndex;
						_nodeWriter(&C); //update C
					}
					else
					{
						C.numLeftChild = B.fileIndex;
						_nodeWriter(&C);
					}
					//B = C;
					//C.BF = _calculateBalanceFactor(&B);
					//_nodeWriter(&C);
				}
				else
				{
					//LR Rotation -- RR Rotation and then a LL rotation
					std::cout << "LR\n";

					//determine which side of the tree the insertion occured on -- only need A = lastOutOfSpec

					_nodeReader(numParentLastOutSpec, &B);

					bool leftChild;
					if ((numParentLastOutSpec != 0) && (numOfLastOutSpec == B.numLeftChild)) leftChild = true;
					else leftChild = false;

					int numLast = A.fileIndex;
					int numLast_leftChild = A.numLeftChild;
					int numLast_rightChild = A.numRightChild;

					_nodeReader(numLast_leftChild, &A);
					RR_Rotate(&A, &B, &C);
					_nodeReader(numOfLastOutSpec, &A); //read in last out of spec into A

					A.numLeftChild = B.fileIndex;
					_nodeWriter(&A);

					//need to read in lastOutOfSpec again 

					LL_Rotate(&A, &B, &C);

					_nodeReader(numParentLastOutSpec, &C); //read in parent of last out of spec into C

					if (numOfLastOutSpec == numRootNode) numRootNode = B.fileIndex;
					else if (strcmp(B.key, C.key) > 0)
					{
						C.numRightChild = B.fileIndex;
						_nodeWriter(&C); //update C
					}
					else
					{
						C.numLeftChild = B.fileIndex;
						_nodeWriter(&C);
					}
					//B = C;
					//C.BF = _calculateBalanceFactor(&B);
					//_nodeWriter(&C); 
				}
			}
			else if (displacement == -1)
			{
				_nodeReader(A.numRightChild, &C);
				if (C.BF == -1)
				{
					//call this method to set returnedNode with the root of the rotated subtree
					RR_Rotate(&A, &B, &C);

					_nodeReader(numParentLastOutSpec, &C);
					if (numOfLastOutSpec == numRootNode) numRootNode = B.fileIndex;
					else if (strcmp(B.key, C.key) > 0)
					{
						C.numRightChild = B.fileIndex;
						_nodeWriter(&C); //update C
					}
					else
					{
						C.numLeftChild = B.fileIndex;
						_nodeWriter(&C);
					}
				}
				else
				{
					_nodeReader(numParentLastOutSpec, &B);
					//determine which side of the tree the insertion occured on 
					bool leftChild;
					if ((numParentLastOutSpec != 0) && (A.fileIndex == B.numLeftChild)) leftChild = true;
					else leftChild = false;

					std::cout << "RL rotation\n";

					int numLast = A.fileIndex;
					int numLast_leftChild = A.numLeftChild;
					int numLast_rightChild = A.numRightChild;

					_nodeReader(numLast_rightChild, &A);
					LL_Rotate(&A, &B, &C);
					//set lastOutOfSpec child
					_nodeReader(numOfLastOutSpec, &A);
					A.numRightChild = B.fileIndex;
					_nodeWriter(&A);

					RR_Rotate(&A, &B, &C);

					_nodeReader(numParentLastOutSpec, &C); //read in parent of last out of spec into C

					if (numOfLastOutSpec == numRootNode) numRootNode = B.fileIndex;
					else if (strcmp(B.key, C.key) > 0)
					{
						C.numRightChild = B.fileIndex;
						_nodeWriter(&C); //update C
					}
					else
					{
						C.numLeftChild = B.fileIndex;
						_nodeWriter(&C);
					}
				}
			}
		}
	}
	//_readFile(); 
}

void AVL::LL_Rotate(AVL_Node *rotationPoint, AVL_Node *B, AVL_Node *C)
{
	//pass in the 3 nodes that will be used for memory
	//B will be empty 
	//C will be parent of rotated tree
	//A = rotationPoint

	std::cout << "LL Rotate \n";
	//AVL_Node *storage = rotationPoint->leftChild->rightChild;
	//AVL_Node *newRoot = rotationPoint->leftChild;

	_nodeReader(rotationPoint->numLeftChild, B); //B is now newRoot
	int numStorage = B->numRightChild; //save storage

	//do actual rotation
	B->numRightChild = rotationPoint->fileIndex;
	rotationPoint->numLeftChild = numStorage;

	//save these after rotation
	_nodeWriter(rotationPoint);
	_nodeWriter(B);

	//B->rightChild = rotationPOint
	//B->leftChild = -- unchanged --

	//Update BFs
	rotationPoint = B;
	B->BF = _calculateBalanceFactor(rotationPoint);
	_nodeWriter(B); //save B

	if (B->numLeftChild != 0)
	{
		//ONLY NEED B RIGHT NOW
		//read the child into C --this will overwright grandparent 
		_nodeReader(B->numLeftChild, C);
		rotationPoint = C;

		C->BF = _calculateBalanceFactor(rotationPoint);

		_nodeWriter(C); //save C
	}
	if (B->numRightChild != 0)
	{
		_nodeReader(B->numRightChild, C);
		rotationPoint = C;

		C->BF = _calculateBalanceFactor(rotationPoint);

		_nodeWriter(C); //save C
	}
	//B will be newRoot -- handle after calling this method 
}

void AVL::RR_Rotate(AVL_Node *rotationPoint, AVL_Node *B, AVL_Node *C)
{
	//NEW ROOT LEFTCHILD NOT BEGIN SET
	std::cout << "RR Rotation\n";

	_nodeReader(rotationPoint->numRightChild, B); //B is now newRoot
	int numStorage = B->numLeftChild; //save storage

	//actual rotation 
	B->numLeftChild = rotationPoint->fileIndex;
	rotationPoint->numRightChild = numStorage;

	//save these after rotation
	_nodeWriter(rotationPoint);
	_nodeWriter(B);
	//B->rightChild = rotationPOint
	//B->leftChild = -- unchanged --

	//Update BFs
	rotationPoint = B;
	B->BF = _calculateBalanceFactor(rotationPoint);
	_nodeWriter(B); //save B

	//update BFs
	if (B->numLeftChild != 0)
	{
		//ONLY NEED B RIGHT NOW
		//read the child into C --this will overwright grandparent 
		_nodeReader(B->numLeftChild, C);
		rotationPoint = C;

		C->BF = _calculateBalanceFactor(rotationPoint);

		_nodeWriter(C); //save C
	}
	if (B->numRightChild != 0)
	{
		_nodeReader(B->numRightChild, C);
		rotationPoint = C;

		C->BF = _calculateBalanceFactor(rotationPoint);

		_nodeWriter(C); //save C
	}
	//B will be newRoot -- handle after calling this method 
}

int AVL::_calculateBalanceFactor(AVL_Node* tippingNode)
{
	int rightHeight, leftHeight, storedNode;
	storedNode = tippingNode->fileIndex;

	if (tippingNode->numRightChild != 0)
	{
		_nodeReader(tippingNode->numRightChild, tippingNode);
		rightHeight = (_getNodeHeight(tippingNode) + 1);
	}
	else rightHeight = 0;

	_nodeReader(storedNode, tippingNode); //back to stored node 

	if (tippingNode->numLeftChild != 0)
	{
		_nodeReader(tippingNode->numLeftChild, tippingNode);
		leftHeight = (_getNodeHeight(tippingNode) + 1);
	}
	else leftHeight = 0;

	_nodeReader(storedNode, tippingNode); //set back

	//_getNodeHeight(tippingNode); //not sure why this is here -- mistake??
	return (leftHeight - rightHeight);
}

int AVL::_getNodeHeight(AVL_Node* focusNode)
{
	//WILL OVERWRIGHT WHATEVER NODE IS PASSED INTO THIS METHOD
	int ret1, ret2;
	int savedNode = focusNode->fileIndex;

	//calculate the height of the node 
	if ((focusNode->numRightChild == 0) && (focusNode->numLeftChild == 0))
	{
		return 0;
	}
	else
	{
		if (focusNode->numRightChild != 0)
		{
			_nodeReader(focusNode->numRightChild, focusNode);
			ret1 = _getNodeHeight(focusNode);
		}
		else ret1 = 0;

		_nodeReader(savedNode, focusNode); //set back to origional node

		if (focusNode->numLeftChild != 0)
		{
			_nodeReader(focusNode->numLeftChild, focusNode);

			ret2 = _getNodeHeight(focusNode);
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

void AVL::_nodeReader(int index, AVL_Node* returnedNode)
{
	AVL_Node newNode;

	//std::ifstream myfile; 
	//myfile.open(storageFile, ios::in | ios::binary); 
	int position((index - 1) * sizeof(AVL_Node));
	file.seekg(position);
	file.read((char*)returnedNode, sizeof(AVL_Node));
	//file.flush(); 
	//_readFile(); 
}
//else exit(1); //failed to open file

void AVL::_nodeWriter(AVL_Node *targetNode)
{
	int size = sizeof(AVL_Node);
	int position = ((targetNode->fileIndex - 1) * sizeof(AVL_Node));
	//std::ofstream myfile; 
	//myfile.open(storageFile, ios::binary);
	//to overwrite a specific node, skip the ones before it by skipping size of the nodes
	file.seekp(position, ios::beg);
	file.write((char*)targetNode, sizeof(AVL_Node));
	//file << "WOOEWFIJWEOEFW"; 
	file.flush();
	//_readFile(); 
//_readFile(); 
}

void AVL::_readFile()
{
	AVL_Node newNode;
	std::ifstream myfile;
	//myfile.open(storageFile, ios::in | ios::binary);
	file.seekg(0, ios::beg);

	std::cout << "beginning read WHOLE FILE\n";

	while (!file.eof())
	{
		strcpy(newNode.key, "NULL");
		file.read((char*)&newNode, sizeof(AVL_Node));
		std::cout << "key: " << newNode.key << "\n";
		std::cout << "BF : " << newNode.BF << "\n";
	}
	file.clear();
}