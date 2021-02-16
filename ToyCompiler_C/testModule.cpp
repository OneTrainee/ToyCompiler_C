#include "testModule.h"
#include "Parser.h"
#include <iostream>
using namespace std;


/*
	输入源文件,测试读取是否顺利
*/
void Lexical_test::test_resource_file() {
	LexicalAnalyzer s("C:\\Users\\97905\\Desktop\\小型类C编译器\\cc.txt");
	int r;
	s.getCh();
	while (r = s.getSym()) {
		cout << s.outSymbolName() << endl;
		
	}
}

void Parser_test::test_declaration() {
	Parser pa("C:\\Users\\97905\\Desktop\\小型类C编译器\\cc.txt");
	pa.Program();
}

