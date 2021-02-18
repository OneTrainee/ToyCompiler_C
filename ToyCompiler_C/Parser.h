#pragma once
#include <iostream>
#include "LexicalAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "IntermediaCodeGenerator.h"
#include "Logger.h"
#include "common.h"
#include <map>
using namespace std;




//
// 词法解析器
//
class Parser {

public:
	Parser(string resource_file_name);

private:
	LexicalAnalyzer lexical;
	SemanticAnalyzer semantic;
	IntermediaCodeGenerator inter_code_generator;
	Logger logger;

private: // 日志相关
	static map<string, string> LogDict; // 日志字典
	static map<string, string> InitLogDict(); // 初始化日志字典
	void notifyLogger(string currentStatementStatus); // 通知记录器

public:


	// 包含各种语法规则，来进行解析 

	//
	// 程序主体语句的集合
	//
	void Program();	// <程序>
	void End();  // <结束>
	void DeclarationList(); // <声明语句序列结尾>
	void DeclarationListTail(); // <声明语句序列结尾>
	void Declaration();	// <声明语句>
	void DeclarationTail(bool isStr); // <声明语句结尾>
	void StatmentList(); // <语句序列>
	void StatmentListTail(); // <语句序列结尾>
	void Statment(); // <语句>

	//
	// 各种语句的集合
	//
	void AssignStatment(); // <赋值语句>
	void CompoundStatment(); // <复合语句>
	void IfStatement(); // <if语句>
	void WhileStatement(); // <while语句>
	void InStatement(); // <cin语句>
	void OutStatement(); // <cout语句>

	// 布尔表达式 [注意：其不需要返回值，直接生成中间代码即可]
	int BoolExpr();

	//
	// 算数表达式全部集合
	//
	AlgExprReturnValue AlgExrp(); // <算数表达式>
	AlgExprReturnValue AlgExprTail(AlgExprReturnValue, bool isAdd); // <算数表达式尾> ； mark = 1 加号 ; mark = 2 减号。
	AlgExprReturnValue Item(); // <项>
	AlgExprReturnValue ItemTail(AlgExprReturnValue); // <项尾>
	AlgExprReturnValue Factor(); // <因子>

};

