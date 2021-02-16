#pragma once
#include <fstream>
using namespace std;


// 符号标志
typedef enum symbol{
	KW_MAIN, KW_IF, KW_ELSE, KW_WHILE, KW_INT, KW_COUT, KW_CIN, KW_ENDL, KW_RETURN, KW_STRING, // 关键字
	LBRACE, RBRACE, LPAREN, RPAREN, SEMI, COMMA, // 界符 ;/ ,
	ASSIGN, ADD, MINUS, TIMES, DIVIDE, // 运算
	EQ, NE, G, GE, B, BE, IN, OUT, // 判断标志位
	STR, NUM, ID, // 字符串，数字，标识符
	END, STRASSIGN // 文件结尾,字符串赋值
}SYMBOL;


class LexicalAnalyzer {
public:
	LexicalAnalyzer(string file_name); // 构造函数

public: // 基本组件
	bool getCh(); // 获取字符
	int getSym(); // 获取符号
	void checkKeyWord(); // 遍历获取的id，是否在keyword中，打上相关标签

public: // 输出组件
	SYMBOL outSym();
	string outSymbolName();
	string outVarName();
	int outNum();

private: // 扫描工具
	char ch; // 存储字符的全局变量
	char oldCh; // 上一个字符
	char line[80]; // 缓冲区
	enum symbol sym; // 符号
	const char* symbolName[34] = {
		"main","if","else","while","int","cout","cin","endl","return","string",
		"{","}","(",")",";",",",
		"=","+","-","*","/",
		"==","!=",">",">=","<","<=",">>","<<",
		"String" , "Number", "Id",
		"#","@"
	};
	
private: // 扫描结果
	char varName[40]; // 存储id的值
	int num; // 存储数字的值
	char str[40]; // 存放字符串的值

private: // 文件
	ifstream input_resource_file;
	ofstream lexical_result_file;

};

