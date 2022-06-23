
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

// Test for ITER hardware: all cRIO modules available
TEST(TP_cRIO_PBP, functional){
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
	 * STARTING DATA ACQUISITION
	 */
	cout << endl << "TEST 3: Set DAQ Start" << endl << endl;
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (0-OFF, 1-ON)" << endl << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDAQStartStop(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDAQStartStop function] DAQStartStop read " << valueGetter << endl;

	/**
	 * TEST 4
	 * DEVICE PROFILE
	 */
	cout << endl << "TEST 4: Testing device profile" << endl << endl;
	valueGetter = -1;
	myStatus = irio_getDevProfile(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevProfile function] Device Profile configured in the FPGA "
			"expected is 1. Value read: " << valueGetter << " (0: DAQ DMA, 1: Point by Point)" << endl;

	/**
	 * TEST 5
	 * I/O SAMPLING RATE
	 */
	int32_t Fref = 0;

	cout << endl << "TEST 5: Testing I/O sampling rate" << endl << endl;
	// Equation applied to set SamplingRate: Fref/samplingRate=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - SamplingRate is the sampling rate desired to be configured
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_NE(Fref, 0);
	if (Fref != 0) {
		int32_t samplingRate = Fref; // 1000 Samples/seg, maximum sampling rate
		cout << "[irio_getFref function] FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
		cout << "[irio_setSamplingRate function] Sampling rate set to "
			 << Fref/samplingRate << ", meaning " << samplingRate << " Samples/s"<< endl;

		myStatus = irio_setSamplingRate(&p_DrvPvt,0,Fref/samplingRate,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getSamplingRate(&p_DrvPvt,0,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getSamplingRate function] Sampling rate read "
			 << valueGetter << ", meaning " << Fref*valueGetter << " Samples/s" << endl;
	}

	/**
	 * TEST 6
	 * DEBUG MODE CONFIGURATION AND ANALOG I/O PORTS
	 */
	cout << endl << "TEST 6: Testing debug mode configuration and analog I/O ports" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 0 (0-OFF, 1-ON)" << endl;
	myStatus = irio_setDebugMode(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDebugMode(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDebugMode function] DebugMode read: " << valueGetter << endl;

	cout << endl << "AO0, AO1 & AO2 are configured with 0.00V. "
					"Values read from AI0, AI1 and AO2 must be close to 0V. "
					"Expected absolute value is less than 0.002V" << endl << endl;

	double CVDAC = 0.0;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGCVDAC function] CVDAC (conversion from Volts for AO): "
		 << std::fixed << CVDAC << endl;

	cout << "[irio_setAO function] AO0, AO1 & AO2 are configured with 0.00V" << endl;

	for (int AOchannel = 0; AOchannel < 3; AOchannel++) {
		myStatus = irio_setAO(&p_DrvPvt,AOchannel,(int)(0.0*CVDAC),&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
	}

	for (int AOchannel = 0; AOchannel < 3; AOchannel++) {
		valueGetter = -1;
		myStatus = irio_getAO(&p_DrvPvt,AOchannel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAO function] Value read from AO" << AOchannel << " is :"
			 << std::setprecision(3) << (float) (valueGetter/CVDAC) << "V" << endl << endl;
	}

	double CVADC = 0.0;
	myStatus = irio_getSGCVADC(&p_DrvPvt,&CVADC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGCVADC function] CVADC (conversion from AO to Volts): "
		 << std::fixed << CVADC << endl;

	for (int AIchannel = 0; AIchannel < 3; AIchannel++) {
		valueGetter = -1;
		myStatus = irio_getAI(&p_DrvPvt,AIchannel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAI function] Slot 2 9205. Value read from AI" << AIchannel << " is :"
			 << std::setprecision(3) << (float) (valueGetter*CVADC) << "V" << endl << endl;
	}

	cout << endl << "Setting DebugMode to ON, values expected from AIs are: "
					"AI0 = 1.25, AI1 = 3.125 & AI2 = -4.75" << endl << endl;

	cout << endl << "[irio_setDebugMode function] DebugMode set to 1 (0-OFF, 1-ON)" << endl;
	myStatus = irio_setDebugMode(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDebugMode(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDebugMode function] DebugMode read: " << valueGetter << endl;

	EXPECT_EQ(myStatus, IRIO_success);

	for (int AIchannel = 0; AIchannel < 3; AIchannel++) {
		valueGetter = -1;
		myStatus = irio_getAI(&p_DrvPvt,AIchannel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAI function] Slot 2 9205. Value read from AI" << AIchannel << " is :"
			 << std::setprecision(4) << (float) (valueGetter*CVADC) << "V" << endl;
	}

	/**
	 * TEST 7
	 * DEBUG MODE CONFIGURATION AND ANALOG ENABLE I/O PORTS
	 */
	cout << endl << "TEST 7: Testing debug mode configuration and analog enable I/O ports" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 0 (0-OFF, 1-ON)" << endl;
	myStatus = irio_setDebugMode(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDebugMode(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDebugMode function] DebugMode read: " << valueGetter << endl;

	std::vector<int32_t> digitalValues = {4000,8000,-8000};
	int channel = 0;

	for (std::vector<int>::iterator it = digitalValues.begin(); it != digitalValues.end(); ++it) {
		cout << "[irio_setAOEnable function] AOEnable" << channel << " set to 1 (0-DISABLE, 1-ENABLE)" << endl;
		myStatus = irio_setAOEnable(&p_DrvPvt,channel,1,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getAOEnable(&p_DrvPvt,channel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getAOEnable function] AOEnable" << channel << " read: " << valueGetter << endl;
		EXPECT_EQ(myStatus, IRIO_success);

		cout << "[irio_setAO function] AO" << channel << " is configured with: " << *it
				<< " (digital value), this means " << (float)(*it/CVDAC) << "V" << endl;
		myStatus = irio_setAO(&p_DrvPvt,channel,*it,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getAO(&p_DrvPvt,channel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAO function] Value read from AO" << channel << " is : " << valueGetter
			 << " (digital value), this means " << std::setprecision(4)
			 << (float) (valueGetter/CVDAC) << "V" << endl;

		valueGetter = -1;
		myStatus = irio_getAI(&p_DrvPvt,channel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAI function] Value read from AI" << channel << " is : " << valueGetter
			 << " (digital value), this means " << std::setprecision(4)
			 << (float) (valueGetter*CVADC) << "V" << endl << endl;
		channel++;
	}

	cout << endl << "Disabling Analog Outputs" << endl << endl;
	for (int i = 0; i < (int) digitalValues.size(); i++){
		cout << "[irio_setAOEnable function] AOEnable" << i << " set to 0 (0-DISABLE, 1-ENABLE)" << endl;
		myStatus = irio_setAOEnable(&p_DrvPvt,i,0,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getAOEnable(&p_DrvPvt,i,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getAOEnable function] AOEnable" << i << " read: " << valueGetter << endl << endl;
		EXPECT_EQ(myStatus, IRIO_success);
	}

	/**
	 * TEST 8
	 * DIGITAL I/O PORTS
	 */
	cout << "TEST 8: Testing digital I/O ports" << endl << endl;
	cout << "NI9401 module" << endl;
	for (int i = 0; i < 2; i++){
		cout << "[irio_setDO function] NI9401 DO0 Value is set: " << i << endl;
		myStatus = irio_setDO(&p_DrvPvt,0,i,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getDO(&p_DrvPvt,0,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDO function] Value read from NI9401 DO0 is: " << valueGetter << endl;

		valueGetter = -1;
		myStatus = irio_getDI(&p_DrvPvt,0,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDI function] Value read from NI9401 DI0 is: " << valueGetter << endl << endl;
	}

	cout << endl << "NI9477, NI9426 modules" << endl;
	for (int i = 0; i < 2; i++){
		cout << "[irio_setDO function] NI9477 DO1 Value is set: " << i << endl;
		myStatus = irio_setDO(&p_DrvPvt,1,i,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getDO(&p_DrvPvt,1,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDO function] Value read from NI9477 DO1 is: " << valueGetter << endl;

		valueGetter = -1;
		myStatus = irio_getDI(&p_DrvPvt,1,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDI function] Value read from NI9426 DI1 is: " << valueGetter << endl << endl;
	}

	cout << endl << "NI9476, NI9425 modules" << endl;
	for (int i = 0; i < 2; i++){
		cout << "[irio_setDO function] NI9476 DO2 Value is set: " << i << endl;
		myStatus = irio_setDO(&p_DrvPvt,2,i,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getDO(&p_DrvPvt,2,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDO function] Value read from NI9476 DO2 is: " << valueGetter << endl;

		valueGetter = -1;
		myStatus = irio_getDI(&p_DrvPvt,2,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDI function] Value read from NI9425 DI2 is: " << valueGetter << endl << endl;
	}

	/**
	 * TEST 9
	 * AUXILIARY ANALOG AND DIGITAL I/O PORTS
	 */
	cout << "TEST 9: Testing auxiliary analog and digital I/O ports" << endl << endl;
	std::vector<int32_t> auxValues = {5000,250};
	channel = 0;

	for (std::vector<int>::iterator it = auxValues.begin(); it != auxValues.end(); ++it) {
		cout << "[irio_setAuxAO function] AuxAO" << channel << " is set to value: " << *it << endl;
		myStatus = irio_setAuxAO(&p_DrvPvt,channel,*it,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getAuxAO(&p_DrvPvt,channel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAuxAO function] Value read from AuxAO" << channel
			 << " is: " << valueGetter << endl;

		valueGetter = -1;
		myStatus = irio_getAuxAI(&p_DrvPvt,channel,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getAuxAI function] Value read from AuxAI" << channel
			 << " is: " << valueGetter << endl << endl;
		channel++;
	}

	//Writes 0 and 1 into auxDO0 and reads value in auxDO0 and auxDI0
	//Writes 0 and 1 into auxDO1 and reads value in auxDO1 and auxDI1
	for (int i = 0; i < 2; i++){
		for (int w = 0; w < 2; w++) { // Digital ports only take binary values: 0 and 1
			cout << "[irio_setAuxDO function] AuxDO" << i << " Value is set: " << w << endl;
			myStatus = irio_setAuxDO(&p_DrvPvt,i,w,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);

			valueGetter = -1;
			myStatus = irio_getAuxDO(&p_DrvPvt,i,&valueGetter,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxDO function] Value read from AuxDO" << i << " is: " << valueGetter << endl;

			valueGetter = -1;
			myStatus = irio_getAuxDI(&p_DrvPvt,i,&valueGetter,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			cout << "[irio_getAuxDI function] Value read from AuxDI" << i << " is: " << valueGetter << endl << endl;
		}
	}

	/**
	 * TEST 10
	 * STOP DATA ACQUISITION
	 */
	cout << endl << "TEST 10: Stop DAQ Start" << endl << endl;
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDAQStartStop(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDAQStartStop function] DAQStartStop read: " << valueGetter << endl;

	/**
	 * TEST 11
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 11: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Tests for UPM hardware: all cRIO modules not available
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
