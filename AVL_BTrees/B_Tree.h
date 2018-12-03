#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>

using namespace std; 

class B_Tree
{
	

public:
	struct BNode
	{
		//alter this value to change the t - value of the b tree
		static const int t = 100;

		int fileIndex;
		char key[2 * t][50]; //keys
		int counter[2 * t]; //counters for each of the number of occurances of each key
		int child[2 * t + 1]; //pointers to children

		int numKeys = 0; 
		int numChildren = 0; 
		int maxNumKeys = 2 * t - 1; //max number of keys allowed --- same as max index in nodes 
		int maxNumChildren = 2 * t + 1; 
		bool isleaf = false; //is this node a leaf
	};

	struct SearchReturn
	{
		int keyLocation = 0;
		int nodeLocation = 0;
		bool foundExact = false; 
	};

	B_Tree();

	void Insert(char in_key[]);
	void GetInfo(); 

	int numRootNode = 0;
	int writeIndex = 1;

	int numDiskReads = 0; //number of disk reads
	int numDiskWrites = 0; //number of disk writes
	int numKeys = 0;

private: 
	string storageFile = "BTree_Storage.txt"; //storage file for BTree Nodes

	void _traverse(BNode *currentNode); //print in-order traversal 
	void _search(BNode *Y, char in_key[], SearchReturn *returnedInfo); 
	int _getHeight(BNode *currentNode, int currentHeight); 
	void _insertNonFull(char in_key[], BNode *X, BNode *Y, BNode *Z); 
	void _splitChild(BNode *X,BNode *Y, BNode *Z, int pointer); 
	void _readFile(int location, BNode *X); 
	void _writeFile(BNode *X);
	int _getFileSize();
};

