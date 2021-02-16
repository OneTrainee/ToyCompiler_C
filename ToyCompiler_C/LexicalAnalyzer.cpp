#include "LexicalAnalyzer.h"
#include "common.h"
#include <iostream>

LexicalAnalyzer::LexicalAnalyzer(string resource_file_name):input_resource_file(resource_file_name), ch(0){

	if (!input_resource_file) {
		CERR("The file \""+ resource_file_name + "\" does not exsit!");
	}
	cout << "LexicalAnalyzer has initialized successfully ..." << endl;
}

bool LexicalAnalyzer::getCh() {
	oldCh = ch;
	// 使用 "input_resource_file >> ch" 这种语法会阻止空白字符输入	
	if (ch = input_resource_file.get()) {
		return true;
	}
	else {
		return false;
	}
}

string LexicalAnalyzer::outSymbolName() {
	return string(symbolName[sym]);
}

enum symbol LexicalAnalyzer::outSym() {
	return sym;
}

string LexicalAnalyzer::outVarName() {
	return varName;
}

int LexicalAnalyzer::outNum() {
	return num;
}

int LexicalAnalyzer::getSym() {
	
	// 存储到上一个
	if (lexical_result_file) {
		lexical_result_file << endl << symbolName[sym];
	}

	int f = 0;
	// 循环读取,直到读到非空白字符
	while (ch == ' ' || ch == 10 || ch == 9) {
		f = getCh();
	}

	// 如果是字母或_开头
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_') {
		int idCount = 0;
		do {
			// 存入上一个,再获取新的一个,顺序不要颠倒了
			varName[idCount++] = ch;
			f = getCh();
		} while (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_');
		varName[idCount] = 0;
		checkKeyWord(); // 判断是关键字还是id..

		return 1;
	}

	// 如果是数字开头，默认正数
	else if (ch >= '0' && ch <= '9')
	{
		int value = 0;
		do {
			value = value * 10 + ch - '0';
			f = getCh();
		} while (ch >= '0' && ch <= '9'); // 判断条件一定要明确

		sym = NUM;
		num = value;

	}
	// 其他各种符号 与 字符串
	else {
		switch (ch)
		{
			/*** 单运算符判断 ***/
		case '#':
			sym = END;
			f = 0;
			break;
		case '{':
			sym = LBRACE;
			f = getCh(); // 继续读取
			break;
		case'}':
			sym = RBRACE;
			f = getCh();
			break;
		case '(':
			sym = LPAREN;
			f = getCh();
			break;
		case ')':
			sym = RPAREN;
			f = getCh();
			break;
		case ';':
			sym = SEMI;
			f = getCh();
			break;
		case ',':
			sym = COMMA;
			f = getCh();
			break;
		case '+':
			sym = ADD;
			f = getCh();
			break;
		case '-':
			sym = MINUS;
			f = getCh();
			break;
		case '*':
			sym = TIMES;
			f = getCh();
			break;
		case '@':
			sym = STRASSIGN;
			f = getCh();
			break;
		case '/':
			sym = DIVIDE;
			f = getCh();
			/**** 双运算符判断 *****/
		case '=':
			f = getCh();
			if (ch == '=') {
				sym = EQ;
				// 再往前读一行
				f = getCh();
			}
			else {
				sym = ASSIGN;
			}
			break;
		case '!':
			f = getCh(); // 直接去读 = 
			f = getCh(); // 再读取接下来要处理的值
			sym = NE;
			break;
		case '>':
			f = getCh();
			if (ch == '=') {
				sym = GE;
				f = getCh();
			}
			else if (ch == '>') {
				sym = IN;
				f = getCh();
			}
			else {
				sym = G;
			}
			break;
		case '<':
			f = getCh();
			if (ch == '=') {
				sym = BE;
				f = getCh();
			}
			else if (ch == '<') {
				sym = OUT;
				f = getCh();
			}
			else {
				sym = B;
			}
			break;
			/*** 字符串判断 ***/
		case '\"':
			sym = STR;
			f = getCh(); // 读取下一个字符
			int strLen = 0;
			while (ch != '\"') {
				str[strLen++] = ch;
				f = getCh();
			}
			f = getCh(); // 继续读取下一个字符
			break;
		}

	}

	// 结尾进行判断
	if (f == 0) {
		sym = END;
	}
	return f;
}


const int arrLen = 10;
const char* keyWords[arrLen] = { "main","if","else","while","int","cout","cin","endl","return","string" };
const enum symbol keySymbols[arrLen] = { KW_MAIN, KW_IF, KW_ELSE, KW_WHILE, KW_INT, KW_COUT, KW_CIN, KW_ENDL, KW_RETURN,KW_STRING };


void LexicalAnalyzer::checkKeyWord() {
	// 将id来遍历 keyWords[] 数组，打上标签
	int flag = 0; // 标记是否是关键字
	for (int i = 0; i < arrLen; i++) {
		if (strcmp(varName, keyWords[i]) == 0) {
			sym = keySymbols[i]; // 完成赋值
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		sym = ID;
}