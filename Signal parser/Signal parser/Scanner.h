#ifndef SCANNER_H_
#define SCANNER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <list>

using namespace std;

struct _word_
{
	string Word;
	unsigned short int Code;
};

class Scanner
{
public:
	Scanner();
	~Scanner();

	list<int> processFile();
	bool setFile(string);

	void showIdentifers();

	list<_word_> getIDENTIFIERSTable();
	list<_word_> getKEYSTable();

private:
	list<_word_> IDENTIFIERS;
	list<_word_> KEYS;

	ifstream mFile;

	void addIdentifier(string, int);
	short int getLastIdentifierNumber();
	unsigned short int getLexemType(char);
	int searchIdentifier(string);
	int searchKeys(string);
};

#endif