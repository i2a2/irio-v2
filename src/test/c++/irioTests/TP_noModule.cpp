#include <gtest/gtest.h>

#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <boost/version.hpp>
#include <boost/regex.hpp>

// IRIO Library
extern "C" {
#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>
}

#include "testUtils-IRIO.h"

// Always max verbosity
static int verbosity = 1;

/**
 * Test verifies driver’s ability to check the correct communication and interconnection
 * between auxiliar I/O analog/digital FPGA registers.
 * This test is related to the following requirements:
 *
 * PXIE7965R
 * PXIE7966R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

TEST(TP_noModule, functional) {
	std::string testName = "TP_noModule: Functional test of bitfile noModule";
	std::string testDescription = "Test verifies driver’s ability to check the "
			"correct communication and interconnection between auxiliary I/O analog/digital FPGA registers";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int FPGATemp=0;
	int aivalue=0;
	int i=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("functionalNoModuleTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	// In TP_onlyResources test all parameters of irio_initDriver has been tested, so
	// in this test they are suppose that are not going to be incorrect.
	// Critical failure and closing driver if something fail

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing auxiliary analog I/O ports" << std::endl << std::endl;
	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
	// but only six auxiliary analog output ports instantiated
	for(i=0;i<6;i++){
		int analogValue = 0;
		std::cout << "[irio_setAuxAO function] value " << analogValue << " is set in auxAO" << i << std::endl;
		myStatus=irio_setAuxAO(&p_DrvPvt,i,analogValue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}

		EXPECT_NE(status.detailCode, IRIO_error);

		myStatus=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAO function] value read from auxAO" << i << "is: " << aivalue << std::endl;

		EXPECT_NE(status.detailCode, IRIO_error);

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAI function] value read from auxAI" << i << "is: " << aivalue << std::endl << std::endl;

		EXPECT_NE(status.detailCode, IRIO_error);

		if(aivalue!=analogValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 0, read value: %d [ERROR]\n",i,aivalue);

		analogValue = 100;
		std::cout << "[irio_setAuxAO function] value " << analogValue << " is set in auxAO" << i << std::endl;
		myStatus=irio_setAuxAO(&p_DrvPvt,i,analogValue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}

		EXPECT_NE(status.detailCode, IRIO_error);

		myStatus=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAO function] value read from auxAO" << i << "is: " << aivalue << std::endl;

		EXPECT_NE(status.detailCode, IRIO_error);

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAI function] value read from auxAI" << i << "is: " << aivalue << std::endl << std::endl;

		EXPECT_NE(status.detailCode, IRIO_error);

		if(aivalue!=analogValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 1,read value: %d [ERROR]\n",i,aivalue);
	}

	std::cout << "Testing auxiliary digital I/O ports" << std::endl << std::endl;

	// It is known prior to the execution of the test that there are only six auxiliary digital
	//input and six auxiliary digital output ports instantiated
	for(i=0;i<6;i++) {
		int digitalvalue = 0;

		std::cout << "[irio_setAuxDO function] value " << digitalvalue << " is set in auxDO" << i << std::endl;
		myStatus=irio_setAuxDO(&p_DrvPvt,i,digitalvalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}

		myStatus=irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxDO function] value read from auxDO" << i << "is: " << aivalue << std::endl;

		myStatus=irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxDI function] value read from auxDI" << i << "is: " << aivalue << std::endl << std::endl;

		if(aivalue!=digitalvalue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 0, value read:%d [ERROR]\n",i,aivalue);

		digitalvalue = 1;

		std::cout << "[irio_setAuxDO function] value " << digitalvalue << " is set in auxDO" << i << std::endl;
		myStatus=irio_setAuxDO(&p_DrvPvt,i,digitalvalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}

		myStatus=irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxDO function] value read from auxDO" << i << "is: " << aivalue << std::endl;

		myStatus=irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxDI function] value read from auxDI" << i << "is: " << aivalue << std::endl << std::endl;

		if(aivalue!=digitalvalue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 1, value read:%d [ERROR]\n",i,aivalue);
	}

	std::cout << "Testing FPGA temperature" << std::endl << std::endl;

	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
//	TODO: Darle formato a esta cadena en C++. Por ahora uso printf por simplicidad
//	std::cout << "[irio_getDevTemp function] temperature read from FPGA: " << FPGATemp*0.25 << std::endl;
	printf("[irio_getDevTemp function] temperature read from FPGA: %.2fºC\n\n",FPGATemp*0.25);

	std::cout << "Testing the close of the driver" << std::endl << std::endl;

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}


// Tests to catch errors

TEST(TP_noModule, setFPGATwice) {
	std::string testName = "TP_noModule: Set FPGA twice test";
	std::string testDescription = "Test verifies driver’s ability to check that the FPGA is started twice";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue = 0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("setFPGATwiceTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "First start of the FPGA, no error expected" << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	std::cout << "Second start of the FPGA, error/warning expected" << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}

TEST(TP_noModule, setFPGAInitErrorSessionClose) {
	std::string testName = "TP_noModule: Start FPGA after a failure on the driver's initialization. Condition: there has to be the driver's session close";
	std::string testDescription = "Test verifies driver’s ability to check bitfile is already downloaded and running on the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("setFPGAInitErrorSessionCloseTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   "wrongBitfile",
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   "wrongFilePath",
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	EXPECT_NE(status.detailCode, IRIO_success);
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;
	if (aivalue == 0)
		std::cout << "IRIO can not initialize the FPGA if there is not a driver's session open" << std::endl;

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	EXPECT_NE(status.detailCode, IRIO_success);
}

TEST(TP_noModule, setFPGAInitErrorSessionOpen) {
	std::string testName = "TP_noModule: Start FPGA after a failure on the driver's initialization. Condition: there has to be the driver's session open";
	std::string testDescription = "Test verifies driver’s ability to check bitfile is already downloaded and running on the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("setFPGAInitErrorSessionOpenTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   "wrongVersion",
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_success);
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;
	if (aivalue == 1)
		std::cout << "IRIO can initialize the FPGA if there is a driver's session open despite the driver initialization failure."
				" User must close the driver to restart the FPGA." << std::endl;

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	EXPECT_NE(status.detailCode, IRIO_success);
}

//TODO: El problema de este test es que depende cuando se ejecute el parámetro a tener en cuenta,
//      p_DrvPvt->fpgaStarted está a 1 o a 0 dependiendo de qué funciones se han ejecutado antes
//      Para hacerlo independiente, sin tener que llamar a init_driver() que es la función que
//      hace p_DrvPvt->fpgaStarted = 0 al principio, habría que ejecutar esa sentencia
//      dentro del test

TEST(TP_noModule, setFPGANoInitDriver) {
	std::string testName = "TP_noModule: Start FPGA before the driver's initialization.";
	std::string testDescription = "Test verifies driver’s ability to check that FPGA can not be initialized if the driver is not initialized";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	EXPECT_NE(status.detailCode, IRIO_success);
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;
	if (aivalue == 1)
		std::cout << "IRIO can initialize the FPGA if there is a driver's session open despite the driver initialization failure."
				"Must close the driver to restart the FPGA." << std::endl;
	EXPECT_NE(status.detailCode, IRIO_success);
}

TEST(TP_noModule, setAuxAO6) {
	std::string testName = "TP_noModule: writing on non implemented port auxiliary analog output 6";
	std::string testDescription = "Test verifies driver’s ability to check the error if user tries to write on "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("writingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing an auxiliary analog output port that it is not implemented in the FPGA" << std::endl << std::endl;

	int analogValue = 0;
	std::cout << "[irio_setAuxAO function] value " << analogValue << " is set in unimplemented auxAO6. Error/warning expected" << std::endl;
	myStatus=irio_setAuxAO(&p_DrvPvt,6,analogValue,&status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}

	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}

TEST(TP_noModule, getAuxAO6) {
	std::string testName = "TP_noModule: Reading from non implemented port auxiliary analog output 6";
	std::string testDescription = "Test verifies driver’s ability to check the error if user tries to read from "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("readingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing an auxiliary analog output port that it is not implemented in the FPGA" << std::endl << std::endl;

	myStatus=irio_getAuxAO(&p_DrvPvt,6,&aivalue,&status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	else
		std::cout << "[irio_getAuxAO function] value read from auxAO6 is: " << aivalue << std::endl;

	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}

TEST(TP_noModule, getAuxAI10) {
	std::string testName = "TP_noModule: Reading from non implemented port auxiliary analog input 10";
	std::string testDescription = "Test verifies driver’s ability to check the error if user tries to read from "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("readingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing an auxiliary analog input port that it is not implemented in the FPGA" << std::endl << std::endl;

	myStatus=irio_getAuxAI(&p_DrvPvt,10,&aivalue,&status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	else
		std::cout << "[irio_getAuxAI function] value read from auxAI10 is: " << aivalue << std::endl;

	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}

TEST(TP_noModule, setAuxDO6) {
	std::string testName = "TP_noModule: Writing on non implemented port auxiliary digital output 6";
	std::string testDescription = "Test verifies driver’s ability to check the error if user tries to write on "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("writingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing an auxiliary analog output port that it is not implemented in the FPGA" << std::endl << std::endl;

	int digitalValue = 0;
	std::cout << "[irio_setAuxDO function] value " << digitalValue << " is set in unimplemented auxDO6. Error/warning expected" << std::endl;
	myStatus=irio_setAuxDO(&p_DrvPvt,6,digitalValue,&status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}

	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}

TEST(TP_noModule, getAuxDO6) {
	std::string testName = "TP_noModule: Reading from non implemented port auxiliary digital output 6";
	std::string testDescription = "Test verifies driver’s ability to check the error if user tries to read from "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("readingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing an auxiliary analog output port that it is not implemented in the FPGA" << std::endl << std::endl;

	myStatus=irio_getAuxDO(&p_DrvPvt,6,&aivalue,&status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	else
		std::cout << "[irio_getAuxDO function] value read from auxDO6 is: " << aivalue << std::endl;

	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}

TEST(TP_noModule, getAuxDI6) {
	std::string testName = "TP_noModule: Reading from non implemented port auxiliary digital input 6";
	std::string testDescription = "Test verifies driver’s ability to check the error if user tries to read from "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Testing driver initialization" << std::endl << std::endl;
	myStatus = irio_initDriver("readingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
		std::cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << std::endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(status.detailCode, IRIO_success);

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	ASSERT_NE(status.detailCode, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	usleep(100);

	std::cout << "Testing an auxiliary digital input port that it is not implemented in the FPGA" << std::endl << std::endl;

	myStatus=irio_getAuxDI(&p_DrvPvt,6,&aivalue,&status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	else
		std::cout << "[irio_getAuxDI function] value read from auxDI6 is: " << aivalue << std::endl;

	EXPECT_NE(status.detailCode, IRIO_success);

	std::cout << "Closing driver..." << std::endl;
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
}


