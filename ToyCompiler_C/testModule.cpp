#include "testModule.h"
#include "Parser.h"
#include <iostream>
using namespace std;


/*
	����Դ�ļ�,���Զ�ȡ�Ƿ�˳��
*/
void Lexical_test::test_resource_file() {
	LexicalAnalyzer s("C:\\Users\\97905\\Desktop\\С����C������\\cc.txt");
	int r;
	s.getCh();
	while (r = s.getSym()) {
		cout << s.outSymbolName() << endl;
		
	}
}

void Parser_test::test_declaration() {
	Parser pa("C:\\Users\\97905\\Desktop\\С����C������\\cc.txt");
	pa.Program();
}

