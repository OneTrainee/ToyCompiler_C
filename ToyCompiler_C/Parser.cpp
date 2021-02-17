#include "Parser.h"
#include "common.h"


int IfWithoutElseButton = 0;


inline void _COUT(string s) {
	cout << s << endl;
}

// 
Parser::Parser(string resource_file_name) :lexical(resource_file_name) {}

// <程序>-><main关键字>{<声明语句序列><语句序列>}#
void Parser::Program() {
	
	_COUT("<程序>-><main关键字>{<声明语句序列><语句序列>}#");
	
	lexical.getCh(); // 必须预先读取第一个字符
	
	lexical.getSym();
	if (lexical.outSym() == KW_MAIN) { /* 尝试获取 main */
		lexical.getSym();
		if (lexical.outSym() == LBRACE) { /* 尝试获取 { */

			lexical.getSym(); /* 尝试获取 string 或 int ，为分析声明语句做准备 ..*/
			DeclarationList(); 
			StatmentList();

			lexical.getSym(); /* 尝试读取 # */
			if (lexical.outSym() == END) {
				End();
				return; /* 语义分析已完成,进行下一步*/
			}
			
		}
		else {

		}
	}
	else {
		CERR("Error:<程序>-><main关键字>{<声明语句序列><语句序列>}# ");
	}
}

// DeclarationList:<声明语句序列> -> <声明语句><声明语句序列结尾>
void Parser::DeclarationList() {
	_COUT("DeclarationList:<声明语句序列> -> <声明语句><声明语句序列结尾>");

	// 判断当前符号是否满足声明语句的定义
	if (lexical.outSym() == KW_INT || lexical.outSym() == KW_STRING) {
		/*注：接下来要用，此时不需要提前、读取*/
		Declaration();
		DeclarationListTail();
	}
}

// Declaration:<声明语句>-><int/string关键字(type)> 变量名 <声明语句结尾>
void Parser::Declaration() {
	_COUT("Declaration:<声明语句>-><int/string关键字(type)> 变量名 <声明语句结尾>");
	
	if (lexical.outSym() == KW_INT || lexical.outSym() == KW_STRING) {
		
		// 判断类型是int还是string...
		bool isStr = lexical.outSym() == KW_STRING ? 1 : 0;

		lexical.getSym(); /* 尝试读取变量名 */
		if (lexical.outSym() == ID) {

			// 读取变量后直接添加进变量表
			semantic.AddNewVarObjectIntoVarTable(isStr, lexical.outVarName());

			// 继续分析,看看是否是句子结尾
			DeclarationTail(isStr);
		}
		else {
			
			//
			// 程序报错...
			//
		}
	}
}


void Parser::DeclarationTail(bool isStr) {
	_COUT("DeclarationTail:<声明语句结尾>-> , <变量名><声明语句结尾> | ;");

	lexical.getSym(); /* 尝试读取[','|';'] */
	if (lexical.outSym() == SEMI) { // 若读到';',则这条语句结束.
		return;
	}
	else if (lexical.outSym() == COMMA) { // 如果读到逗号,
		lexical.getSym(); /* 尝试读取变量名 */

		// 读取变量后直接添加进变量表
		semantic.AddNewVarObjectIntoVarTable(isStr, lexical.outVarName());

		// 继续分析,看看是否是句子结尾
		DeclarationTail(isStr);
	}
	else {

		//
		// 程序报错...
		//
	}

}

// DeclarationListTail:<声明语句序列结尾> -> <声明语句><声明语句序列结尾> | 空
void Parser::DeclarationListTail() {
	_COUT("DeclarationListTail:<声明语句序列结尾> -> <声明语句><声明语句序列结尾> | 空");

	lexical.getSym();
	if (lexical.outSym() == KW_INT || lexical.outSym() == KW_STRING) { // 仍然存在新的声明语句
		Declaration();
		DeclarationListTail();
	}
	else {
		// 声明语句分析完毕
		return;
	}
}

void Parser::End() {
	_COUT("<结束> -> #");
	
	semantic.ShowVarTable();
	inter_code_generator.ShowIntermediaCodeList();
	
	//后面还有很多代码需要来完善处理..
}


// <语句序列>-><语句><语句序列结尾>\n
void Parser::StatmentList() {
	_COUT("<语句序列>-><语句><语句序列结尾>");
	Parser::Statment();
	Parser::StatmentListTail();
}


// StatmentListTail:<语句序列结尾>-><语句><语句序列结尾>|空
void Parser::StatmentListTail() {
	_COUT("<语句序列结尾>-><语句><语句序列结尾>|空");
	/*
	注意：
		1.因为每一条语句都有明确的结束，所以不存在预读取。
		所以，我们这里必须要再预先读出来一个，读出来只要是 '}',
		比如 <句子>}，说明main函数结束了。
		但对于复合语句存在一点问题，即复合语句最后是调用。
		2. 对于if/else，我们必须预先读取一个else，如果不是，
		再进这里会又再超前读取，因此，我们设计一个默认变量，
		当if无else的句子进这里时，就停止读取即可。
	理解：StatmentListTail() 只验证是否是 }，至于是main还是 <复合函数的>，
		这并不重要，这取决于在哪个函数中。
	*/
	if (IfWithoutElseButton == 0) {
		lexical.getSym(); /* 预读取句子的开始部分 */
	}
	else {
		IfWithoutElseButton = 1;
	}


	// 如果为 '}'，则结束分析
	if (lexical.outSym() == RBRACE) {
		return;
	}
	else {
		// 不为 }，则继续分析句子
		Statment();

		/*
			注意:对于句子，有明确的结尾，肯定不会存在预读取
			大胆调用 StatmentListTail(); 其开始读取句子开头即可
		*/
		StatmentListTail();
	}

}

// Statement:<语句>-> <if语句> | <while语句> | <in语句> | <out语句> | <赋值语句> | <复合语句>
void Parser::Statment() {
	_COUT("<语句>-> <if语句> | <while语句> | <in语句> | <out语句> | <赋值语句> | <复合语句>");
	/*之前已经预读取过一个，因此不用继续读取*/

	// 直接使用switch() 判断来调用相应函数即可
	switch (lexical.outSym()) {
	case ID:	// 赋值语句
		AssignStatment();
		break;
	case KW_IF: // if 语句
		IfStatement();
		break;
	case KW_WHILE: // while语句
		WhileStatement();
		break;
	case KW_CIN: // cin 语句
		Parser::InStatement();
		break;
	case KW_COUT: // cout 语句
		Parser::OutStatement();
		break;

		/*之后每写完一个补充在这里即可*/
		/*··············*/
	}
}

// IfStatement<if语句> -> <if关键字>(<布尔表达式>)<复合语句>else<复合语句> |  <if关键字>(<布尔表达式>)<复合语句>
void Parser::IfStatement() {
	
	_COUT("<if语句> -> <if关键字>(<布尔表达式>)<复合语句>else<复合语句> |  <if关键字>(<布尔表达式>)<复合语句>");

	if (lexical.outSym() != KW_IF) {
		//
		// 程序出错
		//
	}

	// 尝试读取'('
	lexical.getSym();
	if (lexical.outSym() != LPAREN) {
		//
		// 程序报错
		//
	}

	// 进入 <布尔表达式>,获取index等待回填,在处理else之后将其更新
	int indexFalse = BoolExpr(); // 会读取一个字符
	int indexTrue = 0; 

	// 尝试读取 ')
	if (lexical.outSym() != RPAREN) {

		//
		// 程序报错
		//
	}

	// 
	// 尝试对复合语句进行分析
	//
	
	// 尝试读取 '{'
	lexical.getSym();
	if (lexical.outSym() == LBRACE) {
		CompoundStatment();
	}
	else {
		//
		// 程序报错
		//
	}

	//
	// 现在来分析是否存在 else 关键字
	// 
	lexical.getSym();
	if (lexical.outSym() != KW_ELSE) {
		// 确定后面没有else,则可以直接回填跳转语句
		inter_code_generator.intermediaCodeList[indexFalse].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;
		
		// 打开这个开关,不会预读取
		IfWithoutElseButton = 1;
		return;
	}
	else {

		IfWithoutElseButton = 0;
		// 因为else语句的存在，正常代码执行之后必须存在一个跳转 
		/*******************************
			--------------
			| jx -----> a |
			---------------
			|  正确语句xx |
			---------------
			| jmp--> end  |
			---------------
			|  错误语句xx | a
			---------------
			|    end      |
			---------------
		*******************************/

		// 生成一个四元式,表示True跳转的地址,等待回填
		QUADMEMBER _op1;
		_op1.type = QUADMEMBER::_EMPTY;
		QUADMEMBER _op2;
		_op2.type = QUADMEMBER::_EMPTY;
		QUADMEMBER _op3;
		_op3.type = QUADMEMBER::_ADDRESS; 

		QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_JMP, _op1, _op2, _op3);
		inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);
		indexTrue = inter_code_generator.intermediaCodeList.size() - 1;

		// 再次进行else语句
		inter_code_generator.intermediaCodeList[indexFalse].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;

		// 尝试读取 '{' 
		lexical.getSym();
		if (lexical.outSym() == LBRACE) {

			// 运行复合语句
			CompoundStatment();

			// 回填True_Jmp的地址
			inter_code_generator.intermediaCodeList[indexTrue].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;
		}
		else {

			//
			// 程序出错
			//
		}

	}


}


// IfWhile:<while语句> -> <while关键字>(布尔表达式)<复合语句>
void Parser::WhileStatement() {
	_COUT("IfWhile:<while语句> -> <while关键字>(布尔表达式)<复合语句>");

	// 1.在进入布尔表达式之前,预先记录中间代码的id,index
	int inIndex = IntermediaCodeGenerator::currentId;

	// 2&&3.进行布尔表达式翻译 | 记录从布尔表达式获取的中间代码ID - outIndex
	lexical.getSym();

	// 尝试读取 '('
	if (lexical.outSym() != LPAREN) { 
		//
		// 报错
		//
		CERR("error (");
	}

	// 翻译布尔表达式
	int outIndex = BoolExpr();

	// 尝试读取')'
	if (lexical.outSym() != RPAREN) {
		//
		// 报错
		//
		CERR("error )");
	}

	// 4. 进行<复合语句>翻译
	lexical.getSym();
	if (lexical.outSym() != LBRACE) {
		//
		// 报错
		//
	}
	CompoundStatment(); // <复合语句>翻译

	// 5. 向中间代码添加一条无偿跳转至inIndex
	QUADMEMBER op1;
	op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER op2;
	op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER opResult;
	opResult.type = QUADMEMBER::_ADDRESS;
	opResult.value.address = inIndex;
	
	QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_JMP, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

	// 6. 向当前中间代码ID填写进outIndex中,然后退出
	inter_code_generator.intermediaCodeList[outIndex].qStatment.opResult.value.address = IntermediaCodeGenerator::currentId;

}

/*****************************************************
			各个语句的具体实现
******************************************************/

// AssignStatment:<赋值语句> -> <变量名> = <算数表达式>;|<变量名> @ <字符串>;
void Parser::AssignStatment() {
	_COUT("AssignStatment:<赋值语句> -> <变量名> = <算数表达式>;|<变量名> @ <字符串>;");
	
	// 1）申请变量
	string varName = lexical.outVarName();
	AlgExprReturnValue res;		// 算数表达式返回的结果

	// 2）尝试读取 '='
	lexical.getSym();
	if (lexical.outSym() == ASSIGN) {
		// 3）读取运算结果（会预读取一位 ;)
		res = AlgExrp();

		// 4）开始生成四元式中的op1（算数的运算结果）
		QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(res);

		// 5）生成四元式中的op2(空,0)
		QUADMEMBER op2;
		op2.type = QUADMEMBER::_EMPTY;


		// 6）开始申请四元式中的opResult
		QUADMEMBER opResult;
		opResult.type = QUADMEMBER::_VAR; // 被赋值语句一定为变量,这点毋庸置疑..
		semantic.GetTheExistentVarObjectFromVarTable(varName, opResult.value.varPointer);
		
		// 7）生成中间元素代码并添加进中间代码队列中
		QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_ASSIGN, op1, op2, opResult);
		inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

		// 
		if (lexical.outSym() == SEMI) {
			return;
		}
		else {

			//
			// 报错.
			//
		}

	}
}

// CompoundStatment:<复合语句> -> {<语句序列>}
void Parser::CompoundStatment() {
	_COUT("CompoundStatment:<复合语句> -> {<语句序列>}");

	// 只要执行这函数，则当前 sym 一定为 {，不用预先读取。
		// 进行常规验证即可
	if (lexical.outSym() == LBRACE) {
		StatmentList();
		/*注意:
			1. StatmentListTail()会预先读取一个sym来进行验证
			2. 因此直接常规验证即可。
		*/
		if (lexical.outSym() == RBRACE) {
			// 当复合语句结束时，将 IfWithoutElseButton 自动关闭。
			// 因为这是关于上一层，当if嵌套时，下一层的这个特定会影响上一层的。
			// 这显然是不合理的。
			IfWithoutElseButton = 0;
			return;
		}
		else {
			//
			// 报错
			//
		}
	}
	else {
		//
		// 报错
		// 
	}
}

// InStatement:<in语句> -> <in关键字>  >> <变量名>
void Parser::InStatement() {
	_COUT("InStatement:<in语句> -> <in关键字>  >> <变量名>");

	if (lexical.outSym() != KW_CIN) {
		//
		// 程序报错
		//
	}
	lexical.getSym(); /* 获取 '>>' */
	if (lexical.outSym() != IN) {
		//
		// 程序报错
		//
	}
	// 获取 <变量名> 
	lexical.getSym();

	// 生成对应的中间代码 (IN,__,__,<变量名>)
	QUADMEMBER op1;
	op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER op2;
	op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER opResult;

	// 输入必须
	if (lexical.outSym() == ID) {
		opResult.type = QUADMEMBER::_VAR;
		if (semantic.GetTheExistentVarObjectFromVarTable(lexical.outVarName(), opResult.value.varPointer) == false) {
			// 
			// 在该变量表中不存在该变量,出错..
			//
		}
	}
	else {
		//
		// 不是合法的变量名,这里报错
		//
	}

	QUADSTATMENT qStatment = inter_code_generator.GenerateQuadStatment(OP_IN, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatment);

	// 读取 ';'，语句结束
	lexical.getSym();
	if (lexical.outSym() != SEMI) {
		//
		// 程序出差
		//
	}

	return;

}

// OutStatement:<out语句> -> <out关键字>  << [<变量名>|<数字>];
void Parser::OutStatement(){
	_COUT("OutStatement:<out语句> -> <out关键字>  << [<变量名>|<数字>];");

	if (lexical.outSym() != KW_COUT) {
		//
		// 程序报错
		//
	}
	lexical.getSym(); /* 获取 '>>' */
	if (lexical.outSym() != OUT) {
		//
		// 程序报错
		//
	}
	// 获取 <变量名> 
	lexical.getSym();

	// 生成对应的中间代码 (OUT,__,__,<变量名>)
	QUADMEMBER op1;
	op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER op2;
	op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER opResult;

	// 输入必须
	if (lexical.outSym() == ID) {
		opResult.type = QUADMEMBER::_VAR;
		if (semantic.GetTheExistentVarObjectFromVarTable(lexical.outVarName(), opResult.value.varPointer) == false) {
			// 
			// 在该变量表中不存在该变量,出错..
			//
		}
	}
	else {
		//
		// 不是合法的变量名,这里报错
		//
	}

	QUADSTATMENT qStatment = inter_code_generator.GenerateQuadStatment(OP_OUT, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatment);

	// 读取 ';'，语句结束
	lexical.getSym();
	if (lexical.outSym() != SEMI) {
		//
		// 程序出差
		//
	}

	return;
}


// BoolExpr:<布尔表达式> -> <算数表达式> [>|>=|<|<=|==|!=] <算数表达式>
int Parser::BoolExpr() {
	_COUT("<布尔表达式> -> <算数表达式> [>|>=|<|<=|==|!=] <算数表达式>");
	
	/*
		注意：虽然<布尔表达式>不会预先读取一个字符，但是其调用的<算数表达式>却会预先读取一个符号
			因此<布尔表达式>会预先读取一个符号，这你要明确。
	*/

	OperatorCode opCode;

	// 1）尝试获取左算数表达式C
	AlgExprReturnValue lAlgReturnValue = Parser::AlgExrp();

	// 2）读取 比较运算符,用于之后的翻译
	/*注意：AlgExrp可能会预先读取，故不用存储*/
	if (lexical.outSym() == G || lexical.outSym()  == GE || lexical.outSym()  == B || lexical.outSym() == BE || lexical.outSym() ==EQ || lexical.outSym() ==NE) {
		switch (lexical.outSym()) {
		case G: // 大于 -> 小于等于 OP_JLE
			opCode = OP_JLE;
			break;
		case GE: // 大于等于 -> 小于 OP_JL
			opCode = OP_JL;
			break;
		case B: // 小于 -> 大于等于 OP_JGE
			opCode = OP_JGE;
			break;
		case BE: // 小于等于 -> 大于 OP_JG
			opCode = OP_JG;
			break;
		case EQ: // 等于 -> 不等于 OP_JNE
			opCode = OP_JNE;
			break;
		case NE: // 不等于 -> 等于
			opCode = OP_JE;
			break;
		}
	}
	else {
		
		//
		// 错误处理
		//
	}

	// 3）获取右子式
	AlgExprReturnValue rAlgReturnValue = Parser::AlgExrp();


	QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(lAlgReturnValue);
	QUADMEMBER op2 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(rAlgReturnValue);
	QUADMEMBER opResult;
	opResult.type = QUADMEMBER::_EMPTY;


	QUADSTATMENT qStatment = inter_code_generator.GenerateQuadStatment(OP_CMP, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatment);

	// 4）构造中间代码 (jmp_,_,address)，返回四元式在列表中的指针，在if语句中进行构造
	QUADMEMBER _op1;
	_op1.type = QUADMEMBER::_EMPTY;
	QUADMEMBER _op2;
	_op2.type = QUADMEMBER::_EMPTY;
	QUADMEMBER _op3;
	_op3.type = QUADMEMBER::_ADDRESS; // 表示地址

	QUADSTATMENT _qStatment = inter_code_generator.GenerateQuadStatment(opCode, _op1, _op2, _op3);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(_qStatment);

	// 5）返回该数值，被调用函数直接获取对应索引来进行处理
	return inter_code_generator.intermediaCodeList.size() - 1;

}


/*****************************************************
			算数表达式的整体生成
******************************************************/
// AlgExpr<算数表达式> -> <项>[+| -] <算数表达式尾> | <项> 空
AlgExprReturnValue Parser::AlgExrp() {
	_COUT("AlgExpr<算数表达式> -> <项>[+| -] <算数表达式尾> | <项> 空");

	// 1 获取<因子>
	AlgExprReturnValue lAlgReturnValue = Item();

	//
	// Warnning: Item() will read the next sym in advance.
	//			so we don't have to provoke getSym() in this function.
	//

	// 2 尝试获取 '+/-'
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

// Item:<项> -> <因子> * <项尾> | <因子>
AlgExprReturnValue Parser::Item() {
	_COUT("Item:<项> -> <因子> * <项尾> | <因子>");
	
	// 1）首先尝试获取因子
	AlgExprReturnValue lAlgReturnValue = Factor();

	// 2）尝试获取'*'
	lexical.getSym();

	// 3）如果是 *，则说明下面依然存在式子
	if (lexical.outSym() == TIMES) {
		return ItemTail(lAlgReturnValue);
	}
	else {
		return lAlgReturnValue;
	}

	
}

// Factor:<因子>-><数字> | <变量名>
AlgExprReturnValue Parser::Factor() {
	_COUT("Factor:<因子>-><数字> | <变量名>");

	lexical.getSym();

	// 构造变量返回值
	AlgExprReturnValue alg;
	
	if (lexical.outSym() == NUM) {
		alg.type = AlgExprReturnValue::_NUM;
		alg.value.num = lexical.outNum();
	}
	else if (lexical.outSym() == ID) {
		alg.type = AlgExprReturnValue::_VAR;
		// 尝试调用语义分析器从变量表中获取对应变量.
		semantic.GetTheExistentVarObjectFromVarTable(lexical.outVarName(), alg.value.pVarObject);
	}
	else {
		
		//
		// 错误处理
		//
	}

	// 返回好已构造好的表达式
	return alg;
}

// ItemTail:<项尾> -> <因子> * | <因子> 空
AlgExprReturnValue Parser::ItemTail(AlgExprReturnValue lAlgReturnValue) {
	_COUT("ItemTail:<项尾> -> <因子> * | <因子> 空");

	// 1) 先读取因子.
	AlgExprReturnValue rAlgReturnValue = Parser::Factor();

	// 2) 生成中间变量.
	string tmpVarName = semantic.GenerateTmpVarObject();
	AlgExprReturnValue resultAlgReturnValue;
	resultAlgReturnValue.type = AlgExprReturnValue::_TMP_VAR;
	resultAlgReturnValue.value.pVarObject = semantic.outVarObjectPointer(tmpVarName);

	// 3）将三个值转化为四元式.
	QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(lAlgReturnValue);
	QUADMEMBER op2 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(rAlgReturnValue);
	QUADMEMBER opResult = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(resultAlgReturnValue);

	// 4）生成中间代码并插入队列
	QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(OP_TIMES, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

	// 5）尝试读取 '*'
	lexical.getSym();

	// 6）如果是 '*'，则后面依然需要有计算的，先计算出当前的中间变量，
	//	  再把当前的语句转换为中间代码，最后将中间变量作为参数传递给下一个。
	//	  否则，为空。 后面的分析中注意被提前读取一个字符，在赋值表达式或布尔式中注意。
	if (lexical.outSym() == TIMES) {
		return ItemTail(resultAlgReturnValue);
	}
	else {
		return resultAlgReturnValue;
	}
}

//AlgExprTail<算数表达式尾> -> [+|-] <算数表达式尾>|空
AlgExprReturnValue Parser::AlgExprTail(AlgExprReturnValue lAlgReturnValue, bool isAdd) {
	_COUT("AlgExprTail<算数表达式尾> -> [+|-] <算数表达式尾>|空");

	// 1）肯定存在因子,进来先读取因子
	AlgExprReturnValue rAlgReturnValue = Item();

	// 2）生成中间变量
	string tmpVarName = semantic.GenerateTmpVarObject();
	AlgExprReturnValue resultAlgReturnValue;
	resultAlgReturnValue.type = AlgExprReturnValue::_TMP_VAR;
	resultAlgReturnValue.value.pVarObject = semantic.outVarObjectPointer(tmpVarName);

	// 3）将三个值转换成四元式
	QUADMEMBER op1 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(lAlgReturnValue);
	QUADMEMBER op2 = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(rAlgReturnValue);
	QUADMEMBER opResult = inter_code_generator.TransferQuadMemberFromAlgExprReturnValue(resultAlgReturnValue);

	// 4）生成中间代码并插入队列中
	enum OperatorCode opCode = isAdd == true ? OP_ADD : OP_MINUS;
	QUADSTATMENT qStatement = inter_code_generator.GenerateQuadStatment(opCode, op1, op2, opResult);
	inter_code_generator.InsertQuadStatmentIntoIntermediaCodeList(qStatement);

	// 注意: Parser::Item()会预读取，所以这里不用读取
	//LexicalAnalyzer::getSym();
	
	// 5）分析sym
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
