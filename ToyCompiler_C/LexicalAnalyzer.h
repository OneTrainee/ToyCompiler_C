#pragma once
#include <fstream>
using namespace std;


// ���ű�־
typedef enum symbol{
	KW_MAIN, KW_IF, KW_ELSE, KW_WHILE, KW_INT, KW_COUT, KW_CIN, KW_ENDL, KW_RETURN, KW_STRING, // �ؼ���
	LBRACE, RBRACE, LPAREN, RPAREN, SEMI, COMMA, // ��� ;/ ,
	ASSIGN, ADD, MINUS, TIMES, DIVIDE, // ����
	EQ, NE, G, GE, B, BE, IN, OUT, // �жϱ�־λ
	STR, NUM, ID, // �ַ��������֣���ʶ��
	END, STRASSIGN // �ļ���β,�ַ�����ֵ
}SYMBOL;


class LexicalAnalyzer {
public:
	LexicalAnalyzer(string file_name); // ���캯��

public: // �������
	bool getCh(); // ��ȡ�ַ�
	int getSym(); // ��ȡ����
	void checkKeyWord(); // ������ȡ��id���Ƿ���keyword�У�������ر�ǩ

public: // ������
	SYMBOL outSym();
	string outSymbolName();
	string outVarName();
	int outNum();

private: // ɨ�蹤��
	char ch; // �洢�ַ���ȫ�ֱ���
	char oldCh; // ��һ���ַ�
	char line[80]; // ������
	enum symbol sym; // ����
	const char* symbolName[34] = {
		"main","if","else","while","int","cout","cin","endl","return","string",
		"{","}","(",")",";",",",
		"=","+","-","*","/",
		"==","!=",">",">=","<","<=",">>","<<",
		"String" , "Number", "Id",
		"#","@"
	};
	
private: // ɨ����
	char varName[40]; // �洢id��ֵ
	int num; // �洢���ֵ�ֵ
	char str[40]; // ����ַ�����ֵ

private: // �ļ�
	ifstream input_resource_file;
	ofstream lexical_result_file;

};

