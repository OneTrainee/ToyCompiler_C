#include "IntermediaCodeGenerator.h"
#include "common.h"
#include <vector>

// opCode��Ӧ�����ƣ���ʾ����ֱ�ۡ�
const char* opCodeName[] = {
	"=","+","-","*","/",
	"j>","j>=","j<","j<=","j==","j!==",
	"jmp","cmp",
	"in","out","@","end"
};

int IntermediaCodeGenerator::currentId = 0;

// �� AlgExprReturnValue ת���� QUADMEMBER
QUADMEMBER IntermediaCodeGenerator::TransferQuadMemberFromAlgExprReturnValue(AlgExprReturnValue a) {
	QUADMEMBER q;

	// ���͸�ֵ
	if (a.type == AlgExprReturnValue::_NUM) {
		q.type = QUADMEMBER::_NUM;
		q.value.num = a.value.num;
	}
	else {
		q.type = a.type == AlgExprReturnValue::_VAR ? QUADMEMBER::_VAR: QUADMEMBER::_TMP_VAR;
		q.value.varPointer = a.value.pVarObject;
	}

	return q;


}
QUADSTATMENT IntermediaCodeGenerator::GenerateQuadStatment(OperatorCode _opCode, QUADMEMBER _op1, QUADMEMBER _op2, QUADMEMBER _opResult) {
	QUADSTATMENT qStatment;
	qStatment.opCode = _opCode;
	qStatment.op1 = _op1;
	qStatment.op2 = _op2;
	qStatment.opResult = _opResult;
	return qStatment;
}

// ����Ԫʽ�����м�����Ա�����뵽�м���������(�ȴ��Ż���������룩
void IntermediaCodeGenerator::InsertQuadStatmentIntoIntermediaCodeList(QUADSTATMENT q) {
	IntermediaCodeElement e;
	e.id = IntermediaCodeGenerator::currentId++;
	e.qStatment = q;
	e.codeStatus = IntermediaCodeElement::CODE_BLOCK_NULL; // �м����ĳ�ʼ״̬��Ϊ��

	// ���뵽 IntermediaCodeList �У���־���м�����Ѿ�����
	IntermediaCodeGenerator::intermediaCodeList.push_back(e);

}

void IntermediaCodeGenerator::ShowIntermediaCodeList() {
	for (auto it : intermediaCodeList) {
		// ���id
		cout << it.id << ",(";

		// ��� opCode
		cout << opCodeName[it.qStatment.opCode] <<",";

		// enum TYPE { _EMPTY = 0, _NUM, _VAR, _TMP_VAR, _ADDRESS, _STRING };
		// ��� op1 
		QUADMEMBER::TYPE type = it.qStatment.op1.type;
		if (type == QUADMEMBER::_NUM) {
			cout << it.qStatment.op1.value.num << ",";
		}
		else if (type == QUADMEMBER::_EMPTY) {
			cout << "__,";
		}
		else if (type == QUADMEMBER::_STRING) {
			cout << it.qStatment.op1.value.str << ",";
		}
		else if(type == QUADMEMBER::_TMP_VAR || type == QUADMEMBER::_VAR){
			cout << it.qStatment.op1.value.varPointer->name << ",";
		}

		// ���op2
		type = it.qStatment.op2.type;
		if (type == QUADMEMBER::_NUM) {
			cout << it.qStatment.op2.value.num << ",";
		}
		else if (type == QUADMEMBER::_EMPTY) {
			cout << "__,";
		}
		else if (type == QUADMEMBER::_STRING) {
			cout << it.qStatment.op2.value.str << ",";
		}
		else if (type == QUADMEMBER::_TMP_VAR || type == QUADMEMBER::_VAR) {
			cout << it.qStatment.op2.value.varPointer->name << ",";
		}

		// ��� opResult;
		type = it.qStatment.opResult.type;
		if (type == QUADMEMBER::_NUM) {
			cout << it.qStatment.opResult.value.num << ")";
		}
		else if (type == QUADMEMBER::_EMPTY) {
			cout << "__)";
		}
		else if (type == QUADMEMBER::_STRING) {
			cout << it.qStatment.opResult.value.str << ")";
		}
		else if (type == QUADMEMBER::_TMP_VAR || type == QUADMEMBER::_VAR) {
			cout << it.qStatment.opResult.value.varPointer->name << ")";
		}
		else if (type == QUADMEMBER::_ADDRESS) {
			cout << it.qStatment.opResult.value.address << ")";
		}

		cout << endl;
	}
}
