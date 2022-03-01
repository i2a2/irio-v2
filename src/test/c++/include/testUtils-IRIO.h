#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <fstream>
#include <stdio.h>
#include <gtest/gtest.h>
#include <type_traits>
#include <functional>
#include <algorithm>

#define FWHT   "\x1b[37m"	//Foreground color, White
#define BBLA   "\x1b[40m"	//Background color, Black
#define RST    "\x1B[0m"	//Foreground color, Reset

class TestUtilsIRIO
{
	static void displayTitle(const std::string& msg, const std::string& forecolor=FWHT, const std::string& backcolor=BBLA) {
		std::cout<<forecolor<<backcolor<<"+-----------------------------------------------------------------------------"<<std::endl;
		std::cout<<msg<<std::endl;
		std::cout<<"+-----------------------------------------------------------------------------"<<std::endl<<RST<<std::endl;
	}

	static std::string getEnvVar(const std::string& shellVarName) {
		std::string shellVarValue;
		try {
			shellVarValue=std::string(std::getenv(shellVarName.c_str()));
		}
		catch(...) {
			throw std::logic_error("Shell variables not defined. Please define "+shellVarName+" variable before run this test.");
		}

		return shellVarValue;
	}
};

#endif /* TESTUTILS_H_ */
