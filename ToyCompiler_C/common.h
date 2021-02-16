#pragma once
#include <iostream>
using namespace std;
#define CERR(_REASON) { \
	cerr << _REASON << endl; \
	cerr << "prorgram is exitting..." << endl; \
	exit(0); \
} 



// 单个变量数据结构
typedef struct  _VarObject {
	string name; // 变量名
	bool isInitial; // 该变量是否被初始化
	enum { _NUM=1, _STR } type; // [数字,字符串]
	struct _value{
		int num;
		string str;
	} value;
} *PVAROBJECT, VAROBJECT;

struct AlgExprReturnValue {
	enum{_NUM,_VAR,_TMP_VAR} type;
	struct {
		int num; // 数字
		PVAROBJECT pVarObject; // 变量在变量表中的指针
	} value;
};

// 四元式中的操作符（首个元素）
enum OperatorCode {
	OP_ASSIGN, OP_ADD, OP_MINUS, OP_TIMES, OP_DIVIDE, // = + - * /
	OP_JG, OP_JGE, OP_JL, OP_JLE, OP_JE, OP_JNE, // >  >=  < <= == !=
	OP_JMP, OP_CMP,// cmp jmp
	OP_IN, OP_OUT, OP_STRASSIGN, OP_END // cin, cout , @ ,end
};

// 四元式中省下的三个成员（其中可能为中间变量，变量或者数字）
typedef struct _QUADMEMBER {
	// 0 该操作数为空 1 表示数字, 2表示变量，3表示中间变量,4 表示地址，5表示字符串
	enum TYPE { _EMPTY = 0, _NUM, _VAR, _TMP_VAR, _ADDRESS, _STRING };
	enum TYPE type;
	struct{
		int num;	// 数字
		PVAROBJECT varPointer; // 变量对象（来自变量表 或 中间变量表）
		int address; // 跳转语句时的地址
		string str; // 字符串
	} value;
}QUADMEMBER, * PQUADMEMBER;

// 生成四元式
typedef struct _QUADDSTATMENT {
	OperatorCode opCode; // 操作码
	QUADMEMBER op1;	// 操作数1
	QUADMEMBER op2; // 操作数2
	QUADMEMBER opResult; // 操作结果
}QUADSTATMENT, * PQUADSTATMENT;

// 中间代码元素
typedef struct _IntermediaCodeElement {
	typedef enum { CODE_BLOCK_NULL = 0, CODE_BLOCK_START, CODE_BLOCK_END, CODE_BLOCK_DELETE } STATUS;
	int id;		// 四元式的id序号(依次增加)
	STATUS codeStatus; // 中间代码状态（比如 1表示代码块开始，2表示代码块结束，3表示该代码块优化时被删除）
	QUADSTATMENT qStatment; // 中间代码的四元式
}IntermediaCodeElement, * PIntermediaCodeElement;