#include "Scanner.h"

using namespace std;

class Parser
{
public:
	Parser(list<int> , list<_word_>, list<_word_> );
	~Parser();

	void PRG();
	void PRCIDN(int , int );
	void IDN(int, int );
	void PRMLST(int , int );
	void DCLLST(int , int );
	void DCL(int , int );
	void VAR(int , int );
	void ATTR(int, int );
	void BLK(int , int);
	void DCLS(int , int );
	void VARDCLS(int , int );
	void ERR(char * );
private:
	list<_word_> IDENTIFIERS;
	list<_word_> KEYS;
	list<int> codedLine;

	int searchIdentifier(string);
	string getWordByKey(int, list<_word_>);
	void printTreeElement(int, string );
};

