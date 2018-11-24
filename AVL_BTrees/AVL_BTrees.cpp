// AVL_BTrees.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "AVL.h"
#include <iostream>
#include <fstream>

using namespace std; 

int main()
{
	AVL tree; 
	char key[40]; 
	char cmd[40]; 

	cout << "Input command...(input for manual)\n"; 
	cin >> cmd; 

	if (strcmp(cmd, "input"))
	{
		do
		{
			std::cin >> key;
			tree.Insert(key);
		} while (strcmp(key, "exit") != 0);
		return 0; 
	}
	else
	{
		//code to read files
		string argument, string;
		//Code to Read File 

		char c;

		AVL AVL_T;
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
				AVL_T.Insert(chari);

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
	} 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
