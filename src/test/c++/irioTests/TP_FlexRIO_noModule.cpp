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
 * PXIe-7961R, PXIe-7965R, PXIe-7966R, PXIe-7975R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7961, 7965, 7966, 7975
 */

TEST(TP_FlexRIO_noModule, functional) {
	string testName = "TP_FlexRIO_noModule: Functional test of bitfile noModule";
	string testDescription = "Test verifies driver’s ability to check the "
			"correct communication and interconnection between auxiliary I/O analog/digital FPGA registers";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalNoModuleTest";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;
	string FPGAversion = "V1.2"; // User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * IRIO DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver(appCallID.c_str(),
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
	cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value 1" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		if (myStatus == IRIO_error)
			irio_closeDriver(&p_DrvPvt,0,&status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue = 0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << aivalue << " (0-stopped, 1-running)" << endl;

	/**
	 * TEST 2
	 * AUXILIARY 32 BITS ANALOG I/O PORTS
	 */
	cout << endl << "TEST 2: Testing auxiliary 32 bits analog I/O ports" << endl << endl;
	std::vector<int32_t> auxValues = {0,100};
	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
	// but only six auxiliary analog output ports instantiated
	for(int i=0;i<6;i++){
		for (std::vector<int32_t>::iterator it = auxValues.begin(); it != auxValues.end(); ++it) {
			cout << "[irio_setAuxAO function] Value " << *it << " is set in auxAO" << i << endl;
			myStatus = irio_setAuxAO(&p_DrvPvt,i,*it,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);

			aivalue = -1;
			myStatus = irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxAO function] Value read from auxAO" << i << "is: " << aivalue << endl;

			aivalue = -1;
			myStatus = irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxAI function] Value read from auxAI" << i << "is: " << aivalue << endl << endl;

			if(aivalue!=*it){
				irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value %d, read value: %d [ERROR]\n",i,*it,aivalue);
				TestUtilsIRIO::getErrors(status);
			}
		}
	}

	/**
	 * TEST 3
	 * AUXILIARY 64 BITS ANALOG I/O PORTS
	 */
	cout << "TEST 3: Testing auxiliary 64 bits analog I/O ports" << endl << endl;
	std::vector<int64_t> aux64Values = {0,100};
	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
	// but only six auxiliary analog output ports instantiated
	for(int i=0;i<6;i++){
		for (std::vector<int64_t>::iterator it = aux64Values.begin(); it != aux64Values.end(); ++it) {
			int64_t ai64value = 0;
			cout << "[irio_setAuxAO_64 function] value " << *it << " is set in aux64AO" << i << endl;
			myStatus = irio_setAuxAO_64(&p_DrvPvt,i,*it,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);

			ai64value = -1;
			myStatus = irio_getAuxAO_64(&p_DrvPvt,i,&ai64value,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxAO_64 function] value read from aux64AO" << i << "is: " << ai64value << endl;

			ai64value = -1;
			myStatus = irio_getAuxAI_64(&p_DrvPvt,i,&ai64value,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxAI_64 function] value read from aux64AI" << i << "is: " << ai64value << endl << endl;

			if(ai64value!=*it){
				irio_mergeStatus(&status,Generic_Error,verbosity,"Aux64AI%d expected value %d, read value: %d [ERROR]\n",i,*it,ai64value);
				TestUtilsIRIO::getErrors(status);
			}
		}
	}

	/**
	 * TEST 4
	 * AUXILIARY DIGITAL I/O PORTS
	 */
	cout << "TEST 4: Testing auxiliary digital I/O ports" << endl << endl;
	// It is known prior to the execution of the test that there are only six auxiliary digital
	//input and six auxiliary digital output ports instantiated
	for (int i=0;i<6;i++){
		for (int auxDig=0;auxDig<2;auxDig++) { // Digital ports only take binary values: 0 and 1
			cout << "[irio_setAuxDO function] Value " << auxDig << " is set in auxDO" << i << endl;
			myStatus = irio_setAuxDO(&p_DrvPvt,i,auxDig,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);

			aivalue = -1;
			myStatus = irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxDO function] Value read from auxDO" << i << "is: " << aivalue << endl;

			aivalue = -1;
			myStatus = irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxDI function] Value read from auxDI" << i << "is: " << aivalue << endl << endl;

			if(aivalue!=auxDig){
				irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value %d, read value: %d [ERROR]\n",i,auxDig,aivalue);
				TestUtilsIRIO::getErrors(status);
			}
		}
	}

	/**
	 * TEST 5
	 * FPGA TEMPERATURE
	 */
	cout << "TEST 5: Reading FPGA temperature" << endl << endl;
	int FPGATemp = 0;
	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl;
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
