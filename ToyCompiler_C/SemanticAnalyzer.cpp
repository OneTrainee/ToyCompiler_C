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
		CERR(("�����Ѵ���:" + varName + " AddNewVarObjectIntoVarTable ����ʧ��"));
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
	// �ȳ��Դӱ������л�ȡ
	if (varTable.count(varName) > 0) {
		pVarObj = &varTable[varName];
		return true;
	}

	// ������м�������л�ȡ
	if (tmpVarTable.count(varName) > 0) {
		pVarObj = &tmpVarTable[varName];
		return true;
	}

	// �����������
	CERR("�ӱ������л�ȡ����ʧ��,varName:" + varName + "GetTheExisittingVarObjectFromVarTable��������ʧ��");
	return false;
}
//
// �Զ������м��������������
//
string SemanticAnalyzer::GenerateTmpVarObject(){

	// ��ʼ����������
	VAROBJECT vObj;
	vObj.type = VAROBJECT::_NUM;
	vObj.isInitial = false;

	/* ע��:�м��������Ҫ����ʼ��������ز�����ֱ��ȥ�������ɵ���Ԫʽ����*/
	string name = "@Tmp" + to_string(tmpVarObjectPostifix++);
	vObj.name = name;
	
	// �洢����������,��������Զ����ɺ�׺,���
	tmpVarTable[name] = vObj;

	return name;
}

PVAROBJECT SemanticAnalyzer::outVarObjectPointer(string varName) {
	
	if (varTable.count(varName)) return &varTable[varName];
	else if (tmpVarTable.count(varName)) return &tmpVarTable[varName];
	else {
		CERR(("�޷��ӱ������л�ȡ�ñ���ֵ"+varName));
		return nullptr;
	}

}
