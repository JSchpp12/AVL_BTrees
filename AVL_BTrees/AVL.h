#pragma once
class AVL
{
public:
	struct AVL_Node
	{
		int BF; 
		int fileIndex;   //where in the file this node is written
		int leftChild;   
		int rightChild; 
		char key[50]; 
	};

	int writeIndex = 0;

	//storage for nodes in memory 
	AVL_Node nodeStorage[3];

	AVL();

private: 
	void writeNodeToFile(AVL_Node* nodeToWrite); 
	void _updateBFs(AVL_Node* in_node);
	int _calculateBalanceFactor(AVL_Node* focusNode);
	int _getNodeHeight(AVL_Node* focusNode);
	void _rotationHandler(AVL_Node* focusNode)
};

