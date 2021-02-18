#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
using namespace std;

// 日志记录工具
class Logger {

public:
	Logger(string fileName, bool isUseOutStream = false, ostream& _outStream = std::cout);	
	bool log(string logTxt);


private:

	//
	// log 必须要输出到一个文件中
	//		可选一个输出流,默认是 std::cout
	// 
	bool isUseOutStream; 
	ostream& outStream; // 依赖其他输入流
	ofstream outFile;
};