#pragma once
#include "common.h"
#include <vector>
using namespace std;

class IntermediaCodeGenerator {

	friend class Parser;

public:
	QUADMEMBER TransferQuadMemberFromAlgExprReturnValue(AlgExprReturnValue);
	QUADSTATMENT GenerateQuadStatment(OperatorCode _opCode, QUADMEMBER _op1, QUADMEMBER _op2, QUADMEMBER _opResult);
	void InsertQuadStatmentIntoIntermediaCodeList(QUADSTATMENT q);
	void ShowIntermediaCodeList(); // 展示中间代码



protected: // Parser中需要使用
	static int currentId; // 链表的当前id值
	vector<IntermediaCodeElement> intermediaCodeList; // 中间代码生成式的链表
	vector<IntermediaCodeElement> falseIntermediaCodeLink; // 假链，用于链表地址的更新

};