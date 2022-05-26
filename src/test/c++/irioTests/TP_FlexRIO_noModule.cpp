#include <gtest/gtest.h>

#include <cstdlib>

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

using std::cout; using std::endl;
using std::string; using std::cerr;

/**
 * Test verifies driver’s ability to check the correct communication and interconnection
 * between auxiliar I/O analog/digital FPGA registers.
 * This test is related to the following requirements:
 *
 * PXIE7965R
 * PXIE7966R
 *
 * This test is executed only with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

TEST(TP_FlexRIO_noModule, functional) {
	string testName = "TP_FlexRIO_noModule: Functional test of bitfile noModule";
	string testDescription = "Test verifies driver’s ability to check the "
			"correct communication and interconnection between auxiliary I/O analog/digital FPGA registers";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/*
	 * TEST 0
	 * IRIO DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
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

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 1
	 * FPGA START
	 */
	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value " << 1 << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	/*
	 * TEST 2
	 * AUXILIARY 32 BITS ANALOG I/O PORTS
	 */
	cout << endl << "TEST 2: Testing auxiliary 32 bits analog I/O ports" << endl << endl;
	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
	// but only six auxiliary analog output ports instantiated
	for(int i=0;i<6;i++){
		int analogValue = 0;
		cout << "[irio_setAuxAO function] Value " << analogValue << " is set in auxAO" << i << endl;
		myStatus = irio_setAuxAO(&p_DrvPvt,i,analogValue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAO function] Value read from auxAO" << i << "is: " << aivalue << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAI function] Value read from auxAI" << i << "is: " << aivalue << endl << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		if(aivalue!=analogValue){
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 0, read value: %d [ERROR]\n",i,aivalue);
			TestUtilsIRIO::getErrors(status);
		}

		analogValue = 100;
		cout << "[irio_setAuxAO function] Value " << analogValue << " is set in auxAO" << i << endl;
		myStatus = irio_setAuxAO(&p_DrvPvt,i,analogValue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAO function] Value read from auxAO" << i << "is: " << aivalue << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAI function] Value read from auxAI" << i << "is: " << aivalue << endl << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		if(aivalue!=analogValue){
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 1,read value: %d [ERROR]\n",i,aivalue);
			TestUtilsIRIO::getErrors(status);
		}
	}

	/*
	 * TEST 3
	 * AUXILIARY 64 BITS ANALOG I/O PORTS
	 */
	cout << endl << "TEST 3: Testing auxiliary 64 bits analog I/O ports" << endl << endl;
	int64_t ai64value = 0;
	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
	// but only six auxiliary analog output ports instantiated
	for(int i=0;i<6;i++){
		int analog64Value = 0;
		cout << "[irio_setAuxAO_64 function] value " << analog64Value << " is set in aux64AO" << i << endl;
		myStatus = irio_setAuxAO_64(&p_DrvPvt,i,analog64Value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAO_64(&p_DrvPvt,i,&ai64value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAO_64 function] value read from aux64AO" << i << "is: " << ai64value << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAI_64(&p_DrvPvt,i,&ai64value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAI_64 function] value read from aux64AI" << i << "is: " << ai64value << endl << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		if(ai64value!=analog64Value){
			irio_mergeStatus(&status,Generic_Error,verbosity,"Aux64AI%d expected value 0, read value: %d [ERROR]\n",i,ai64value);
			TestUtilsIRIO::getErrors(status);
		}

		analog64Value = 100;
		cout << "[irio_setAuxAO_64 function] value " << analog64Value << " is set in aux64AO" << i << endl;
		myStatus = irio_setAuxAO_64(&p_DrvPvt,i,analog64Value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAO_64(&p_DrvPvt,i,&ai64value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAO_64 function] value read from aux64AO" << i << "is: " << ai64value << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxAI_64(&p_DrvPvt,i,&ai64value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAI_64 function] value read from aux64AI" << i << "is: " << ai64value << endl << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		if(aivalue!=analog64Value){
			irio_mergeStatus(&status,Generic_Error,verbosity,"Aux64AI%d expected value 1,read value: %d [ERROR]\n",i,ai64value);
			TestUtilsIRIO::getErrors(status);
		}
	}

	/*
	 * TEST 4
	 * AUXILIARY DIGITAL I/O PORTS
	 */
	cout << endl << "TEST 4: Testing auxiliary digital I/O ports" << endl << endl;
	aivalue = 0;
	// It is known prior to the execution of the test that there are only six auxiliary digital
	//input and six auxiliary digital output ports instantiated
	for(int i=0;i<6;i++) {
		int digitalvalue = 0;
		cout << "[irio_setAuxDO function] Value " << digitalvalue << " is set in auxDO" << i << endl;
		myStatus = irio_setAuxDO(&p_DrvPvt,i,digitalvalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDO function] Value read from auxDO" << i << "is: " << aivalue << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDI function] Value read from auxDI" << i << "is: " << aivalue << endl << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		if(aivalue!=digitalvalue){
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 0, value read:%d [ERROR]\n",i,aivalue);
			TestUtilsIRIO::getErrors(status);
		}

		digitalvalue = 1;

		cout << "[irio_setAuxDO function] Value " << digitalvalue << " is set in auxDO" << i << endl;
		myStatus = irio_setAuxDO(&p_DrvPvt,i,digitalvalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDO function] Value read from auxDO" << i << "is: " << aivalue << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDI function] Value read from auxDI" << i << "is: " << aivalue << endl << endl;

		EXPECT_EQ(myStatus, IRIO_success);

		if(aivalue!=digitalvalue){
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 1, value read:%d [ERROR]\n",i,aivalue);
			TestUtilsIRIO::getErrors(status);
		}
	}

	/*
	 * TEST 5
	 * FPGA TEMPERATURE
	 */
	cout << endl << "TEST 5: Reading FPGA temperature" << endl << endl;
	int FPGATemp = 0;
	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 6
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 6: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}
