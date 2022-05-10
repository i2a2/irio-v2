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
 */

TEST(TP_cRIO_PBP, functional) {
	string testName = "TP_cRIO_PBP: Functional test of bitfile cRIO_PBP";
	string testDescription = "Test verifies driver’s ability to check cRIO PBP profile. "
	                 		 "Tis test is focused on the resources implemented in the FPGA "
	                 		 "to improve IRIO code coverage. Several parameters are going to be "
	                 		 "instantiated with no functionality because cRIO modules are not connected "
	                 		 "in NI9159 chassis.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "9159";
	string RIOSerial = "0x01C10FAC";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.0";
	string NIRIOmodel = "NI 9159";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "cRIO_PBP";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	myStatus = irio_initDriver("functionalcRIOPBPTest",
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

	/*
	 * TEST 1
	 * IRIO VERSION
	 */
	cout << endl << "TEST 1: Testing IRIO version" << endl << endl;
	char* version = new char[7];
	myStatus = irio_getVersion(version,&status);
	cout << "[irio_getVersion function] IRIO version: " << version << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus,IRIO_success);
	delete [] version;

/**
 * READ DEVICE INFORMATION FROM FPGA
 */

	/**
	 * TEST 2
	 * FPGA START
	 */
	cout << endl << "TEST 2: Testing FPGA start mode" << endl << endl;
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
	 * TEST 3
	 * FPGA VI VERSION
	 */
	cout << endl << "TEST 3: Testing FPGA VI Version" << endl << endl;
	char* VIVersion = new char[strlen(FPGAversion.c_str())];
	unsigned long int valueLength;
	myStatus = irio_getFPGAVIVersion(&p_DrvPvt,VIVersion,4,&valueLength,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getFPGAVIVersion function] FPGA VI version: " << VIVersion << endl;

	/*
	 * TEST 4
	 * DEVICE QUALITY STATUS
	 */
	cout << endl << "TEST 4: Testing device quality status" << endl << endl;
	myStatus = irio_getDevQualityStatus(&p_DrvPvt,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevQualityStatus function] Device Quality Status expected "
			"is 0. Value read: " << aivalue << endl;

	/*
	 * TEST 5
	 * FPGA TEMPERATURE
	 */
	cout << endl << "TEST 5: Testing FPGA temperature" << endl << endl;
	int FPGATemp=0;
	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl;
	EXPECT_NE(myStatus, IRIO_error);

	/*
	 * TEST 6
	 * DEVICE PROFILE
	 */
	cout << endl << "TEST 6: Testing device profile" << endl << endl;
	myStatus = irio_getDevProfile(&p_DrvPvt,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevProfile function] Device Profile expected is 1 "
			"(Point by Point Profile). Value read: " << aivalue << endl;

	/*
	 * TEST 7
	 * Point by Point profile sampling rate configuration
	 */
	cout << endl << "TEST 7: Testing PBP sampling rate configuration" << endl << endl;
	int32_t Fref = 0;
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_EQ(myStatus, IRIO_success);
	if (Fref != 0) {
		cout << "FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
	}
	aivalue = 500; // 500 Samples/s
	cout << "[irio_setSamplingRate function] Sampling rate wrote: " << aivalue << endl;
	myStatus = irio_setSamplingRate(&p_DrvPvt,0,aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getSamplingRate(&p_DrvPvt,0,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSamplingRate function] Sampling rate read: " << aivalue << endl;

	/*
	 * TEST 8
	 * ANALOG I/O PORTS
	 */
	cout << endl << "TEST 8: Testing Analog Inputs" << endl << endl;
	cout << "Configuring the Analog output port 0 with a digital value.This function is always "
			"used with cRIO modules connected, but in this test we use it to improve code coverage. " << endl;
	cout << "Analog I/O ports are not connected so they are independent. Value 100 is going to be write "
			"on analog output AO0 and other different value is going to be read on analog input AI0" << endl;
	cout << "[irio_setAO function] AO0 set to 2048 (digital value)" << endl;
	aivalue = 2048;
	int channel = 0;
	myStatus = irio_setAO(&p_DrvPvt,channel,aivalue,&status); // Set AO channel 0 terminal to 2048 digital value
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getAO(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAO function] AO0 read: " << aivalue << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getAI(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAI function] AI0 read: " << aivalue << endl;

	/*
	 * TEST 9
	 * SIGNAL GENERATOR PHASE
	 */
	cout << endl << "TEST 9: Testing Signal Generator phase" << endl << endl;
	aivalue = 90;
	cout << "[irio_setSGPhase function] Signal Generator phase wrote: " << aivalue << endl;
	myStatus = irio_setSGPhase(&p_DrvPvt,channel,aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getSGPhase(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGPhase function] Signal Generator phase read: " << aivalue << endl;

	/**
	 * TEST 10
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 10: Closing IRIO DRIVER" << endl << endl;
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

	string RIODevice = "9159";
	string RIOSerial = "0x01C10FAC";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "NI 9159";

	// Original bitfile. Check that all cRIO modules are connected
	string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	string bitfileName = "cRIOIO_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	myStatus = irio_initDriver("functionalcRIOIOTest",
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

	/*
	 * TEST 1
	 * IRIO VERSION
	 */
	cout << endl << "TEST 1: Testing IRIO version" << endl << endl;
	char* version = new char[7];
	myStatus = irio_getVersion(version,&status);
	cout << "[irio_getVersion function] IRIO version: " << version << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus,IRIO_success);
	delete [] version;

/**
 * READ DEVICE INFORMATION FROM FPGA
 */

	/**
	 * TEST 2
	 * FPGA START
	 */
	cout << endl << "TEST 2: Testing FPGA start mode" << endl << endl;
	cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value " << 1 << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	usleep(100);

	/*
	 * TEST 3
	 * FPGA VI VERSION
	 */
	cout << endl << "TEST 3: Testing FPGA VI Version" << endl << endl;
	char* VIVersion = new char[strlen(FPGAversion.c_str())];
	unsigned long int valueLength;
	myStatus = irio_getFPGAVIVersion(&p_DrvPvt,VIVersion,4,&valueLength,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getFPGAVIVersion function] FPGA VI version: " << VIVersion << endl;

	/*
	 * TEST 4
	 * DEVICE QUALITY STATUS
	 */
	cout << endl << "TEST 4: Testing device quality status" << endl << endl;
	myStatus = irio_getDevQualityStatus(&p_DrvPvt,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevQualityStatus function] Device Quality Status expected "
			"is 0. Value read: " << aivalue << endl;

	/*
	 * TEST 5
	 * FPGA TEMPERATURE
	 */
	usleep(100);
	cout << endl << "TEST 5: Testing FPGA temperature" << endl << endl;
	int FPGATemp=0;
	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 6
	 * DEVICE PROFILE
	 */
	cout << endl << "TEST 6: Testing device profile" << endl << endl;
	myStatus = irio_getDevProfile(&p_DrvPvt,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevProfile function] Device Profile expected is 1 "
			"(Point by Point Profile). Value read: " << aivalue << endl;

	/*
	 * TEST 7
	 * Point by Point profile sampling rate configuration
	 */
	cout << endl << "TEST 7: Testing PBP sampling rate configuration" << endl << endl;
	int32_t Fref = 0;
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_EQ(myStatus, IRIO_success);
	if (Fref != 0) {
		cout << "FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
	}
	aivalue = 500; // 500 Samples/s
	cout << "[irio_setSamplingRate function] Sampling rate wrote: " << aivalue << endl;
	myStatus = irio_setSamplingRate(&p_DrvPvt,0,aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getSamplingRate(&p_DrvPvt,0,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSamplingRate function] Sampling rate read: " << aivalue << endl;

	/*
	 * TEST 8
	 * ANALOG I/O PORTS
	 */
	cout << endl << "TEST 8: Testing Analog Inputs" << endl << endl;
	cout << "Analog I/O ports are not connected so they are independent. Value 100 is going to be write "
			"on analog output AO0 and other different value is going to be read on analog input AI0" << endl;
	cout << "[irio_setAO function] AO0 set to 2048 (digital value)" << endl;
	aivalue = 2048;
	int channel = 0;
	myStatus = irio_setAO(&p_DrvPvt,channel,aivalue,&status); // Set AO channel 0 terminal to 2048 digital value
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getAO(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAO function] AO0 read: " << aivalue << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getAI(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAI function] AI0 read: " << aivalue << endl;

	/**
	 * TEST 9
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 9: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

