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
#include <math.h>
}

#include "testUtils-IRIO.h"
#include <thread>
#include <chrono>
#include <ctime>

// Always max verbosity
static int verbosity = 1;

using std::cout; using std::endl;
using std::string; using std::cerr;

/*
 * Necessary structure to manage data acquisition thread
 * @param p_Drv pointer to irio core struct p_DrvPvt
 * @param flagToFinish flag variable to stop data acquisition when required
 * @param blocksRead number of blocks readed from each DMA channel
 * @param DBuffer buffer that storage all data
 */
struct threadData_t{
	irioDrv_t *p_Drv;
	int flagToFinish;
	long int blocksRead;
	uint64_t *DBuffer;
};

/*
 * Fuction to continuous data acquisition from DMA channel and storage it on a buffer
 *
 * @param[in] data structure that contains all parameters needed to manage the data acquisition
 * @param[in] blocksToRead number of blocks to read from DMA buffer
 * @param[in] timeout timeout to wait until blocksToRead storaged on DMA buffer
 * @param[in] tout boolean variable to use DMA timeout function or not
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 *
 */
static void DMAthread(threadData_t*, int, int, bool, TStatus*);

static void DMAthread(threadData_t* data, int blocksToRead, int timeout, bool tout, TStatus* status) {
	int myStatus;
	int elementsRead = 0;
	int DMATtoHOSTFrameType = 0;

	while(data->flagToFinish == 0){

		if (tout){
			myStatus = irio_getDMATtoHostData_timeout(data->p_Drv,blocksToRead,DMATtoHOSTFrameType,
					                            data->DBuffer,&elementsRead,timeout,status);
			if(myStatus == IRIO_success){
					data->blocksRead += elementsRead;
			}
			else
				TestUtilsIRIO::getErrors(*status);
		}
		else {
			myStatus = irio_getDMATtoHostData(data->p_Drv,blocksToRead,DMATtoHOSTFrameType,
											  data->DBuffer,&elementsRead,status);
			if(myStatus == IRIO_success){
				if (elementsRead == blocksToRead){
					data->blocksRead += elementsRead;
				}
				else {
					usleep(10);
				}
			}
			else{
				TestUtilsIRIO::getErrors(*status);
			}
		}
	}
}

static void performanceTest(bool tout = false);

/**
 * These test verifies the performance of the FlexRIO device using DMA timeout function or not
 * This test is related to the following requirements:
 *
 * PXIE7966R. 7966R device can not be substituted by any other model
 *
 * This test must be executed with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=7966
 */

TEST(TP_FlexRIO_perf, functional) {
	performanceTest();
}

TEST(TP_FlexRIO_perf, functional_timeout) {
	performanceTest(true);
}

static void performanceTest(bool tout) {
	string testName = "TP_FlexRIO_perf: Functional test of bitfile FlexRIO_perf";
	string testDescription = "Test verifies the performance of a FlexRIO device";
	if (tout)
		testDescription = testDescription + " using timeout when acquiring data";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7966";
	string RIOSerial = "0x0177A2AD";

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

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 1
	 * FPGA START
	 */
	cout << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	/*
	 * TEST 2
	 * DEBUG MODE CONFIGURATION
	 */
	myStatus = irio_setDebugMode(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 3
	 * DMA CONFIGURATION
	 */
	myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 4
	 * DMA ENABLE
	 */
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 5
	 * FPGA CLOCK REFERENCE CONFIGURATION
	 */
	int32_t Fref = -1;
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	ASSERT_NE(Fref, 0);
	cout << endl << "FPGA Clock reference, Fref: " << Fref << " Hz" << endl;

	/*
	 * TEST 6
	 * DMA PARAMETERS
	 */
	// User decides how many blocks wants to read. Useful in mod5761 test but useless in performance test
	int blocksToRead = 1;                // Put a random number here
	uint16_t DMATtoHOSTBlockNWords = 0;
	uint16_t DMATtoHOSTNCh = 0;

    myStatus = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt,&DMATtoHOSTBlockNWords,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on getting number of "
				"data blocks from the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	myStatus = irio_getDMATtoHOSTNCh(&p_DrvPvt,&DMATtoHOSTNCh,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on getting number of "
				"DMA channels" << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 7
	 * PERFORMANCE TESTS
	 */
	cout << endl << "*** PERFORMANCE TESTS ***" << endl;
	cout << "These tests pretends to show that IRIO library is not able to overcome the throughput of 800.000MB/s" << endl;

	uint32_t sampleSize = 2;        // 2 bytes for each word read on each DMA channel
	int32_t decimationFactor = 0;
	int i = 0;

	// 1 Sample = 1 word of 8 bytes  ; 8 bytes = 4 DMA channels, 2 bytes of sample size each DMA channel
	// 100 kSamples/seg --> 0.8MB/s  ; 1 MSamples/seg --> 8MB/s
	// 10 MSamples/seg --> 80MB/s    ; 20 MSamples/seg --> 160MB/s
	// 25 MSamples/seg --> 200 MB/s  ; 50 MSamples/seg --> 400MB/s
	// 100 MSamples/seg --> 800MB/s
	std::vector<int32_t> samplingRate = {100000,1000000,10000000,20000000,25000000,50000000,100000000};

	int timeSleep = 5; // User decides how much time the thread is going to acquire data

	// Necessary timeout parameters
	float timePerWord = 0;
	uint32_t timeout = 0;

	/*
	 * NECESSARY THREAD TO RUN PERFORMANCE TESTS
	 */
	threadData_t data;
	data.p_Drv = &p_DrvPvt;
	data.DBuffer = new uint64_t[blocksToRead*DMATtoHOSTBlockNWords*sampleSize*DMATtoHOSTNCh];
	using clock = std::chrono::high_resolution_clock;

	for(std::vector<int>::iterator it = samplingRate.begin() ; it != samplingRate.end(); ++it) {
		i += 1;
		float throughput = (float)(*it)*sampleSize*DMATtoHOSTNCh/1000000; //  MB/s transfered
																		  //  /1000000 = conversion from B/s to MB/s
		cout << endl << "Performance test " << i << endl << endl;

		// Equation applied to set DMATtoHostSamplingRate: Fref/samplesPerSeg=DecimationFactor
		// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
		//		 - samplesPerSeg is the amount of samples that are going to be generated
		//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
		// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
		decimationFactor = Fref/(*it);
		myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,decimationFactor,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		if (tout) {
			timePerWord =  1/((float)(*it))*1000; // *1000 = conversion from segs to millisecs
			timeout = ceil(2*timePerWord*blocksToRead*DMATtoHOSTBlockNWords);  // por exceso
		}

		data.flagToFinish = 0;
		data.blocksRead = 0;
		auto start = clock::now();

		std::thread thr(DMAthread,&data,blocksToRead,timeout,tout,&status);

		myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
			cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
		}
		ASSERT_EQ(myStatus, IRIO_success);

		sleep(timeSleep); // 5 Seconds to wait while the thread acquires data.

		myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		// All data has been read so thread can be joined to main process and be destroyed safely
		data.flagToFinish=1;
		thr.join();

		auto end = clock::now();
		std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds> (end-start);

		float throughput_test = (float) data.blocksRead*DMATtoHOSTBlockNWords*sampleSize*DMATtoHOSTNCh/us.count(); // MB/s

		int DMAsOverflow = -1;
		myStatus = irio_getDMATtoHostOverflow(&p_DrvPvt,&DMAsOverflow,&status);
		if (DMAsOverflow==0){
			cout << "Bandwidth expected: " << throughput << "MB/s during " << timeSleep << " seconds. "
				 << throughput*timeSleep << "MB of data reception are expected" << endl;
			cout << "Bandwidth tested: " << std::fixed << std::setprecision(3) << throughput_test << "MB/s [OK]. Data "
					"transfered: " << std::fixed << std::setprecision(3)
				 <<  throughput_test*us.count()/1000000
				 << " MB" << endl;
		}
		else{
			irio_mergeStatus(&status,Generic_Error,verbosity,
							 "Bandwidth tested: %.2fMB/s [ERROR]. DMA Overflow with data loss\n",throughput);
			TestUtilsIRIO::getErrors(status);
		}
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		ASSERT_EQ(myStatus, IRIO_success);

		myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
			cout << "Test can not continue if there is a failure on cleaning the DMA." << endl;
		}
		ASSERT_EQ(myStatus, IRIO_success);
	}

	delete [] data.DBuffer;

	/**
	 * TEST 8
	 * CLOSING DMAS
	 */
	// Once data acquisition has finished DMAs have to be closed
	myStatus = irio_closeDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 9
	 * IRIO DRIVER CLOSING
	 */
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}
