// AVL_BTrees.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "AVL.h"
#include "B_Tree.h"
#include <iostream>
#include <fstream>

using namespace std; 

int main()
{
	AVL AVL_tree;
	B_Tree bTree; 
	ifstream infile; 

	char key[40]; 
	char cmd[40]; 
	string readIn; 

	cout << "Input command...(input for manual)\n"; 
	cin >> cmd; 

	if (strcmp(cmd, "input") == 0)
	{
		do
		{
			std::cin >> key;
			bTree.Insert(key); 
			//if (strcmp(key, "read") == 0) tree.Read(); 
			//else tree.Insert(key);
		} while (strcmp(key, "exit") != 0);
		return 0; 
	}
	else
	{
		//code to read files
		string argument, string;
		//Code to Read File 
		char c;

		char chari[50]; // assumes no word is longer than 49 characters
		memset(chari, 0, 50); // zero the word buffer
		int iPtr;
		bool IsDelimiter = false, WasDelimiter = false;

		ifstream inFile;
		inFile.open("Harry Potter and the Sorcerer.txt", ios::binary);
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
				bTree.Insert(chari); 
				AVL_tree.Insert(chari);

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

		bTree.GetInfo(); 
		std::cout << "\n"; 
		AVL_tree.GetInfo(); 
	} 
}