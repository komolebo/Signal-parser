#include "Scanner.h"

Scanner::Scanner()
{
	KEYS.push_back({ "BEGIN", 401 });
	KEYS.push_back({ "END", 402 });
	KEYS.push_back({ "VAR", 403 });
	KEYS.push_back({ "PROCEDURE", 404 });
	KEYS.push_back({ "SIGNAL", 405 });
	KEYS.push_back({ "COMPLEX", 406 });
	KEYS.push_back({ "INTEGER", 407 });
	KEYS.push_back({ "FLOAT", 408 });
	KEYS.push_back({ "BLOCKFLOAT", 409 });
	KEYS.push_back({ "EXT", 410 });
}

Scanner::~Scanner()
{
}

unsigned short int Scanner::getLexemType(char symb)
{
	if ((int)symb == 32 || (int)symb == 10 || (int)symb == 9)
		return 0;
	if ((int)symb >= 48 && (int)symb <= 57)
		return 1;
	if ((int)symb >= 65 && (int)symb <= 90 || (int)symb >= 97 && (int)symb <= 122 || symb == '_')
		return 2;
	if (symb == '(')
		return 3;
	if (symb == ')' || symb == ',' || symb == ';' || symb == ':')
		return 4;
	return 5;
}

int Scanner::searchKeys(string word)
{
	list<_word_>::iterator it;
	int index = 0;

	for (it = KEYS.begin(); it != KEYS.end(); ++it)
	{
		if ((*it).Word == word) return (*it).Code;
		index++;
	}
	return -1;
}

int Scanner::searchIdentifier(string word)
{
	list<_word_>::iterator it;

	for (int i = 0; i < (int)word.size(); i++)
		word[i] = toupper(word[i]);

	int index = 0;

	for (it = IDENTIFIERS.begin(); it != IDENTIFIERS.end(); ++it)
	{
		if ((*it).Word == word) return (*it).Code;
		index++;
	}
	return -1;
}

short int Scanner::getLastIdentifierNumber()
{
	list<_word_>::iterator it = IDENTIFIERS.end();

	if (IDENTIFIERS.size() == 0) return 1000;

	return (*(--it)).Code;
}

void Scanner::addIdentifier(string word, int code)
{
	for (int i = 0; i < (int)word.size(); i++)
		word[i] = toupper(word[i]);
	IDENTIFIERS.push_back({ word, code });
}

bool Scanner::setFile(string filename)
{
	mFile.open(filename);

	if (!mFile.is_open())
		return false;
	
	return true;
}

list<int> Scanner::processFile()
{
	list<int> codedLine;

	char symbol;
	string buf;
	bool suppressOutput;
	unsigned short int lexcode;
	mFile.get(symbol);

	while (!mFile.eof())
	{
		buf = "";
		suppressOutput = false;

		switch (getLexemType(symbol))
		{
		case 0:	/* whitespace */
			while (!mFile.eof())
			{
				mFile.get(symbol);
				if (getLexemType(symbol) != 0)
					break;
			}
			suppressOutput = true;
			break;
		case 2:	/* identifier */
			while (!mFile.eof() && getLexemType(symbol) == 2 || getLexemType(symbol) == 1)
			{
				buf += symbol;
				mFile.get(symbol);
			}
			if (searchKeys(buf) > 0)
				lexcode = searchKeys(buf);
			else
			{
				if (searchIdentifier(buf) > 0)
					lexcode = searchIdentifier(buf);
				else
				{
					lexcode = getLastIdentifierNumber() + 1;
					addIdentifier(buf, lexcode);
				}
			}
			break;
		case 3: /* Maybe comment, that's '(' */
			if (mFile.eof())
				lexcode = 40; // code of '('
			else
			{
				mFile.get(symbol);
				if (symbol == '*')
				{
					if (mFile.eof())
						cout << "Error: * expected but end of file found" << endl;
					else
					{
						mFile.get(symbol);
						do
						{
							while (!mFile.eof() && symbol != '*')
								mFile.get(symbol);
							if (mFile.eof())
							{
								cout << "Error: * expected but end of file found \n";
								symbol = '+';
								break;
							}
							else
								mFile.get(symbol);
						} while (symbol != ')');

						if (symbol == ')')
							suppressOutput = true;
						if (!mFile.eof())
							mFile.get(symbol);
					}
				}
				else if (mFile.eof())
					cout << "Error: ) expected but end of file found" << endl;
				else
					lexcode = 40;
			}
			break;
		case 4: /* Delimiter except '(' */
			lexcode = (int)symbol;
			mFile.get(symbol);
			break;
		case 5:
		case 1:
			cout << "Error: Illegal symbol" << endl;
			while (getLexemType(symbol) != 0 && !mFile.eof())
				mFile.get(symbol);
			break;
		default:
			break;
		}

		if (!suppressOutput)
			codedLine.push_back(lexcode);
	}
	mFile.close();
	return codedLine;
}

void Scanner::showIdentifers()
{
	list<_word_>::iterator it;
	for (it = IDENTIFIERS.begin(); it != IDENTIFIERS.end(); ++it)
		cout <<(*it).Code << ": " << (*it).Word << endl;
}

list<_word_> Scanner::getIDENTIFIERSTable()
{
	return IDENTIFIERS;
}

list<_word_> Scanner::getKEYSTable()
{
	return KEYS;
}