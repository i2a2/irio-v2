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
#include <irioHandlerDMA.h>
#include <limits.h>
}

#include "testUtils-IRIO.h"

// Always max verbosity
static int verbosity = 1;

using std::cout; using std::endl;
using std::string; using std::cerr;

/**
 * Test verifies the use of digital lines in the NI6581.
 * This test is related to the following requirements:
 *
 * PXIe-7961R, PXIe-7965R or PXIe-7966R
 *
 * NI6581 adapter module
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7961, 7965, 7966
 *
 */

TEST(TP_FlexRIO_mod6581, functional) {
	string testName = "TP_FlexRIO_mod6581: Functional test of bitfile FlexRIOMod6581";
	string testDescription = "Test verifies the data digital acquisition profile in "
			                 "the FlexRIO device using the NI6581 digital I/O adapter module";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalMod6581Test";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string bitfileName = "FlexRIOMod6581_"+RIODevice;
	string FPGAversion = "V1.1"; // User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
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
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int32_t valueReadI32 = 0;
	irio_getFPGAStart(&p_DrvPvt,&valueReadI32,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << valueReadI32 << " (0-stopped, 1-running)" << endl;

	/*
	 * TEST 2
	 * SETTING AND GETTING FROM AUX DIGITAL OUTPUT
	 */
	cout << endl << "TEST 2: Testing auxiliary digital output port 6. "
				    "It allows to write on whole digital output port 2" << endl << endl;

	cout << "[irio_setAuxDO function] auxDO6 set to 1" << endl;
	myStatus = irio_setAuxDO(&p_DrvPvt,6,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueReadI32 = -1;
	myStatus = irio_getAuxDO(&p_DrvPvt,6,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAuxDO function] AuxDO6 value read: " << valueReadI32 << endl << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	cout << "auxDO6 configured to 1, means that DO[7-0] are connected to port 2 in Connector A" << endl;

	/*
	 * TEST 3
	 * WRITING AND READING DIGITAL I/O, BOTH VALUES: 0 AND 1.
	 */
	cout << endl << "Test 3: This test writes 0 and 1 into every digital output "
			        "and reads from DO register." << endl << endl;
	cout << "Hardware digital I/O [7-0] are interconnected physically, "
			"then the value written in DO must be read in the DI too" << endl;
	cout << "Although all values written and read are showed, in case "
			"of reading an unexpected value, Error message will be shown" << endl << endl;

	for (int i = 0; i < 8; i++) {
		for (int digValue=0;digValue<2;digValue++) { // Digital ports only take binary values: 0 and 1
			cout << "[irio_setDO function] Write " << digValue << " value in DO" << i << endl;
			myStatus = irio_setDO(&p_DrvPvt,i,digValue,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);

			valueReadI32 = -1;
			myStatus=irio_getDO(&p_DrvPvt,i,&valueReadI32,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getDO function] DO" << i << " value read: " << valueReadI32 << endl;

			if (valueReadI32 != digValue){
				irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in D0%d is %d, but value read is:%d [ERROR]\n",i,digValue,valueReadI32);
				TestUtilsIRIO::getErrors(status);
			}

			valueReadI32 = -1;
			myStatus = irio_getDI(&p_DrvPvt,i,&valueReadI32,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getDI function] DI" << i << " value read: " << valueReadI32 << endl << endl;

			if (valueReadI32 != digValue){
				irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in DI%d is %d, but value read is:%d [ERROR]\n",i,digValue,valueReadI32);
				TestUtilsIRIO::getErrors(status);
			}
		}
	}

	/**
	 * TEST 4
	 * IRIO DRIVER CLOSING
	 */
	cout << "TEST 4: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}
