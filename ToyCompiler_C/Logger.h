#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
using namespace std;

// ��־��¼����
class Logger {

public:
	Logger(string fileName, bool isUseOutStream = false, ostream& _outStream = std::cout);	
	bool log(string logTxt);


private:

	//
	// log ����Ҫ�����һ���ļ���
	//		��ѡһ�������,Ĭ���� std::cout
	// 
	bool isUseOutStream; 
	ostream& outStream; // ��������������
	ofstream outFile;
};