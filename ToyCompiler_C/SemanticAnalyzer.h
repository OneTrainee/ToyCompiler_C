#pragma once

#include <map>
#include <iostream>
#include "common.h"
using namespace std;

class SemanticAnalyzer {

public:
	SemanticAnalyzer() = default;
	bool AddNewVarObjectIntoVarTable(bool isStr,string varName); // 添加新的变量到变量表中
	void ShowVarTable(bool isTmpVarTable = 0);
	bool GetTheExistentVarObjectFromVarTable(const string, PVAROBJECT& pVarObj); // 从变量表中得到已存在的对象
	string GenerateTmpVarObject(); // 自动生成中间变量到变量表中,这里肯定是数值

public:
	PVAROBJECT outVarObjectPointer(string varName);

private:
	map<string,_VarObject> varTable; // 变量表
	map<string,_VarObject> tmpVarTable; // 中间变量表
	static int tmpVarObjectPostifix; // 中间变量后缀

};