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
#include <thread>

// Always max verbosity
static int verbosity = 1;

using std::cout; using std::endl;
using std::string; using std::cerr;

struct threadData_t{
	irioDrv_t *p_Drv;
	int flagToFinish;
	long int blocksRead;
	uint64_t *DBuffer;
};

void DMAthread(threadData_t* data, TStatus* status) {
	int myStatus = 0;
	int elementsRead = 0;
	int numBlocks = 10;
	int DMAFrameType = 0;

	do{
		myStatus = irio_getDMATtoHostData(data->p_Drv,numBlocks,DMAFrameType,
				                          data->DBuffer,&elementsRead,status);
		if(myStatus == IRIO_success){
			if (elementsRead == 10){
				data->blocksRead += elementsRead;
			}
			else {
				usleep(10);
			}
		}
		else{
			TestUtilsIRIO::getErrors(*status);
			cout << "DEBUG: Error on irio_getDMATtoHostData " << endl;
		}
	}while(data->flagToFinish == 0);
}

/**
 * Test verifies the performance of the FlexRIO device
 * This test is related to the following requirements:
 *
 * PXIE7966R. 7966R device can not be substituted by any other model
 *
 * NI5761 adapter module
 *
 * This test must be executed with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=7966
 */

TEST(TP_FlexRIO_mod5761_perf, functional) {
	string testName = "TP_FlexRIO_mod5761_perf: Functional test of bitfile FlexRIO_perf";
	string testDescription = "Test verifies the performance of the FlexRIO device connected to NI5761 module";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

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

	int valueReadI32 = 0; // Used in getter functions

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

	myStatus = irio_setDebugMode(&p_DrvPvt,1,&status); // Debug mode set to ON, value 1
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getDebugMode(&p_DrvPvt,&valueReadI32,&status); // DebugMode FPGA register is read
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getDebugMode function] DebugMode read: " << valueReadI32 << endl << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 3
	 * SIGNALTYPE FOR WG 0 CONFIGURATION: SINE TYPE
	 */
	usleep(100);
	cout << endl << "TEST 3: Configuring Signal Generator Type" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 1 (SINE)" << endl;
	myStatus = irio_setSGSignalType(&p_DrvPvt,0,1,&status); // Sine signal configured, value 1
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getSGSignalType function] SGSignalType0 read: " << valueReadI32 << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 4
	 * SG Update rate
	 */
	usleep(100);
	uint32_t SGFref = 0;
	int SGChannel = 0;
	int32_t samplingRate = 10000000; // 10*10⁶ Samples/s

	cout << endl << "TEST 4: Set Signal Generator update rate" << endl << endl;
	myStatus = irio_getSGFref(&p_DrvPvt,SGChannel,&SGFref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "FPGA SignalGenerator Clock Reference, SGfref: " << SGFref << " Hz" << endl;
	// Equation applied to set SGUpdateRate: SGUpdateRate=(SGFref/(Samples/s))
	// Where - SGFref is p_DrvPvt.SGfref, this value is read from FPGA by irioDriver initialization
	//		 - samplesPerSeg is the amount of samples that are going to be generated
	// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.SGfref/10000) in third parameter of irio_setSGUpdateRate
	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to 10 MSamples/s" << endl;
	myStatus = irio_setSGUpdateRate(&p_DrvPvt,0,SGFref/samplingRate,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getSGUpdateRate(&p_DrvPvt,0,&valueReadI32, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getSGUpdateRate function] SGUpdateRate0 read: " << valueReadI32 << " MSamples/s"
			  << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 5
	 * SG Frequency
	 */
	usleep(100);
	cout << endl << "TEST 5: Set Signal Generator frequency" << endl << endl;
	// We want program signal generator with 100Hz periodic signal
	// Equation to apply to obtain freq_desired is:
	//       SGFreq = Freq_desired*((2to32)/(SGFref/(Samples/s)))
	uint32_t freqDesired = 100;
	int accIncr = freqDesired*(UINT_MAX/samplingRate);
	cout << "[irio_setSGFreq function] SGFreq0 set to " << accIncr << ", meaning " <<
			freqDesired << " Hz" << endl;
	myStatus = irio_setSGFreq(&p_DrvPvt,0,accIncr,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	 myStatus = irio_getSGFreq(&p_DrvPvt,0,&valueReadI32,&status);
	 cout << "[irio_getSGFreq function] SGFreq0 read " << valueReadI32 << ", meaning " <<
			      valueReadI32/(UINT_MAX/samplingRate) << " Hz" << endl;
	 if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	/*
	 * TEST 6
	 * SG Amplitude
	 */
	usleep(100);
	cout << endl << "TEST 6: Set Signal Generator amplitude" << endl << endl;
	int32_t amplitude = 4096;
	double CVDAC = 0.0;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_setSGAmp function] SGAmp0 set to " << amplitude << ", meaning " << amplitude/CVDAC << " V" << endl;
	myStatus = irio_setSGAmp(&p_DrvPvt,0,amplitude,&status); // y(t)=4096sin(2*pi*10000*t) signal configured
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	 myStatus = irio_getSGAmp(&p_DrvPvt,0,&valueReadI32,&status);
	 cout << "[irio_getSGAmp function] SGAmp0 read " << valueReadI32 << ", meaning " <<
			      valueReadI32/CVDAC << " V" << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 7
	 * AO0_ENABLE
	 */
	usleep(100);
	cout << endl << "TEST 7: Configuring the Analog output enable on port 0" << endl << endl;
	cout << "[irio_setAOEnable] AOEnable0 set to 1 (ENABLE)" << endl;
	myStatus = irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO0 enable
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getAOEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAOEnable function] AOEnable0 read: " << valueReadI32 << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 8
	 * DMA CLEANING
	 */
	usleep(100);
	cout << endl << "TEST 8: Testing DMAs' cleaning." << endl << endl;
	cout << "[irio_cleanDMAsTtoHost function] No output if DMAs have been cleaned successfully" << endl;
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 9
	 * DMA CONFIGURATION
	 */
	usleep(100);
	cout << endl << "TEST 9: Testing DMAs' set up configuration" << endl << endl;
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
	 * TEST 10
	 * DMA SAMPLING RATE CONFIGURATION
	 */
	samplingRate = 1000; // 500k samples/seg
	int32_t Fref = 0; // Should be uint32_t but in irioCore struct is defined as int32_t
	usleep(100);
	// Equation applied to set DMATtoHostSamplingRate: Fref/samplesPerSeg=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - samplesPerSeg is the amount of samples that are going to be generated
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
	cout << endl << "TEST 10: Testing DMAs' sampling rate configuration." << endl << endl;
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_NE(Fref, 0);
	if (Fref != 0) {
		cout << "FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
		cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to: "
				  << samplingRate << " Samples/s"<< endl;

		// TODO: Revisar esto. No tiene sentido lo que hacen en los tests de C.
		// Ahi indica que Fref son 100MHz cuando son 125
//		int32_t decimationFactor = Fref/samplingRate;
//		myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,decimationFactor,&status);
		myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1000,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
//		cout << "[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA0 read: "
//				  << Fref/valueReadI32 << " Samples/s" << endl;
		cout << "[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA0 read: "
				  << valueReadI32 << " Samples/s" << endl;
		EXPECT_EQ(myStatus, IRIO_success);
	}

	/*
	 * TEST 11
	 * DMA ENABLE
	 */
	usleep(100);
	cout << endl << "TEST 11: Testing DMAs' Enable" << endl << endl;
	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * PERFORMANCE TESTS
	 */
	cout << endl << "***  PERFORMANCE TESTS ***" << endl;
	cout << "This group of tests pretends to show that the IRIO library is not able "
			"to overcome the throughput of 750MB/s" << endl;

	/*
	 * THREAD NEEDED TO RUN PERFORMANCE TESTS
	 */
	usleep(100);
	threadData_t data;
	data.p_Drv = &p_DrvPvt;
	data.blocksRead = 0;
	data.flagToFinish = 0;
	data.DBuffer = new uint64_t[4096*8*10];
	std::thread t (DMAthread,&data,&status);

	/*
	 * TEST 12
	 * PERFORMANCE TEST 1
	 */
	cout << endl << "TEST 12: Performance test 1" << endl << endl;
	cout << "This test pretends to show that IRIO library is able to "
			"overcome the throughput of 800kB/s" << endl;
	cout << "Bandwidth tested: 800kB/s during 5 seconds" << endl;
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << "Thread acquiring data" << endl;
	cout << "Sleep 5 seconds..." << endl;
	sleep(5); // 5 Seconds to wait while the thread acquires data.

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	int DMAsOverflow = 0;
	myStatus = irio_getDMATtoHostOverflow(&p_DrvPvt,&DMAsOverflow,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	if (DMAsOverflow==0)	{
		cout << "800kB/s during 5 seconds, 4MB of data "
				"reception are expected" << endl;
		cout << "Bandwidth tested: 800kB/s [OK]. Data "
				"transfered: " << std::setprecision(3) <<  data.blocksRead*4096.0*8.0/1000000.0
				<< " MB" << endl << endl;
	}
	else{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 1. "
				         "Bandwidth tested: 800kB/s [ERROR]. DMA Overflow with data loss\n");
	}

	/*
	 * TEST 13
	 * PERFORMANCE TEST 2
	 */
	cout << endl << "TEST 13: Performance test 2" << endl << endl;
	cout << "This test pretends to show that IRIO library is able to "
			"overcome the throughput of 8MB/s" << endl;
	cout << "Bandwidth tested: 8MB/s during 10 seconds" << endl;

	// Clean DMAs and reset blocks read
	cout << "[irio_cleanDMAsTtoHost function] Cleaning DMAs" << endl;
	data.blocksRead=0;
	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	usleep(100);
	myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,100,&status); // DMA SamplingRate is configured to 100MHz/100
	usleep(100);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	// Now data acquisition can start
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << "Thread acquiring data" << endl;
	cout << "Sleep 10 seconds..." << endl;
	sleep(10); // 10 Seconds to wait while the thread acquires data.

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getDMATtoHostOverflow(&p_DrvPvt,&DMAsOverflow,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	if (DMAsOverflow==0)	{
		cout << "8MB/s during 10 seconds, 80MB of data "
				"reception are expected" << endl;
		cout << "Bandwidth tested: 80MB/s [OK]. Data "
				"transfered: " << std::setprecision(4) <<  data.blocksRead*4096.0*8.0/1000000.0
				<< " MB" << endl << endl;
	}
	else{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 2. "
				         "Bandwidth tested: 80MB/s [ERROR]. DMA Overflow with data loss\n");
	}

	/*
	 * TEST 14
	 * PERFORMANCE TEST 3
	 */
	cout << endl << "TEST 14: Performance test 3" << endl << endl;
	cout << "This test pretends to show that IRIO library is able to "
			"overcome the throughput of 80MB/s" << endl;
	cout << "Bandwidth tested: 80MB/s during 30 seconds" << endl;

	// Clean DMAs and reset blocks read
	data.blocksRead=0;
	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	usleep(100);
	myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,10,&status); // DMA SamplingRate is configured to 100MHz/100
	usleep(100);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	// Now data acquisition can start
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << "Thread acquiring data" << endl;
	cout << "Sleep 30 seconds..." << endl;
	sleep(30); // 30 Seconds to wait while the thread acquires data.

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getDMATtoHostOverflow(&p_DrvPvt,&DMAsOverflow,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	if (DMAsOverflow==0)	{
		cout << "80MB/s during 30 seconds, 2400MB of data "
				"reception are expected" << endl;
		cout << "Bandwidth tested: 80MB/s [OK]. Data "
				"transfered: " << std::setprecision(6) << data.blocksRead*4096.0*8.0/1000000.0
				<< " MB" << endl << endl;
	}
	else{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 4. "
				"Bandwidth tested: 800MB/s [ERROR]. DMA Overflow with data loss. Througput: "
				"%.2f MB/s \n\n",((8*(data.blocksRead)*4096.0)/60000000.0));
	}

	/*
	 * TEST 15
	 * PERFORMANCE TEST 4
	 */
	cout << endl << "TEST 15: Performance test 4" << endl << endl;
	cout << "This test pretends to show that IRIO library is not able to "
			"overcome the throughput of 800MB/s. Overflow expected" << endl;
	cout << "Bandwidth tested: 800MB/s during 60 seconds" << endl;

	// Clean DMAs and reset blocks read
	data.blocksRead=0;
	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	usleep(100);
	myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1,&status); // DMA SamplingRate is configured to 100MHz/100
	usleep(100);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	// Now data acquisition can start
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << "Thread acquiring data" << endl;
	cout << "Sleep 60 seconds..." << endl;
	sleep(60); // 60 Seconds to wait while the thread acquires data.

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_getDMATtoHostOverflow(&p_DrvPvt,&DMAsOverflow,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	if (DMAsOverflow==0)	{
		cout << "800MB/s during 60 seconds, Throughput expected> 750MB/s" << endl;
		cout << "Bandwidth tested: 800MB/s [OK]. Throughput: "
				<< std::setprecision(5) << data.blocksRead*4096.0*8.0/60000000.0
				<< " MB" << endl << endl;
	}
	else{
		irio_mergeStatus(&status,Generic_Error,verbosity,"Bandwidth tested: 800MB/s [ERROR]. "
				         "DMA Overflow with data loss.");
		cout << "Throughput: " << std::setprecision(5) <<
				data.blocksRead*4096.0*8.0/60000000.0 << " MB/s" << endl << endl;
	}

	data.flagToFinish=1;
	t.join();

	cout << endl << "TEST 16: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Test to check errors

TEST (TP_FlexRIO_mod5761_perf, failInitDriver) {
	string testName = "TP_FlexRIO_mod5761_perf: Test of bitfile FlexRIO_perf "
			          "when there is a failure on driver initialization";
	string testDescription = "Test verifies several warnings and errors if "
            "user tries to configure the FPGA when there is a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

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

	int valueReadI32 = 0; // Used in getter functions

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongPath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	// In TP_FlexRIO_onlyResources test all parameters of irio_initDriver has been tested, so
	// in all these tests it is suppose that they are not going to be incorrect.
	// Critical failure and closing driver if something fail

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 2: Testing debug mode configuration, ON mode" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 1 (ON)" << endl;

	myStatus = irio_setDebugMode(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDebugMode function] Trying to read DebugMode..." << endl << endl;
	myStatus = irio_getDebugMode(&p_DrvPvt,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 3: Configuring Signal Generator Type" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 1 (SINE)" << endl;
	myStatus = irio_setSGSignalType(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGSignalType function] Trying to read SGSignalType0..." << endl;
	myStatus = irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	uint32_t SGFref = 0;
	int SGChannel = 0;
	int32_t samplingRate = 10000000;

	cout << endl << "TEST 4: Set Signal Generator update rate" << endl << endl;
	cout << "[irio_getSGFref function] Trying to read SGFref..." << endl;
	myStatus = irio_getSGFref(&p_DrvPvt,SGChannel,&SGFref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to 10 MSamples/s" << endl;
	myStatus = irio_setSGUpdateRate(&p_DrvPvt,0,SGFref/samplingRate,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGUpdateRate function] Trying to read SGUpdateRate0..." << endl;
	myStatus = irio_getSGUpdateRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 5: Set Signal Generator frequency" << endl << endl;
	uint32_t freqDesired = 100;
	int accIncr = freqDesired*(UINT_MAX/samplingRate);
	cout << "[irio_setSGFreq function] SGFreq0 set to " << accIncr << ", meaning " <<
			freqDesired << " Hz" << endl;
	myStatus = irio_setSGFreq(&p_DrvPvt,0,accIncr,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGFreq function] Trying to read SGFreq..." << endl;
	myStatus = irio_getSGFreq(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 6: Set Signal Generator amplitude" << endl << endl;
	int32_t amplitude = 4096;
	double CVDAC = 0.0;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
	cout << "[irio_setSGAmp function] SGAmp0 set to " << amplitude << ", meaning " << amplitude/CVDAC << " V" << endl;
	myStatus = irio_setSGAmp(&p_DrvPvt,0,amplitude,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGAmp function] Trying to read SGAmp..." << endl;
	 myStatus = irio_getSGAmp(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 7: Configuring the Analog output enable on port 0" << endl << endl;
	cout << "[irio_setAOEnable] AOEnable0 set to 1 (ENABLE)" << endl;
	myStatus = irio_setAOEnable(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAOEnable function] Trying to read AOEnable..." << endl;
	myStatus=irio_getAOEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 8: Testing DMAs' cleaning." << endl << endl;
	cout << "[irio_cleanDMAsTtoHost function] No output if DMAs have been cleaned successfully" << endl;
	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 9: Testing DMAs' set up configuration" << endl << endl;
	cout << "[irio_setUpDMAsTtoHost function] No output if DMAs have been configured successfully" << endl;
	myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	samplingRate = 1000;
	int32_t Fref = 0;
	cout << endl << "TEST 10: Testing DMAs' sampling rate configuration." << endl << endl;

	cout << "[irio_getFref function] Trying to read Fref..." << endl;
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(Fref, 0);
	cout << "FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
	cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to: "
			  << samplingRate << " Samples/s"<< endl;

	myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1000,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHostSamplingRate function] Trying to read DMATtoHostSamplingRate..." << endl;
	myStatus = irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 11: Testing DMAs' Enable" << endl << endl;
	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	// Don't make sense tried to acquire data in this test because there isn't.
	// Only read DMAs overflow register to improve code coverage

	int DMAsOverflow = 0;
	myStatus = irio_getDMATtoHostOverflow(&p_DrvPvt,&DMAsOverflow,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 12: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}








































