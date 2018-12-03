#include "pch.h"
#include "B_Tree.h"
#include <string.h>

std::fstream Bfile; 

B_Tree::B_Tree()
{
	//create B_Node stuff
	Bfile.open(storageFile, fstream::in | fstream::out | fstream::trunc | fstream::binary);
	
	numDiskReads = 0; 
	numDiskWrites = 0; 
}

void B_Tree::GetInfo()
{
	double value = ((writeIndex / numKeys) * 100); 
	std::cout << "B_Tree Information: \n";
	std::cout << "Number of Words: " << numKeys << "\n";
	std::cout << "Number of Reads: " << numDiskReads << "\n";
	std::cout << "Number of Writes: " << numDiskWrites << "\n";
	std::cout << "Total Number of Nodes: " << writeIndex << "\n";
	std::cout << "File Size: " << _getFileSize() << "\n";
	std::cout << "Loading Factor: " << value << "\n"; 
}

void B_Tree::Insert(char in_key[])
{
	
	BNode X, Y, Z; 
	SearchReturn searchResults; 

	numKeys++; 

	if (numRootNode == 0)
	{
		//create root node
		strcpy(X.key[1], in_key); 
		X.numKeys = 1; 
		X.counter[1] = 1; 
		X.fileIndex = writeIndex; 
		X.isleaf = true; 
		for (int i = 1; i < X.maxNumChildren; i++)
		{
			//set all children pointers to null
			X.child[i] = 0; 
		}
		_writeFile(&X); 

		numRootNode = X.fileIndex; 
		writeIndex++; 
		return; 
	}

	//read in root node 
	_readFile(numRootNode, &X); 

	_search(&X, in_key, &searchResults); //search for key

	if (searchResults.foundExact == true)
	{
		//found duplicate -- increment counter
		X.counter[searchResults.keyLocation]++; 
		_writeFile(&X); 
		return; 
	}

	_readFile(numRootNode, &X); 
	if (X.numKeys == X.maxNumKeys)
	{
		//will need to split the node since it is full
		//set up new node that will become the node
		Y.fileIndex = writeIndex;
		writeIndex++;
		Y.numKeys = 0; 
		Y.isleaf = false; 
		Y.child[1] = numRootNode; 
		Y.numChildren++; 
		numRootNode = Y.fileIndex; 

		_splitChild(&Y, &X, &Z, 1);
		//_splitChild(&X, &Y, &Z, 0); 
		_insertNonFull(in_key, &Y, &X, &Z); 
	}
	else
	{
		_insertNonFull(in_key, &X, &Y, &Z); 
	}
}

//private

//node x will be result -- y will be start point
void B_Tree::_search(BNode *X, char in_key[], SearchReturn *returnInfo)
{
	bool found = false;
	int i = 1;
	while ((i <= X->numKeys) && (strcmp(in_key, X->key[i]) > 0)) i++;

	//If found key is the same as input, return node
	if (i <= X->numKeys)
	{
		if (strcmp(in_key, X->key[i]) == 0)
		{
			returnInfo->keyLocation = i;
			returnInfo->nodeLocation = X->fileIndex;
			returnInfo->foundExact = true;
			return;
		}
	}

	if (X->isleaf == true)
	{
		returnInfo->keyLocation = 0;
		returnInfo->nodeLocation = 0; 
		return; 
	}

	_readFile(X->child[i], X);
	return _search(X, in_key, returnInfo); 
}

int B_Tree::_getHeight(BNode *currentNode, int currentHeight)
{
	int saveNode = currentNode->fileIndex; //save node to come back to

	if (currentNode->isleaf) return currentHeight; //at bottom of tree

	for (int i = 1; i <= currentNode->numChildren; i++)
	{
		_readFile(currentNode->child[i], currentNode); 
		int childHeight = _getHeight(currentNode, currentHeight + 1); 
		_readFile(saveNode, currentNode); //return to previous node
		if (childHeight > currentHeight) currentHeight = childHeight; 
	}
	return currentHeight; 
}

void B_Tree::_traverse(BNode *currentNode)
{
	if (numRootNode == 0)
	{
		std::cout << "Tree is empty \n"; //tree is empty
		return; 
	}
	if (currentNode->numKeys != 0)
	{
		for (int i = 0; i <= currentNode->numKeys; i++)
		{
			int saveCurrent = currentNode->fileIndex; 

			if (currentNode->child[i] != 0)
			{
				//traverse into child node
				_readFile(currentNode->child[i], currentNode); //read in child node
				_traverse(currentNode); 
				_readFile(saveCurrent, currentNode); //read in previous node
			}
			std::cout << currentNode->key[i]; 
		}
	}
}

void B_Tree::_insertNonFull(char in_key[], BNode *X, BNode *Y, BNode *Z)
{
	//pass in targetNode in X
	int i = X->numKeys; 

	if (X->isleaf == true)
	{
		//alter to fit 1 based arrays instead of 0 based
		while (i >= 1 && (strcmp(X->key[i], in_key) > 0))
		{
			strcpy(X->key[i + 1], X->key[i]); 
			X->counter[i + 1] = X->counter[i]; 
			X->counter[i] = 1; 
			i--; 
		}
		strcpy(X->key[i + 1], in_key); 
		X->counter[i + 1] = 1; 
		X->numKeys++; 

		_writeFile(X); 
	}
	else
	{
		//while ((i >= 1) && (strcmp(X->key[i], in_key) > 0))
		while ((i >= 1) && (strcmp(in_key, X->key[i]) < 0))
		{
			i--; 
		}
		i++; //can get too low -- crash 

		_readFile(X->child[i], Y); //read in child node
		
		if (Y->numKeys == Y->maxNumKeys)
		{
			//need to pass in the correct node, remember - Y is targetnode
			_splitChild(X, Y, Z, i); 

			if (strcmp(in_key, Y->key[i]) > 0) i++; 
			_readFile(X->child[i], Y);
		}
		_insertNonFull(in_key, Y, X, Z); 
	}
}

//Y is full node to be split -- X will be root (Y will be cleared)
void B_Tree::_splitChild(BNode *X, BNode *Y, BNode *Z, int pointer)
{ 
	//Y = new BNode; 
	//Z = new BNode; 
	int maxNumChildren = X->maxNumChildren;
	int medianIndex = X->t - 1; 
	int _t = X->t; //save t locallaly for easier use

	//make Z a new node
	Z->fileIndex = writeIndex; 
	writeIndex++; 
	Z->numKeys = 0;
	Z->numChildren = 0; 

	_readFile(X->child[pointer], Y); 
	Z->isleaf = Y->isleaf; 
	Z->numKeys = medianIndex;

	for (int i = 1; i <= _t-1  ; i++) //move keys after median into new node 
	{
		strcpy(Z->key[i], Y->key[i + _t]); 
		Z->counter[i] = Y->counter[i + _t]; 
		Y->counter[i + _t] = 1; 
	}

	if (Y->isleaf == false)
	{
		for (int i = 1; i <= _t ; i++)
		{
			Z->child[i] = Y->child[i + _t]; 
			Z->numChildren++; 
			Y->numChildren--; 
		}
	}

	Y->numKeys = _t - 1; //Y should now have less keys since they have been moved 

	//add the new node as a child of X
	for (int i = X->numKeys + 1; i >= pointer + 1; i--)
	{
		X->child[i + 1] = X->child[i]; 
	}
	X->child[pointer + 1] = Z->fileIndex; 
	X->numChildren++; 

	//move keys
	for (int i = X->numKeys; i >= pointer ; i --)
	{
		strcpy(X->key[i + 1], X->key[i]); 
		X->counter[i + 1] = X->counter[i]; 
		X->counter[i] = 1; 
	}

	strcpy(X->key[pointer], Y->key[_t]); 
	X->counter[pointer] = Y->counter[_t]; 
	Y->counter[_t] = 1;
	X->numKeys++; 

	_writeFile(X); 
	_writeFile(Y);
	_writeFile(Z); 

	//delete Y, Z; 
}

void B_Tree::_readFile(int location, BNode * X)
{
	Bfile.seekg((location - 1) * sizeof(BNode));
	Bfile.read((char*)X, sizeof(BNode));
	numDiskReads++; //increment number of disk reads
}

void B_Tree::_writeFile(BNode * X)
{
	int position = sizeof(BNode) * (X->fileIndex - 1); 
	Bfile.seekp(position, fstream::beg); 
	Bfile.write((char*)X, sizeof(BNode)); 
	numDiskWrites++; //increment number of disk writes 
}

int B_Tree::_getFileSize()
{
	std::ifstream in(storageFile, fstream::ate | fstream::binary); 
	return in.tellg(); 
}