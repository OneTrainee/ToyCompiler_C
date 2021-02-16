#include "SemanticAnalyzer.h"
#include <string>

int SemanticAnalyzer::tmpVarObjectPostifix = 0;

bool SemanticAnalyzer::AddNewVarObjectIntoVarTable(bool isStr, string varName) {
	
	_VarObject vObj;
	vObj.isInitial = false;
	vObj.type = isStr == true ? _VarObject::_STR : _VarObject::_NUM;
	vObj.name = varName;

	if (varTable.count(varName) == 0) {
		varTable[varName] = vObj;
		return true;
	}
	else {
		CERR(("变量已存在:" + varName + " AddNewVarObjectIntoVarTable 调用失败"));
		return false;
	}

	

}

void SemanticAnalyzer::ShowVarTable(bool isTmpVarTable){
	map<string, struct _VarObject>& table = isTmpVarTable == 0 ? varTable : tmpVarTable;
	for (auto it = table.begin(); it != table.end(); it++) {
		cout << "varName:[" + it->first+"]" << endl;
	}
}

bool SemanticAnalyzer::GetTheExistentVarObjectFromVarTable(const string varName, PVAROBJECT & pVarObj) {
	// 先尝试从变量表中获取
	if (varTable.count(varName) > 0) {
		pVarObj = &varTable[varName];
		return true;
	}

	// 否则从中间变量表中获取
	if (tmpVarTable.count(varName) > 0) {
		pVarObj = &tmpVarTable[varName];
		return true;
	}

	// 否则输出错误
	CERR("从变量表中获取变量失败,varName:" + varName + "GetTheExisittingVarObjectFromVarTable函数调用失败");
	return false;
}
//
// 自动生成中间变量到变量表中
//
string SemanticAnalyzer::GenerateTmpVarObject(){

	// 初始化变量对象
	VAROBJECT vObj;
	vObj.type = VAROBJECT::_NUM;
	vObj.isInitial = false;

	/* 注意:中间变量不需要被初始化，找相关操作数直接去遍历生成的四元式即可*/
	string name = "@Tmp" + to_string(tmpVarObjectPostifix++);
	vObj.name = name;
	
	// 存储到变量表中,这个由于自动生成后缀,因此
	tmpVarTable[name] = vObj;

	return name;
}

PVAROBJECT SemanticAnalyzer::outVarObjectPointer(string varName) {
	
	if (varTable.count(varName)) return &varTable[varName];
	else if (tmpVarTable.count(varName)) return &tmpVarTable[varName];
	else {
		CERR(("无法从变量表中获取该变量值"+varName));
		return nullptr;
	}

}
