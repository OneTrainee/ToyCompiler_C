#pragma once
#include <iostream>
using namespace std;
#define CERR(_REASON) { \
	cerr << _REASON << endl; \
	cerr << "prorgram is exitting..." << endl; \
	exit(0); \
} 



// �����������ݽṹ
typedef struct  _VarObject {
	string name; // ������
	bool isInitial; // �ñ����Ƿ񱻳�ʼ��
	enum { _NUM=1, _STR } type; // [����,�ַ���]
	struct _value{
		int num;
		string str;
	} value;
} *PVAROBJECT, VAROBJECT;

struct AlgExprReturnValue {
	enum{_NUM,_VAR,_TMP_VAR} type;
	struct {
		int num; // ����
		PVAROBJECT pVarObject; // �����ڱ������е�ָ��
	} value;
};

// ��Ԫʽ�еĲ��������׸�Ԫ�أ�
enum OperatorCode {
	OP_ASSIGN, OP_ADD, OP_MINUS, OP_TIMES, OP_DIVIDE, // = + - * /
	OP_JG, OP_JGE, OP_JL, OP_JLE, OP_JE, OP_JNE, // >  >=  < <= == !=
	OP_JMP, OP_CMP,// cmp jmp
	OP_IN, OP_OUT, OP_STRASSIGN, OP_END // cin, cout , @ ,end
};

// ��Ԫʽ��ʡ�µ�������Ա�����п���Ϊ�м�����������������֣�
typedef struct _QUADMEMBER {
	// 0 �ò�����Ϊ�� 1 ��ʾ����, 2��ʾ������3��ʾ�м����,4 ��ʾ��ַ��5��ʾ�ַ���
	enum TYPE { _EMPTY = 0, _NUM, _VAR, _TMP_VAR, _ADDRESS, _STRING };
	enum TYPE type;
	struct{
		int num;	// ����
		PVAROBJECT varPointer; // �����������Ա����� �� �м������
		int address; // ��ת���ʱ�ĵ�ַ
		string str; // �ַ���
	} value;
}QUADMEMBER, * PQUADMEMBER;

// ������Ԫʽ
typedef struct _QUADDSTATMENT {
	OperatorCode opCode; // ������
	QUADMEMBER op1;	// ������1
	QUADMEMBER op2; // ������2
	QUADMEMBER opResult; // �������
}QUADSTATMENT, * PQUADSTATMENT;

// �м����Ԫ��
typedef struct _IntermediaCodeElement {
	typedef enum { CODE_BLOCK_NULL = 0, CODE_BLOCK_START, CODE_BLOCK_END, CODE_BLOCK_DELETE } STATUS;
	int id;		// ��Ԫʽ��id���(��������)
	STATUS codeStatus; // �м����״̬������ 1��ʾ����鿪ʼ��2��ʾ����������3��ʾ�ô�����Ż�ʱ��ɾ����
	QUADSTATMENT qStatment; // �м�������Ԫʽ
}IntermediaCodeElement, * PIntermediaCodeElement;