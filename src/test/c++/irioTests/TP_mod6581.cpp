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
 * PXIE7965R
 * PXIE7966R
 *
 * NI6581 adapter module
 *
 * This test is executed only with a PXIE7965R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965,7966
 */

TEST(TP_mod6581, functional) {
	string testName = "TP_mod6581: Functional test of bitfile FlexRIOMod6581";
	string testDescription = "Test verifies the data acquisition profile in the FlexRIO device";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod6581_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("functionalMod6581Test",
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
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initiated correctly. Closing driver..." << endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 1
	 * FPGA START
	 */
	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	/*
	 * TEST 2
	 * SETTING AND GETTING FROM AUX DIGITAL OUTPUT
	 */
	cout << endl << "TEST 2: Testing auxiliary digital output port 6. "
				              "It allows to write on whole digital output port 2" << endl << endl;

	int32_t valueReadI32 = 0;
	cout << "[irio_setAuxDO function] write '1' value into auxDO6" << endl;
	myStatus=irio_setAuxDO(&p_DrvPvt,6,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getAuxDO(&p_DrvPvt,6,&valueReadI32,&status);
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
	usleep(10);
	cout << endl << "IRIO test 3: This test write 0 and 1, into every digital output, "
			        "and it is read from DO register." << endl << endl;
	cout << "Hardware digital I/O [7-0] are interconnected physically, "
			     "then the value written in DO must be read in the DI too" << endl;
	cout << "Although all values written and read are showed, in case "
			     "of reading an unexpected value, Error message will be shown" << endl << endl;

	for (int i = 0; i < 8; i++) {
		cout << "[irio_setDO function] Write 0 value in DO" << i << endl;
		myStatus=irio_setDO(&p_DrvPvt,i,0,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		myStatus=irio_getDO(&p_DrvPvt,i,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getDO function] DO" << i << " value read: " << valueReadI32 << endl;

		if (valueReadI32 != 0)
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in D0%d is 0, but value read is:%d [ERROR]\n",i,valueReadI32);

		EXPECT_EQ(myStatus, IRIO_success);

		usleep(20);
		myStatus=irio_getDI(&p_DrvPvt,i,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getDI function] DI" << i << " value read: " << valueReadI32 << endl << endl;

		if (valueReadI32 != 0)
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in DI%d is 0, but value read is:%d [ERROR]\n",i,valueReadI32);

		EXPECT_EQ(myStatus, IRIO_success);

		cout << "[irio_setDO function] Write 1 value in DO" << i << endl;
		myStatus=irio_setDO(&p_DrvPvt,i,1,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		myStatus=irio_getDO(&p_DrvPvt,i,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getDO function] DO" << i << " value read: " << valueReadI32 << endl;

		if (valueReadI32 != 1)
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in D0%d is 1, but value read is:%d [ERROR]\n",i,valueReadI32);

		EXPECT_EQ(myStatus, IRIO_success);

		usleep(20);
		myStatus=irio_getDI(&p_DrvPvt,i,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getDI function] DI" << i << " value read: " << valueReadI32 << endl << endl;

		if (valueReadI32 != 1)
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in DI%d is 1, but value read is:%d [ERROR]\n",i,valueReadI32);

		EXPECT_EQ(myStatus, IRIO_success);
	}

	/*
	 * TEST 4
	 * READING FPGA TEMPERATURE
	 */
	int FPGATemp = -1;

	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << endl << "TEST 4: Reading FPGA temperature" << endl << endl;
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 5
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 5: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Tests to check errors

TEST(TP_mod6581, failInitDriver) {
	string testName = "TP_mod6581: Configuring FPGA when there is a failure on driver initialization";
	string testDescription = "Test verifies several warnings and errors if "
                                  "user tries to configure the FPGA when there is a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "wrongPath";
	string bitfileName = "FlexRIOMod6581_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization, expected failure" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod6581",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongPath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	// Expect warnings or errors so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Testing auxiliary digital output port 6. "
				    "It allows to write on whole digital output port 2" << endl << endl;

	int32_t valueReadI32;
	cout << "[irio_setAuxDO function] write '1' value into auxDO6" << endl;
	myStatus=irio_setAuxDO(&p_DrvPvt,6,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAuxDO function] Trying to read AuxDO6 value..." << endl << endl;
	myStatus=irio_getAuxDO(&p_DrvPvt,6,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(10);
	cout << endl << "TEST 3: This test write 0 and 1, into a random digital output, "
			  	    "and it is read from DO register. Warnings or errors expected" << endl;

	cout << "[irio_setDO function] Write 0 value in implemented port DO5" << endl;
	myStatus=irio_setDO(&p_DrvPvt,5,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDO function] Trying to read DO5" << endl;
	myStatus=irio_getDO(&p_DrvPvt,5,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(20);
	cout << "[irio_getDI function] Trying to read DI5" << endl << endl;
	myStatus=irio_getDI(&p_DrvPvt,5,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "Trying to read FPGA temperature..." << endl;
	int FPGATemp = -1;

	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}






