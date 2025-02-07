// AVL_BTrees.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "AVL.h"
#include "B_Tree.h"
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std; 

int main()
{
	AVL tree;
	B_Tree bTree;
	ifstream infile;

	char key[40];
	char cmd[40];
	string readIn;

	clock_t begin = clock(); 

	//code to read files
	string argument, string;
	//Code to Read File 
	char c;

	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;

	ifstream inFile;
	inFile.open("Shakespeare.txt", ios::binary);
	if (inFile.fail())
	{
		cout << "Unable to open input file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		cin.get(c);
		exit(1);
	}
	iPtr = 0;
	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			tree.Insert(chari);
			bTree.Insert(chari);
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;

		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration
		inFile.get(c);
	}
	inFile.close();
	tree.GetInfo();
	bTree.GetInfo();

	clock_t end = clock(); 
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC; 
	std::cout << "Elapsed Time (seconds): " << elapsed_secs << "\n"; 
}