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
	// ʹ�� "input_resource_file >> ch" �����﷨����ֹ�հ��ַ�����	
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
	
	// �洢����һ��
	if (lexical_result_file) {
		lexical_result_file << endl << symbolName[sym];
	}

	int f = 0;
	// ѭ����ȡ,ֱ�������ǿհ��ַ�
	while (ch == ' ' || ch == 10 || ch == 9) {
		f = getCh();
	}

	// �������ĸ��_��ͷ
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_') {
		int idCount = 0;
		do {
			// ������һ��,�ٻ�ȡ�µ�һ��,˳��Ҫ�ߵ���
			varName[idCount++] = ch;
			f = getCh();
		} while (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_');
		varName[idCount] = 0;
		checkKeyWord(); // �ж��ǹؼ��ֻ���id..

		return 1;
	}

	// ��������ֿ�ͷ��Ĭ������
	else if (ch >= '0' && ch <= '9')
	{
		int value = 0;
		do {
			value = value * 10 + ch - '0';
			f = getCh();
		} while (ch >= '0' && ch <= '9'); // �ж�����һ��Ҫ��ȷ

		sym = NUM;
		num = value;

	}
	// �������ַ��� �� �ַ���
	else {
		switch (ch)
		{
			/*** ��������ж� ***/
		case '#':
			sym = END;
			f = 0;
			break;
		case '{':
			sym = LBRACE;
			f = getCh(); // ������ȡ
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
			/**** ˫������ж� *****/
		case '=':
			f = getCh();
			if (ch == '=') {
				sym = EQ;
				// ����ǰ��һ��
				f = getCh();
			}
			else {
				sym = ASSIGN;
			}
			break;
		case '!':
			f = getCh(); // ֱ��ȥ�� = 
			f = getCh(); // �ٶ�ȡ������Ҫ�����ֵ
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
			/*** �ַ����ж� ***/
		case '\"':
			sym = STR;
			f = getCh(); // ��ȡ��һ���ַ�
			int strLen = 0;
			while (ch != '\"') {
				str[strLen++] = ch;
				f = getCh();
			}
			f = getCh(); // ������ȡ��һ���ַ�
			break;
		}

	}

	// ��β�����ж�
	if (f == 0) {
		sym = END;
	}
	return f;
}


const int arrLen = 10;
const char* keyWords[arrLen] = { "main","if","else","while","int","cout","cin","endl","return","string" };
const enum symbol keySymbols[arrLen] = { KW_MAIN, KW_IF, KW_ELSE, KW_WHILE, KW_INT, KW_COUT, KW_CIN, KW_ENDL, KW_RETURN,KW_STRING };


void LexicalAnalyzer::checkKeyWord() {
	// ��id������ keyWords[] ���飬���ϱ�ǩ
	int flag = 0; // ����Ƿ��ǹؼ���
	for (int i = 0; i < arrLen; i++) {
		if (strcmp(varName, keyWords[i]) == 0) {
			sym = keySymbols[i]; // ��ɸ�ֵ
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		sym = ID;
}