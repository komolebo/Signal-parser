#include "Parser.h"

Parser::Parser(list<int> _codedLine, list<_word_> _IDENTIFIERS, list<_word_> _KEYS)
{
	codedLine = _codedLine;
	IDENTIFIERS = _IDENTIFIERS;
	KEYS = _KEYS;
}

Parser::~Parser()
{
}

string Parser::getWordByKey(int code, list<_word_> table)
{
	list<_word_>::iterator it;

	for (it = table.begin(); it != table.end(); ++it)
	{
		if ((*it).Code == code)	return (*it).Word;
	}

	return "";
}

int Parser::searchIdentifier(string word)
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

void Parser::PRG()
{
	int tabCounter = 0;
	printTreeElement(tabCounter, "<program>");

	if (!codedLine.size() || getWordByKey(codedLine.front(), KEYS) != "PROCEDURE")
		ERR("SYNTAX ERR: Expected 'PROCEDURE'");
	printTreeElement(tabCounter + 1, "PROCEDURE");
	codedLine.pop_front();

	PRCIDN(codedLine.front(), tabCounter + 1);

	PRMLST(codedLine.front(), tabCounter + 1);

	BLK(codedLine.front(), tabCounter + 1);
	if (!codedLine.size() || (char)codedLine.front() != ';')
		ERR("SYNTAX ERR: Expected ';' in the end of program");
	codedLine.pop_front();

	if (!codedLine.empty())
		ERR("SYNTAX ERR: Found symbols after 'END;'");
	printTreeElement(tabCounter, "</program>");
}

void Parser::BLK(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<block>");
	DCLS(currentLexem, tabCounter + 1);

	if (!codedLine.size() || getWordByKey(codedLine.front(), KEYS) != "BEGIN")
		ERR("SYNTAX ERR: Expected 'BEGIN'");
	printTreeElement(tabCounter, "BEGIN");
	codedLine.pop_front();

	if (!codedLine.size() || getWordByKey(codedLine.front(), KEYS) != "END")
		ERR("SYNTAX ERR: Expected 'END'");
	printTreeElement(tabCounter, "END");
	codedLine.pop_front();
	printTreeElement(tabCounter, "</block>");
}

void Parser::DCLS(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<declarations-list>");
	VARDCLS(currentLexem, tabCounter + 1);
	printTreeElement(tabCounter, "</declarations-list>");
}

void Parser::VARDCLS(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<var-declarations>");

	if (!codedLine.size() || getWordByKey(codedLine.front(), KEYS) != "VAR")
		ERR("SYNTAX ERR: Expected 'VAR'");
	codedLine.pop_front();

	DCLLST(codedLine.front(), tabCounter + 1);
	printTreeElement(tabCounter, "</var-declarations>");
}

void Parser::PRCIDN(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<procedure-identifier>");
	IDN(currentLexem, tabCounter + 1);
	printTreeElement(tabCounter, "</procedure-identifier>");
}

void Parser::IDN(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<identifier>");
	if (searchIdentifier(getWordByKey(currentLexem, IDENTIFIERS)) < 0)
		ERR("SYNTAX ERR: Expected parameter identifier");
	printTreeElement(tabCounter + 1, getWordByKey(currentLexem, IDENTIFIERS));
	codedLine.pop_front();
	printTreeElement(tabCounter, "</identifier>");
}

void Parser::PRMLST(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<parameters-list>");
	if ((char)currentLexem == '(')
	{
		codedLine.pop_front();
		DCLLST(codedLine.front(), tabCounter + 1);
	}
	else
		ERR("SYNTAX ERR: Expected '(' in parameters list");

	if (codedLine.size() && (char)codedLine.front() == ')')
		codedLine.pop_front();
	else
		ERR("SYNTAX ERR: Expected ')' parameters list");
	
	if (codedLine.size() && (char)codedLine.front() == ';')
		codedLine.pop_front();
	else
		ERR("SYNTAX ERR: Expected ';' parameters list");
	printTreeElement(tabCounter, "</parameters-list>");
}

void Parser::DCLLST(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<declarations-list>");
	DCL(currentLexem, tabCounter + 1);

	while (codedLine.size() && (char)codedLine.front() == ';')
	{
		codedLine.pop_front();
		DCL(codedLine.front(), tabCounter + 1);
	}
	printTreeElement(tabCounter, "</declarations-list>");
}

void Parser::DCL(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<declaration>");
	VAR(currentLexem, tabCounter + 1);

	while (codedLine.size() && (char)codedLine.front() == ',')
	{
		codedLine.pop_front();
		IDN(codedLine.front(), tabCounter + 1);
	}

	if (codedLine.size() && (char)codedLine.front() == ':')
	{
		codedLine.pop_front();
		ATTR(codedLine.front(), tabCounter + 1);

		while (codedLine.size() && (char)codedLine.front() == ' ')
			ATTR(codedLine.front(), tabCounter + 1);
	}
	else
		ERR("SYNTAX ERR: Expected ':' in declaration");
	printTreeElement(tabCounter, "</declaration>");
}

void Parser::ATTR(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<attribute>");
	string currentType = getWordByKey(currentLexem, KEYS);
	if (currentType != "SIGNAL" && currentType != "COMPLEX" && currentType != "INTEGER"
		&& currentType != "FLOAT" && currentType != "BLOCKFLOAT" && currentType != "EXT")
		ERR("SYNTAX ERR: Expected attribute");
	else
	{
		printTreeElement(tabCounter + 1, currentType);
		codedLine.pop_front();
	}
	printTreeElement(tabCounter, "</attribute>");
}

void Parser::VAR(int currentLexem, int tabCounter)
{
	printTreeElement(tabCounter, "<var>");
	IDN(currentLexem, tabCounter + 1);
	printTreeElement(tabCounter, "</var>");
}

void Parser::ERR(char * message)
{
	cout << message << endl;
	exit(0);
}

void Parser::printTreeElement(int tabCounter, string word)
{
	string tabs = "";
	for (int i = 0; i < tabCounter; i++)
		tabs += '\t';
	cout << tabs << word << endl;
}