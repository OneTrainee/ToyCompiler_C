#pragma once

#include <map>
#include <iostream>
#include "common.h"
using namespace std;

class SemanticAnalyzer {

public:
	SemanticAnalyzer() = default;
	bool AddNewVarObjectIntoVarTable(bool isStr,string varName); // ����µı�������������
	void ShowVarTable(bool isTmpVarTable = 0);
	bool GetTheExistentVarObjectFromVarTable(const string, PVAROBJECT& pVarObj); // �ӱ������еõ��Ѵ��ڵĶ���
	string GenerateTmpVarObject(); // �Զ������м��������������,����϶�����ֵ

public:
	PVAROBJECT outVarObjectPointer(string varName);

private:
	map<string,_VarObject> varTable; // ������
	map<string,_VarObject> tmpVarTable; // �м������
	static int tmpVarObjectPostifix; // �м������׺

};