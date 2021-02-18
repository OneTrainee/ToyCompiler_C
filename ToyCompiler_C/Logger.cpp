#include "Logger.h"
#include <string>
#include <memory>

Logger::Logger(string fileName, bool _isUseOutStream,ostream& _outStream):
	outStream(_outStream),isUseOutStream(_isUseOutStream){
	
	outFile.open(fileName);

}



bool Logger::log(string context) {

	if (!outFile) {
		return false;
	}

	outFile << context << endl;

	// 若使用输出流,则重定位到输出流中..
	if (isUseOutStream) {
		outStream << context << endl;
	}

	return true;

}
