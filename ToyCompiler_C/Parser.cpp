#include "Parser.h"
#include "common.h"


int IfWithoutElseButton = 0;


inline void _COUT(string s) {
	cout << s << endl;
}

// 
Parser::Parser(string resource_file_name) :lexical(resource_file_name) {}

// <����>-><main�ؼ���>{<�����������><�������>}#
void Parser::Program() {
	
	_COUT("<����>-><main�ؼ���>{<�����������><�������>}#");
	
	lexical.getCh(); // ����Ԥ�ȶ�ȡ��һ���ַ�
	
	lexical.getSym();
	if (lexical.outSym() == KW_MAIN) { /* ���Ի�ȡ main */
		lexical.getSym();
		if (lexical.outSym() == LBRACE) { /* ���Ի�ȡ { */

			lexical.getSym(); /* ���Ի�ȡ string �� int ��Ϊ�������������׼�� ..*/
			DeclarationList(); 
			StatmentList();

			lexical.getSym(); /* ���Զ�ȡ # */
			if (lexical.outSym() == END) {
				End();
				return; /* ������������,������һ��*/
			}
			
		}
		else {

		}
	}
	else {
		CERR("Error:<����>-><main�ؼ���>{<�����������><�������>}# ");
	}
}

// DeclarationList:<�����������> -> <�������><����������н�β>
void Parser::DeclarationList() {
	_COUT("DeclarationList:<�����������> -> <�������><����������н�β>");

	// �жϵ�ǰ�����Ƿ������������Ķ���
	if (lexical.outSym() == KW_INT || lexical.outSym() == KW_STRING) {
		/*ע��������Ҫ�ã���ʱ����Ҫ��ǰ����ȡ*/
		Declaration();
		DeclarationListTail();
	}
}

// Declaration:<�������>-><int/string�ؼ���(type)> ������ <��������β>
void Parser::Declaration() {
	_COUT("Declaration:<�������>-><int/string�ؼ���(type)> ������ <��������β>");
	
	if (lexical.outSym() == KW_INT || lexical.outSym() == KW_STRING) {
		
		// �ж�������int����string...
		bool isStr = lexical.outSym() == KW_STRING ? 1 : 0;

		lexical.getSym(); /* ���Զ�ȡ������ */
		if (lexical.outSym() == ID) {

			// ��ȡ������ֱ����ӽ�������
			semantic.AddNewVarObjectIntoVarTable(isStr, lexical.outVarName());

			// ��������,�����Ƿ��Ǿ��ӽ�β
			DeclarationTail(isStr);
		}
		else {
			
			//
			// ���򱨴�...
			//
		}
	}
}


void Parser::DeclarationTail(bool isStr) {
	_COUT("DeclarationTail:<��������β>-> , <������><��������β> | ;");

	lexical.getSym(); /* ���Զ�ȡ[','|';'] */
	if (lexical.outSym() == SEMI) { // ������';',������������.
		return;
	}
	else if (lexical.outSym() == COMMA) { // �����������,
		lexical.getSym(); /* ���Զ�ȡ������ */

		// ��ȡ������ֱ����ӽ�������
		semantic.AddNewVarObjectIntoVarTable(isStr, lexical.outVarName());

		// ��������,�����Ƿ��Ǿ��ӽ�β
		DeclarationTail(isStr);
	}
	else {

		//
		// ���򱨴�...
		//
	}

}

// DeclarationListTail:<����������н�β> -> <�������><����������н�β> | ��
void Parser::DeclarationListTail() {
	_COUT("DeclarationListTail:<����������н�β> -> <�������><����������н�β> | ��");

	lexical.getSym();
	if (lexical.outSym() == KW_INT || lexical.outSym() == KW_STRING) { // ��Ȼ�����µ��������
		Declaration();
		DeclarationListTail();
	}
	else {
		// �������������
		return;
	}
}

void Parser::End() {
	_COUT("<����> -> #");
	
	semantic.ShowVarTable();
	inter_code_generator.ShowIntermediaCodeList();
	
	//���滹�кܶ������Ҫ�����ƴ���..
}


// <�������>-><���><������н�β>\n
void Parser::StatmentList() {
	_COUT("<�������>-><���><������н�β>");
	Parser::Statment();
	Parser::StatmentListTail();
}


// StatmentListTail:<������н�β>-><���><������н�β>|��
void Parser::StatmentListTail() {
	_COUT("<������н�β>-><���><������н�β>|��");
	/*
	ע�⣺
		1.��Ϊÿһ����䶼����ȷ�Ľ��������Բ�����Ԥ��ȡ��
		���ԣ������������Ҫ��Ԥ�ȶ�����һ����������ֻҪ�� '}',
		���� <����>}��˵��main���������ˡ�
		�����ڸ���������һ�����⣬�������������ǵ��á�
		2. ����if/else�����Ǳ���Ԥ�ȶ�ȡһ��else��������ǣ�
		�ٽ���������ٳ�ǰ��ȡ����ˣ��������һ��Ĭ�ϱ�����
		��if��else�ľ��ӽ�����ʱ����ֹͣ��ȡ���ɡ�
	��⣺StatmentListTail() ֻ��֤�Ƿ��� }��������main���� <���Ϻ�����>��
		�Ⲣ����Ҫ����ȡ�������ĸ������С�
	*/
	if (IfWithoutElseButton == 0) {
		lexical.getSym(); /* Ԥ��ȡ���ӵĿ�ʼ���� */
	}
	else {
		IfWithoutElseButton = 1;
	}


	// ���Ϊ '}'�����������
	if (lexical.outSym() == RBRACE) {
		return;
	}
	else {
		// ��Ϊ }���������������
		Statment();

		/*
			ע��:���ھ��ӣ�����ȷ�Ľ�β���϶��������Ԥ��ȡ
			�󵨵��� StatmentListTail(); �俪ʼ��ȡ���ӿ�ͷ����
		*/
		StatmentListTail();
	}

}

// Statement:<���>-> <if���> | <while���> | <in���> | <out���> | <��ֵ���> | <�������>
void Parser::Statment() {
	_COUT("<���>-> <if���> | <while���> | <in���> | <out���> | <��ֵ���> | <�������>");
	/*֮ǰ�Ѿ�Ԥ��ȡ��һ������˲��ü�����ȡ*/

	// ֱ��ʹ��switch() �ж���������Ӧ��������
	switch (lexical.outSym()) {
	case ID:	// ��ֵ���
		AssignStatment();
		break;
	case KW_IF: // if ���
		IfStatement();
		break;
	case KW_WHILE: // while���
		WhileStatement();
		break;
	case KW_CIN: // cin ���
		Parser::InStatement();
		break;
	case KW_COUT: // cout ���
		Parser::OutStatement();
		break;

		/*֮��ÿд��һ�����������Ｔ��*/
		/*����������������������������*/
	}
}

// IfStatement<if���> -> <if�ؼ���>(<�������ʽ>)<�������>else<�������> |  <if�ؼ���>(<�������ʽ>)<�������>
void Parser::IfStatement() {
	
	_COUT("<if���> -> <if�ؼ���>(<�������ʽ>)<�������>else<�������> |  <if�ؼ���>(<�������ʽ>)<�������>");

	if (lexical.outSym() != KW_IF) {
		//
		// �������
		//
	}

	// ���Զ�ȡ'('
	lexical.getSym();
	if (lexical.outSym() != LPAREN) {
		//
		// ���򱨴�
		//
	}

	// ���� <�������ʽ>,��ȡindex�ȴ�����,�ڴ���else֮�������
	int indexFalse = BoolExpr(); // ���ȡһ���ַ�
	int indexTrue = 0; 

	// ���Զ�ȡ ')
	if (lexical.outSym() != RPAREN) {

		//
		// ���򱨴�
		//
	}

	// 
	// ���ԶԸ��������з���
	//
	
	// ���Զ�ȡ '{'
	lexical.getSym();
	if (lexical.outSym() == LBRACE) {
		CompoundStatment();
	}
	else {
		//
		// ���򱨴�
		//
	}

	//
	// �����������Ƿ���� else �ؼ���
	// 
	lexical.getSym();
	if (lexical.outSym() != KW_ELSE) {
		// ȷ������û��else,�����ֱ�ӻ�����ת���
		inter_code_generator.intermediaCodeList[indexFalse].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;
		
		// ���������,����Ԥ��ȡ
		IfWithoutElseButton = 1;
		return;
	}
	else {

		IfWithoutElseButton = 0;
		// ��Ϊelse���Ĵ��ڣ���������ִ��֮��������һ����ת 
		/*******************************
			--------------
			| jx -----> a |
			---------------
			|  ��ȷ���xx |
			---------------
			| jmp--> end  |
			---------------
			|  �������xx | a
			---------------
			|    end      |
			---------------
		*******************************/

		// ����һ����Ԫʽ,��ʾTrue��ת�ĵ�ַ,�ȴ�����
		QUADMEMBER _op1;
		_op1.type = QUADMEMBER::_EMPTY;
		QUADMEMBER _op2;
		_op2.type = QUADMEMBER::_EMPTY;
		QUADMEMBER _op3;
		_op3.type = QUADMEMBER::_ADDRESS; 

		QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_JMP, _op1, _op2, _op3);
		inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);
		indexTrue = inter_code_generator.intermediaCodeList.size() - 1;

		// �ٴν���else���
		inter_code_generator.intermediaCodeList[indexFalse].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;

		// ���Զ�ȡ '{' 
		lexical.getSym();
		if (lexical.outSym() == LBRACE) {

			// ���и������
			CompoundStatment();

			// ����True_Jmp�ĵ�ַ
			inter_code_generator.intermediaCodeList[indexTrue].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;
		}
		else {

			//
			// �������
			//
		}

	}


}


// IfWhile:<while���> -> <while�ؼ���>(�������ʽ)<�������>
void Parser::WhileStatement() {
	_COUT("IfWhile:<while���> -> <while�ؼ���>(�������ʽ)<�������>");

	// 1.�ڽ��벼�����ʽ֮ǰ,Ԥ�ȼ�¼�м�����id,index
	int inIndex = IntermediaCodeGenerator::currentId;

	// 2&&3.���в������ʽ���� | ��¼�Ӳ������ʽ��ȡ���м����ID - outIndex
	lexical.getSym();

	// ���Զ�ȡ '('
	if (lexical.outSym() != LPAREN) { 
		//
		// ����
		//
		CERR("error (");
	}

	// ���벼�����ʽ
	int outIndex = BoolExpr();

	// ���Զ�ȡ')'
	if (lexical.outSym() != RPAREN) {
		//
		// ����
		//
		CERR("error )");
	}

	// 4. ����<�������>����
	lexical.getSym();
	if (lexical.outSym() != LBRACE) {
		//
		// ����
		//
	}
	CompoundStatment(); // <�������>����

	// 5. ���м�������һ���޳���ת��inIndex
	QUADMEMBER op1;
	op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER op2;
	op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER opResult;
	opResult.type = QUADMEMBER::_ADDRESS;
	opResult.value.address = inIndex;
	
	QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_JMP, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

	// 6. ��ǰ�м����ID��д��outIndex��,Ȼ���˳�
	inter_code_generator.intermediaCodeList[outIndex].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;

}

/*****************************************************
			�������ľ���ʵ��
******************************************************/

// AssignStatment:<��ֵ���> -> <������> = <�������ʽ>;|<������> @ <�ַ���>;
void Parser::AssignStatment() {
	_COUT("AssignStatment:<��ֵ���> -> <������> = <�������ʽ>;|<������> @ <�ַ���>;");
	
	// 1���������
	string varName = lexical.outVarName();
	AlgExprReturnValue res;		// �������ʽ���صĽ��

	// 2�����Զ�ȡ '='
	lexical.getSym();
	if (lexical.outSym() == ASSIGN) {
		// 3����ȡ����������Ԥ��ȡһλ ;)
		res = AlgExrp();

		// 4����ʼ������Ԫʽ�е�op1����������������
		QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(res);

		// 5��������Ԫʽ�е�op2(��,0)
		QUADMEMBER op2;
		op2.type = QUADMEMBER::_EMPTY;


		// 6����ʼ������Ԫʽ�е�opResult
		QUADMEMBER opResult;
		opResult.type = QUADMEMBER::_VAR; // ����ֵ���һ��Ϊ����,�����ӹ����..
		semantic.GetTheExistentVarObjectFromVarTable(varName, opResult.value.varPointer);
		
		// 7�������м�Ԫ�ش��벢��ӽ��м���������
		QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_ASSIGN, op1, op2, opResult);
		inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

		// 
		if (lexical.outSym() == SEMI) {
			return;
		}
		else {

			//
			// ����.
			//
		}

	}
}

// CompoundStatment:<�������> -> {<�������>}
void Parser::CompoundStatment() {
	_COUT("CompoundStatment:<�������> -> {<�������>}");

	// ֻҪִ���⺯������ǰ sym һ��Ϊ {������Ԥ�ȶ�ȡ��
		// ���г�����֤����
	if (lexical.outSym() == LBRACE) {
		StatmentList();
		/*ע��:
			1. StatmentListTail()��Ԥ�ȶ�ȡһ��sym��������֤
			2. ���ֱ�ӳ�����֤���ɡ�
		*/
		if (lexical.outSym() == RBRACE) {
			// ������������ʱ���� IfWithoutElseButton �Զ��رա�
			// ��Ϊ���ǹ�����һ�㣬��ifǶ��ʱ����һ�������ض���Ӱ����һ��ġ�
			// ����Ȼ�ǲ�����ġ�
			IfWithoutElseButton = 0;
			return;
		}
		else {
			//
			// ����
			//
		}
	}
	else {
		//
		// ����
		// 
	}
}

// InStatement:<in���> -> <in�ؼ���>  >> <������>
void Parser::InStatement() {
	_COUT("InStatement:<in���> -> <in�ؼ���>  >> <������>");

	if (lexical.outSym() != KW_CIN) {
		//
		// ���򱨴�
		//
	}
	lexical.getSym(); /* ��ȡ '>>' */
	if (lexical.outSym() != IN) {
		//
		// ���򱨴�
		//
	}
	// ��ȡ <������> 
	lexical.getSym();

	// ���ɶ�Ӧ���м���� (IN,__,__,<������>)
	QUADMEMBER op1;
	op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER op2;
	op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER opResult;

	// �������
	if (lexical.outSym() == ID) {
		opResult.type = QUADMEMBER::_VAR;
		if (semantic.GetTheExistentVarObjectFromVarTable(lexical.outVarName(), opResult.value.varPointer) == false) {
			// 
			// �ڸñ������в����ڸñ���,����..
			//
		}
	}
	else {
		//
		// ���ǺϷ��ı�����,���ﱨ��
		//
	}

	QUADSTATMENT qStatment = inter_code_generator.GenerateQuadStatment(OP_IN, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatment);

	// ��ȡ ';'��������
	lexical.getSym();
	if (lexical.outSym() != SEMI) {
		//
		// �������
		//
	}

	return;

}

// OutStatement:<out���> -> <out�ؼ���>  << [<������>|<����>];
void Parser::OutStatement(){
	_COUT("OutStatement:<out���> -> <out�ؼ���>  << [<������>|<����>];");

	if (lexical.outSym() != KW_COUT) {
		//
		// ���򱨴�
		//
	}
	lexical.getSym(); /* ��ȡ '>>' */
	if (lexical.outSym() != OUT) {
		//
		// ���򱨴�
		//
	}
	// ��ȡ <������> 
	lexical.getSym();

	// ���ɶ�Ӧ���м���� (OUT,__,__,<������>)
	QUADMEMBER op1;
	op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER op2;
	op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER opResult;

	// �������
	if (lexical.outSym() == ID) {
		opResult.type = QUADMEMBER::_VAR;
		if (semantic.GetTheExistentVarObjectFromVarTable(lexical.outVarName(), opResult.value.varPointer) == false) {
			// 
			// �ڸñ������в����ڸñ���,����..
			//
		}
	}
	else {
		//
		// ���ǺϷ��ı�����,���ﱨ��
		//
	}

	QUADSTATMENT qStatment = inter_code_generator.GenerateQuadStatment(OP_OUT, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatment);

	// ��ȡ ';'��������
	lexical.getSym();
	if (lexical.outSym() != SEMI) {
		//
		// �������
		//
	}

	return;
}


// BoolExpr:<�������ʽ> -> <�������ʽ> [>|>=|<|<=|==|!=] <�������ʽ>
int Parser::BoolExpr() {
	_COUT("<�������ʽ> -> <�������ʽ> [>|>=|<|<=|==|!=] <�������ʽ>");
	
	/*
		ע�⣺��Ȼ<�������ʽ>����Ԥ�ȶ�ȡһ���ַ�����������õ�<�������ʽ>ȴ��Ԥ�ȶ�ȡһ������
			���<�������ʽ>��Ԥ�ȶ�ȡһ�����ţ�����Ҫ��ȷ��
	*/

	OperatorCode opCode;

	// 1�����Ի�ȡ���������ʽC
	AlgExprReturnValue lAlgReturnValue = Parser::AlgExrp();

	// 2����ȡ �Ƚ������,����֮��ķ���
	/*ע�⣺AlgExrp���ܻ�Ԥ�ȶ�ȡ���ʲ��ô洢*/
	if (lexical.outSym() == G || lexical.outSym()  == GE || lexical.outSym()  == B || lexical.outSym() == BE || lexical.outSym() ==EQ || lexical.outSym() ==NE) {
		switch (lexical.outSym()) {
		case G: // ���� -> С�ڵ��� OP_JLE
			opCode = OP_JLE;
			break;
		case GE: // ���ڵ��� -> С�� OP_JL
			opCode = OP_JL;
			break;
		case B: // С�� -> ���ڵ��� OP_JGE
			opCode = OP_JGE;
			break;
		case BE: // С�ڵ��� -> ���� OP_JG
			opCode = OP_JG;
			break;
		case EQ: // ���� -> ������ OP_JNE
			opCode = OP_JNE;
			break;
		case NE: // ������ -> ����
			opCode = OP_JE;
			break;
		}
	}
	else {
		
		//
		// ������
		//
	}

	// 3����ȡ����ʽ
	AlgExprReturnValue rAlgReturnValue = Parser::AlgExrp();


	QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(lAlgReturnValue);
	QUADMEMBER op2 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(rAlgReturnValue);
	QUADMEMBER opResult;
	opResult.type = QUADMEMBER::_EMPTY;


	QUADSTATMENT qStatment = inter_code_generator.GenerateQuadStatment(OP_CMP, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatment);

	// 4�������м���� (jmp_,_,address)��������Ԫʽ���б��е�ָ�룬��if����н��й���
	QUADMEMBER _op1;
	_op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER _op2;
	_op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER _op3;
	_op3.type = QUADMEMBER::_ADDRESS; // ��ʾ��ַ

	QUADSTATMENT _qStatment = inter_code_generator.GenerateQuadStatment(opCode, _op1, _op2, _op3);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(_qStatment);

	// 5�����ظ���ֵ�������ú���ֱ�ӻ�ȡ��Ӧ���������д���
	return inter_code_generator.intermediaCodeList.size() - 1;

}


/*****************************************************
			�������ʽ����������
******************************************************/
// AlgExpr<�������ʽ> -> <��>[+| -] <�������ʽβ> | <��> ��
AlgExprReturnValue Parser::AlgExrp() {
	_COUT("AlgExpr<�������ʽ> -> <��>[+| -] <�������ʽβ> | <��> ��");

	// 1 ��ȡ<����>
	AlgExprReturnValue lAlgReturnValue = Item();

	//
	// Warnning: Item() will read the next sym in advance.
	//			so we don't have to provoke getSym() in this function.
	//

	// 2 ���Ի�ȡ '+/-'
	if (lexical.outSym() == ADD) {
		return AlgExprTail(lAlgReturnValue, true);
	}
	else if (lexical.outSym() == MINUS) {
		return AlgExprTail(lAlgReturnValue, false);
	}
	else {
		return lAlgReturnValue;
	}

}

// Item:<��> -> <����> * <��β> | <����>
AlgExprReturnValue Parser::Item() {
	_COUT("Item:<��> -> <����> * <��β> | <����>");
	
	// 1�����ȳ��Ի�ȡ����
	AlgExprReturnValue lAlgReturnValue = Factor();

	// 2�����Ի�ȡ'*'
	lexical.getSym();

	// 3������� *����˵��������Ȼ����ʽ��
	if (lexical.outSym() == TIMES) {
		return ItemTail(lAlgReturnValue);
	}
	else {
		return lAlgReturnValue;
	}

	
}

// Factor:<����>-><����> | <������>
AlgExprReturnValue Parser::Factor() {
	_COUT("Factor:<����>-><����> | <������>");

	lexical.getSym();

	// �����������ֵ
	AlgExprReturnValue alg;
	
	if (lexical.outSym() == NUM) {
		alg.type = AlgExprReturnValue::_NUM;
		alg.value.num = lexical.outNum();
	}
	else if (lexical.outSym() == ID) {
		alg.type = AlgExprReturnValue::_VAR;
		// ���Ե�������������ӱ������л�ȡ��Ӧ����.
		semantic.GetTheExistentVarObjectFromVarTable(lexical.outVarName(), alg.value.pVarObject);
	}
	else {
		
		//
		// ������
		//
	}

	// ���غ��ѹ���õı��ʽ
	return alg;
}

// ItemTail:<��β> -> <����> * | <����> ��
AlgExprReturnValue Parser::ItemTail(AlgExprReturnValue lAlgReturnValue) {
	_COUT("ItemTail:<��β> -> <����> * | <����> ��");

	// 1) �ȶ�ȡ����.
	AlgExprReturnValue rAlgReturnValue = Parser::Factor();

	// 2) �����м����.
	string tmpVarName = semantic.GenerateTmpVarObject();
	AlgExprReturnValue resultAlgReturnValue;
	resultAlgReturnValue.type = AlgExprReturnValue::_TMP_VAR;
	resultAlgReturnValue.value.pVarObject = semantic.outVarObjectPointer(tmpVarName);

	// 3��������ֵת��Ϊ��Ԫʽ.
	QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(lAlgReturnValue);
	QUADMEMBER op2 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(rAlgReturnValue);
	QUADMEMBER opResult = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(resultAlgReturnValue);

	// 4�������м���벢�������
	QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_TIMES, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

	// 5�����Զ�ȡ '*'
	lexical.getSym();

	// 6������� '*'���������Ȼ��Ҫ�м���ģ��ȼ������ǰ���м������
	//	  �ٰѵ�ǰ�����ת��Ϊ�м���룬����м������Ϊ�������ݸ���һ����
	//	  ����Ϊ�ա� ����ķ�����ע�ⱻ��ǰ��ȡһ���ַ����ڸ�ֵ���ʽ�򲼶�ʽ��ע�⡣
	if (lexical.outSym() == TIMES) {
		return ItemTail(resultAlgReturnValue);
	}
	else {
		return resultAlgReturnValue;
	}
}

//AlgExprTail<�������ʽβ> -> [+|-] <�������ʽβ>|��
AlgExprReturnValue Parser::AlgExprTail(AlgExprReturnValue lAlgReturnValue, bool isAdd) {
	_COUT("AlgExprTail<�������ʽβ> -> [+|-] <�������ʽβ>|��");

	// 1���϶���������,�����ȶ�ȡ����
	AlgExprReturnValue rAlgReturnValue = Item();

	// 2�������м����
	string tmpVarName = semantic.GenerateTmpVarObject();
	AlgExprReturnValue resultAlgReturnValue;
	resultAlgReturnValue.type = AlgExprReturnValue::_TMP_VAR;
	resultAlgReturnValue.value.pVarObject = semantic.outVarObjectPointer(tmpVarName);

	// 3��������ֵת������Ԫʽ
	QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(lAlgReturnValue);
	QUADMEMBER op2 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(rAlgReturnValue);
	QUADMEMBER opResult = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(resultAlgReturnValue);

	// 4�������м���벢���������
	enum OperatorCode opCode = isAdd == true ? OP_ADD : OP_MINUS;
	QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(opCode, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

	// ע��: Parser::Item()��Ԥ��ȡ���������ﲻ�ö�ȡ
	//LexicalAnalyzer::getSym();
	
	// 5������sym
	if (lexical.outSym() == ADD) {
		return AlgExprTail(resultAlgReturnValue, true);
	}
	else if(lexical.outSym() == MINUS){
		return AlgExprTail(resultAlgReturnValue, false);
	}
	else {
		return resultAlgReturnValue;
	}
}
