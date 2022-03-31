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
 * Test verifies the performance of the FlexRIO device
 * This test is related to the following requirements:
 *
 * PXIE7966R. 7966R device can not be sustituted by any other model
 *
 * NI5761 adapter module
 *
 * This test must be executed with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=7966
 * execute export Coupling=X, X=0 (Coupling mode supported by ITER), 1 (Not supported by ITER)
 */

TEST(TP_FlexRIO_mod5761_perf, functional) {
	string testName = "TP_FlexRIO_FlexRIO_mod5761_perf: Functional test of bitfile FlexRIO_perf";
	string testDescription = "Test verifies the performance of the FlexRIO device connected to NI5761 module";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");
	string Coupling  = TestUtilsIRIO::getEnvVar("Coupling");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIO_perf_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/*
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("functionalPerformanceTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	// In TP_FlexRIO_onlyResources test all parameters of irio_initDriver has been tested, so
	// in all these tests it is suppose that they are not going to be incorrect.
	// Critical failure and closing driver if something fail

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	// TODO: Poner todos los get tras set

	/*
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
	 * DEBUG MODE CONFIGURATION: ON MODE
	 */
	usleep(100);
	cout << endl << "TEST 2: Testing debug mode configuration, ON mode" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 1 (ON)" << endl;

	myStatus=irio_setDebugMode(&p_DrvPvt,1,&status); // Debug mode set to ON, value 1
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	// Order of tests inverted in compare to C example.
	// I'm following the previous C++ GoogleTest TP_FlexRIO_mod5761 tests' order

	/*
	 * TEST 3
	 * DMA CLEANING
	 */
	usleep(100);
	cout << endl << "TEST 3: Testing DMAs' cleaning." << endl << endl;
	cout << "[irio_cleanDMAsTtoHost function] No output if DMAs have been cleaned successfully" << endl;
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 4
	 * DMA CONFIGURATION
	 */
	usleep(100);
	cout << endl << "TEST 8: Testing DMAs' set up configuration" << endl;
	cout << "[irio_setUpDMAsTtoHost function] No output if DMAs have been configured successfully" << endl;
	// All DMAs are configured. In this case there is only one DMA implemented in the FPGA
	// with four channels (every channel has a digital value of 16 bits)
	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 5
	 * DMA SAMPLING RATE CONFIGURATION
	 */
	uint32_t samplingRate = 500000; // 500k samples/seg
	int32_t Fref = 0;               // Should be uint32_t but in irioCore struct is defined as int32_t
	int32_t valueReadI32 = 0;
	usleep(100);
	// Equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - SamplingRate is the sampling rate desired to be configured
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
	cout << endl << "TEST 5: Testing DMAs' sampling rate configuration." << endl << endl;
	myStatus = irio_getFref(&p_DrvPvt, &Fref, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_NE(Fref, 0);
	if (Fref != 0) {
		cout << "FPGA Clock reference (Fref value) is: " << Fref << " Hz" << endl;
		cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to: "
				  << samplingRate << " Samples/s"<< endl;

		int32_t decimationFactor = Fref/samplingRate;
		myStatus=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,decimationFactor,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		myStatus=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA0 read: "
				  << Fref/valueReadI32 << " Samples/s" << endl;
		EXPECT_EQ(myStatus, IRIO_success);
	}

	/*
	 * TEST 6
	 * DMA ENABLE
	 */
	usleep(100);
	cout << endl << "TEST 6: Testing DMAs' Enable" << endl << endl;
	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 7
	 * SIGNALTYPE FOR WG 0 CONFIGURATION: SINE TYPE
	 */
	usleep(100);
	cout << endl << "TEST 7: Set Signal Generator Type" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 1 (SINE)" << endl;
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,1,&status); // Sine signal configured, value 1
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 8
	 * SG Update rate
	 */
	usleep(100);
	uint32_t SGFref = 0;
	samplingRate = 10000000;

	cout << endl << "TEST 8: Set Signal Generator update rate" << endl << endl;
	myStatus=irio_getSGFref(&p_DrvPvt, &SGFref, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "FPGA SignalGenerator Fref has the value: " << SGFref << endl;
	// SGUpdateRate=(SGFref/(Samples/s)). In this case SG0 will generate 10 MS/s
	// At this version, user has to apply this calculus
	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to " << SGFref/samplingRate <<
			     " MSamples/s " << endl;
	myStatus=irio_setSGUpdateRate(&p_DrvPvt, 0, SGFref/samplingRate, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 9
	 * SG Frequency
	 */
	usleep(100);
	cout << endl << "TEST 9: Set Signal Generator frequency" << endl << endl;
	// We want program signal generator with 10kHz periodic signal
	// Equation to apply to obtain freq_desired is:
	//       SGFreq = Freq_desired*((2to32)/(SGFref/(Samples/s)))
	uint32_t freqDesired = 10000;
	int accIncr = freqDesired*(UINT_MAX/samplingRate);
	cout << "[irio_setSGFreq function] SGFreq0 set to " << accIncr << ", meaning " << freqDesired << " Hz" << endl;
	myStatus=irio_setSGFreq(&p_DrvPvt, 0, accIncr, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 10
	 * SG Amplitude
	 */
	usleep(100);
	cout << endl << "TEST 10: Set Signal Generator amplitude" << endl << endl;
	int32_t amplitude = 4096;
	double CVDAC = 0.0;
	myStatus = irio_getSGCVDAC(&p_DrvPvt, &CVDAC, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_setSGAmp function] SGAmp0 set to " << amplitude << ", meaning " << amplitude/CVDAC << " V" << endl;
	myStatus=irio_setSGAmp(&p_DrvPvt, 0, amplitude, &status); // y(t)=4096sin(2*pi*10000*t) signal configured
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 11
	 * AO0_ENABLE
	 */
	usleep(100);
	cout << endl << "TEST 11: Configuring the Analog output enable on port 0" << endl << endl;
	cout << "[irio_setAOEnable] AOEnable0 set to 1 (ENABLE)" << endl;
	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO0 enable
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 12
	 * PERFORMANCE TEST 1
	 */
	usleep(100);
	cout << endl << "TEST 12: Performance test 1" << endl << endl;
	cout << "This test pretends to show that the IRIO library is able to "
			"overcome the throughput of 750MB/s" << endl;
	cout << "IRIO Performance Test 1. Bandwidth tested: 800kB/s during 5 seconds" << endl;
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);















}
