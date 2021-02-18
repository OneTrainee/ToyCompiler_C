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
// �ʷ�������
//
class Parser {

public:
	Parser(string resource_file_name);

private:
	LexicalAnalyzer lexical;
	SemanticAnalyzer semantic;
	IntermediaCodeGenerator inter_code_generator;
	Logger logger;

private: // ��־���
	static map<string, string> LogDict; // ��־�ֵ�
	static map<string, string> InitLogDict(); // ��ʼ����־�ֵ�
	void notifyLogger(string currentStatementStatus); // ֪ͨ��¼��

public:


	// ���������﷨���������н��� 

	//
	// �����������ļ���
	//
	void Program();	// <����>
	void End();  // <����>
	void DeclarationList(); // <����������н�β>
	void DeclarationListTail(); // <����������н�β>
	void Declaration();	// <�������>
	void DeclarationTail(bool isStr); // <��������β>
	void StatmentList(); // <�������>
	void StatmentListTail(); // <������н�β>
	void Statment(); // <���>

	//
	// �������ļ���
	//
	void AssignStatment(); // <��ֵ���>
	void CompoundStatment(); // <�������>
	void IfStatement(); // <if���>
	void WhileStatement(); // <while���>
	void InStatement(); // <cin���>
	void OutStatement(); // <cout���>

	// �������ʽ [ע�⣺�䲻��Ҫ����ֵ��ֱ�������м���뼴��]
	int BoolExpr();

	//
	// �������ʽȫ������
	//
	AlgExprReturnValue AlgExrp(); // <�������ʽ>
	AlgExprReturnValue AlgExprTail(AlgExprReturnValue, bool isAdd); // <�������ʽβ> �� mark = 1 �Ӻ� ; mark = 2 ���š�
	AlgExprReturnValue Item(); // <��>
	AlgExprReturnValue ItemTail(AlgExprReturnValue); // <��β>
	AlgExprReturnValue Factor(); // <����>

};

