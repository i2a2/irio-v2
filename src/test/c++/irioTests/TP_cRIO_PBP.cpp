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
using std::string;

/*
 * Test verifies driver’s ability to test cRIO PBP (Point by Point) Profile.
 *
 * This test is related to the following requirements:
 *
 * NI9159 Chassis
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 9159
 */


TEST(TP_cRIO_PBP, noModules) {
	string testName = "TP_cRIO_PBP: Functional test of bitfile cRIO_PBP";
	string testDescription = "Test verifies driver’s ability to check cRIO PBP profile. "
	                 		 "This test is focused on the resources implemented in the FPGA "
	                 		 "to improve IRIO code coverage. Several parameters are going to be "
	                 		 "instantiated with no functionality because cRIO modules are not connected "
	                 		 "in NI9159 chassis.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalcRIOPBPTest";
	string NIRIOmodel = "NI 9159";
	string bitfileName = "cRIO_PBP";
	string FPGAversion = "V1.0"; // User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
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

	// Storage getter methods values
	int valueGetter = -1;

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
	irio_getFPGAStart(&p_DrvPvt,&valueGetter,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << valueGetter << " (0-stopped, 1-running)" << endl << endl;

	cout << "##############################################################" << endl <<
			"The purpose of tests 2, 3, 4, 5 6 and 7 consist of improve code "
			"coverage using ports that are not implemented on other bitfiles." << endl <<
			"##############################################################" << endl;

/**
 * READ DEVICE INFORMATION FROM FPGA
 */

	/**
	 * TEST 2
	 * IRIO VERSION
	 */
	cout << endl << "TEST 2: Testing IRIO version" << endl << endl;
	char* version = new char[7];
	myStatus = irio_getVersion(version,&status);
	cout << "[irio_getVersion function] IRIO version: " << version << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus,IRIO_success);
	delete [] version;

	/**
	 * TEST 3
	 * FPGA VI VERSION
	 */
	cout << endl << "TEST 3: Testing FPGA VI Version" << endl << endl;
	char* VIVersion = new char[FPGAversion.size()+1];
	unsigned long int valueLength;
	myStatus = irio_getFPGAVIVersion(&p_DrvPvt,VIVersion,4,&valueLength,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getFPGAVIVersion function] FPGA VI version: " << VIVersion << endl;

	delete [] VIVersion;

	/**
	 * TEST 4
	 * DEVICE QUALITY STATUS
	 */
	valueGetter = -1;
	cout << endl << "TEST 4: Testing device quality status" << endl << endl;
	myStatus = irio_getDevQualityStatus(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevQualityStatus function] Device Quality Status expected "
			"is 0. Value read: " << valueGetter << endl;

	/**
	 * TEST 5
	 * FPGA TEMPERATURE
	 */
	cout << endl << "TEST 5: Testing FPGA temperature" << endl << endl;
	int FPGATemp=0;
	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl;
	EXPECT_NE(myStatus, IRIO_error);

	/**
	 * TEST 6
	 * DEVICE PROFILE
	 */
	valueGetter = -1;
	cout << endl << "TEST 6: Testing device profile" << endl << endl;
	myStatus = irio_getDevProfile(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevProfile function] Device Profile configured in the FPGA "
			"expected is 1. Value read: " << valueGetter << " (0: DAQ DMA, 1: Point by Point)" << endl;

	/**
	 * TEST 7
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 7: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_cRIO_PBP, cRIO_IO){
	string testName = "TP_cRIO_PBP: Functional test of bitfile cRIO_IO";
	string testDescription = "Test verifies driver’s ability to check cRIO device "
			                 "and cRIO modules connected to it. It returns IRIO driver "
			                 "error if cRIO modules are missing";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalcRIOIOTest";
	string NIRIOmodel = "NI 9159";
	string bitfileName = "cRIOIO_"+RIODevice;
	string FPGAversion = "V1.1"; // User don't have to know what FPGA Version is used
	// Original bitfile. Check that all cRIO modules are connected to avoid errors
	string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
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

	// Storage getter methods values
	int valueGetter = -1;

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
	irio_getFPGAStart(&p_DrvPvt,&valueGetter,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << valueGetter << " (0-stopped, 1-running)" << endl;

	/**
	 * TEST 2
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}
