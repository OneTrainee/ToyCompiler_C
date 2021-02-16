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
	void ShowIntermediaCodeList(); // չʾ�м����



protected: // Parser����Ҫʹ��
	static int currentId; // ����ĵ�ǰidֵ
	vector<IntermediaCodeElement> intermediaCodeList; // �м��������ʽ������
	vector<IntermediaCodeElement> falseIntermediaCodeLink; // ���������������ַ�ĸ���

};