#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <gtest/gtest.h>

#define FWHT   "\x1b[37m"	//Foreground color, White
#define BBLA   "\x1b[40m"	//Background color, Black
#define RST    "\x1B[0m"	//Foreground color, Reset
#define FCYN   "\x1b[36m"	//Foreground color, Cyan

using std::cout; using std::endl;
using std::string; using std::cerr;

class TestUtilsIRIO
{
public:
	static void displayTitle(const string& msg, const string& forecolor=FWHT, const string& backcolor=BBLA) {
		cout<<forecolor<<backcolor<<"+-----------------------------------------------------------------------------"<<endl;
		cout<<msg<<endl;
		cout<<"+-----------------------------------------------------------------------------"<<endl<<RST<<endl;
	}

	static string getEnvVar(const string& shellVarName) {
		string shellVarValue;
		try {
			shellVarValue=string(std::getenv(shellVarName.c_str()));
		}
		catch(...) {
			throw std::logic_error("Shell variables not defined. Please define "+shellVarName+" variable before run this test.");
		}

		return shellVarValue;
	}

	static void getErrors(const TStatus& status) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << endl << endl;
		free(detailStr); detailStr = nullptr;
	}
};

#endif /* TESTUTILS_H_ */
