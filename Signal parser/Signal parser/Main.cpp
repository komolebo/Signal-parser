#include "Scanner.h"
#include "Parser.h"

int main()
{	
	Scanner scanner = Scanner();

	if (!scanner.setFile("Program.txt"))
	{
		cout << "File can't be opened" << endl;
		return 0;
	}
		
	list<int> codedLine = scanner.processFile();	

	cout << "All identifiers:" << endl;
	scanner.showIdentifers();

	cout << "Encoded line:" << endl;
	for each (int symbol in codedLine)
		cout << symbol << " ";
	cout << endl;

	Parser parser = Parser(codedLine, scanner.getIDENTIFIERSTable(), scanner.getKEYSTable());
	parser.PRG();
	
	cout << endl << "Compiled successfully ..." << endl;

	return 0;
}