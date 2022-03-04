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

// Environment variables
static std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
static std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

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

	// User doesn't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

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
	// Critical failure and aborting execution if something fail
	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);

		//TODO: Permitido en GoogleTest?
		std::cout << "CRITICAL FALIURE Aborting execution" << std::endl;
		exit(-1);
	}

	std::cout << std::endl << "Testing FPGA start mode" << std::endl << std::endl;
	std::cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << std::endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning, not error
	EXPECT_NE(status.detailCode, IRIO_error);

	// TODO: Comprobar que no saca mensajes redundantes
	if (myStatus == IRIO_warning) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);
	}
	else if(myStatus == IRIO_error) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
		free(detailStr);

		//TODO: Permitido en GoogleTest?
		std::cout << "CRITICAL FALIURE Aborting execution" << std::endl;
		exit(-1);
	}
	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	std::cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << std::endl << std::endl;

	//TODO: 100us sleep  usleep(100); only if necessary

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

		myStatus=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAO function] value read from auxAO" << i << "is: " << aivalue << std::endl;

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAI function] value read from auxAI" << i << "is: " << aivalue << std::endl << std::endl;

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

		myStatus=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAO function] value read from auxAO" << i << "is: " << aivalue << std::endl;

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			char* detailStr = nullptr;
			irio_getErrorString(status.detailCode, &detailStr);
			std::cerr << "Runtime error/warning detail code: " << status.detailCode << ", " << detailStr << std::endl ;
			free(detailStr);
		}
		else
			std::cout << "[irio_getAuxAI function] value read from auxAI" << i << "is: " << aivalue << std::endl << std::endl;

		if(aivalue!=analogValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 1,read value: %d [ERROR]\n",i,aivalue);
	}

	std::cout << "Testing auxiliary digital I/O ports" << std::endl << std::endl;

	// It is known prior to the execution of the test that there are only six auxiliary digital
	//input and six auxiliary digital output ports instantiated
	for(i=0;i<6;i++) {
		int digitalValue = 0;

		std::cout << "[irio_setAuxDO function] value " << digitalValue << " is set in auxDO" << i << std::endl;
		myStatus=irio_setAuxDO(&p_DrvPvt,i,digitalValue,&status);
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

		if(aivalue!=digitalValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 0, value read:%d [ERROR]\n",i,aivalue);

		digitalValue = 1;

		std::cout << "[irio_setAuxDO function] value " << digitalValue << " is set in auxDO" << i << std::endl;
		myStatus=irio_setAuxDO(&p_DrvPvt,i,digitalValue,&status);
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

		if(aivalue!=digitalValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 1, value read:%d [ERROR]\n",i,aivalue);
	}

	std::cout << "Testing FPGA temperature" << std::endl << std::endl;

	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
//	TODO: Darle formato a esta cadena en C++. Por ahora uso printf por simplicidad
//	std::cout << "[irio_getDevTemp function] temperature read from FPGA: " << FPGATemp*0.25 << std::endl;
	printf("[irio_getDevTemp function] temperature read from FPGA: %.2fºC\n\n",FPGATemp*0.25);

	std::cout << "Testing the close of the driver" << std::endl << std::endl;

	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	std::cout << "Closing driver..." << std::endl;
}


















