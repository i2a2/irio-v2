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
using std::string;

/*
 * Test verifies driver’s ability to test cRIO DAQ (Data Acquisition) Profile.
 *
 * This test is related to the following requirements:
 *
 * NI9159 Chassis
 *
 */

TEST(TP_cRIO_DAQ, functional) {
	string testName = "TP_cRIO_DAQ: Functional test of bitfile cRIO_DAQ";
	string testDescription = "Test verifies driver’s ability to check cRIO DAQ profile. "
	                         "This test is focused on the resources implemented in the FPGA "
		                 	 "to improve IRIO code coverage. Several parameters are going to be "
		                     "instantiated with no functionality because cRIO modules are not connected "
		                     "in NI9159 chassis.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.0";
	string NIRIOmodel = "NI 9159";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "cRIO_DAQ";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/*
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	myStatus = irio_initDriver("functionalcRIODAQTest",
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
	EXPECT_EQ(myStatus, IRIO_success);
	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	/**
	 * TEST 2
	 * DAQ START
	 */
//	cout << endl << "TEST 2: Set DAQ Start" << endl << endl;
//
//	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
//	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 3
	 * DMA CONFIGURATION
	 */
	cout << endl << "IMPORTANTE: Revisar bitfile (uso de DMAs) y ver qué hacer" << endl;
	// TODO: Que hacer con todo lo de DMAs? Esto se queda colgado
//	cout << endl << "TEST 3: Testing DMAs' set up configuration." << endl << endl;
//	cout << "[irio_setUpDMAsTtoHost function] No output if DMAs have been configured successfully" << endl;
//	// All DMAs are configured. In this case there is only one DMA implemented in the FPGA
//	// with four channels (every channel has a digital value of 16 bits)
//	myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 4
	 * DMA ENABLE
	 */
//	cout << endl << "TEST 4: Testing DMAs' Enable" << endl << endl;
//
//	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
//	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);
//
//	myStatus = irio_getDMATtoHostEnable(&p_DrvPvt,0,&valueReadI32,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);
//	cout << "[irio_getDMATtoHostEnable function] DMATtoHostEnable0 read: " << valueReadI32 << endl;

	// TODO: Algo de DMAs? Bitfile tiene FIFOs pero ninguna conexion interna...

	/**
	 * TEST 5
	 * GETTING DATA FROM DMA
	 */
//	cout << endl << "TEST 5: Getting 1 block from DMA" << endl << endl;
//	int sampleCounter = 0;
//	int elementsRead = 0;
//	int positiveTest = 0;
//	int negativeTest = 0;
//
//	uint16_t DMATtoHOSTBlockNWords = 0;
//	uint16_t DMATtoHOSTNCh = 0;
//
//    myStatus = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt,&DMATtoHOSTBlockNWords,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on getting number of "
//				"data blocks from the DMA." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);
//	cout << "[irio_getDMATtoHOSTBlockNWords function] Acquiring number of words for each DMA block. "
//			"BlockNWords: " << DMATtoHOSTBlockNWords << endl;
//
//	myStatus = irio_getDMATtoHOSTNCh(&p_DrvPvt,&DMATtoHOSTNCh,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on getting number of "
//				"DMA channels" << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);
//	cout << "[irio_getDMATtoHOSTNCh function] Acquiring number of DMA channels. "
//			"DMATtoHOSTNCh: " << DMATtoHOSTNCh << endl;
//
//	// User decides how many blocks wants to read
//	int blocksToRead = 1;
//	// Each block: 4096 words * 64 bits/word = 4096 words * 8 bytes/word
//	uint64_t* dataBuffer = new uint64_t[blocksToRead*DMATtoHOSTBlockNWords*8];
//
//	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl << endl;
//	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);

	// TODO: Se lo traga pero el buffer está vacío
//	do{
//		myStatus = irio_getDMATtoHostData(&p_DrvPvt,blocksToRead,0,dataBuffer,&elementsRead,&status); // blocksToRead blocks of 4096 64 bit words are expected to be acquired
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//		ASSERT_EQ(myStatus, IRIO_success);
//
//		if(elementsRead==blocksToRead){ // blocksToRead blocks are available
//			// Shorted to 16 bits instead of 64 because we are only using only one DMA channel, not all four
//			short int* auxDataBuffer = new short int[blocksToRead*DMATtoHOSTBlockNWords*8];
//			auxDataBuffer = (short int*) dataBuffer;
//			sampleCounter++;
//			for (int i=0;i<blocksToRead*DMATtoHOSTBlockNWords;i++){
//				if(auxDataBuffer[(i*DMATtoHOSTNCh)+2]==2048) //Reads FPGA channel 2
//					positiveTest++;
//				else
//					negativeTest++;
//			}
//		}
//		else
//			usleep(DMATtoHOSTBlockNWords/samplingRate*500000);// we wait at least half the duration of the block in microseconds
//	} while (sampleCounter<1);
//
//	delete [] dataBuffer;
//
//	cout << "Signal generator configured with DC signal and digital value 2048, is connected "
//			     "(internally in the FPGA) to the AI channel 2" << endl;
//	cout << blocksToRead*DMATtoHOSTBlockNWords << " samples has been read from channel 2, "
//			"and 2048 value is expected in all of them" << endl;
//	cout << "Samples correctly read: "   << positiveTest << endl;
//	cout << "Samples incorrectly read: " << negativeTest << endl << endl;
//
//	cout << "[irio_setDAQStartStop function] Stop data acquisition" << endl;
//	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on stopping the DAQ." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);
//
//	cout << "[irio_cleanDMAsTtoHost function] Cleaning DMAs" << endl;
//	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//		cout << "Test can not continue if there is a failure on cleaning the DMA." << endl;
//	}
//	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 6
	 * CLOSING DMAS
	 */
	// Once data acquisition has finished DMAs have to be closed
//	cout << endl << "TEST 6: Closing DMAS" << endl << endl;
//	cout << "[irio_closeDMAsTtoHost function] No output if DMAs have been closed successfully" << endl;
//	myStatus = irio_closeDMAsTtoHost(&p_DrvPvt,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_EQ(myStatus, IRIO_success);

	/*
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

TEST(TP_cRIO_DAQ, cRIO_DAQDMA) {
	string testName = "TP_cRIO_DAQ: Functional test of bitfile cRIO_DAQDMA";
	string testDescription = "Test verifies driver’s ability to check cRIO device "
            				 "and cRIO modules connected to it. It returns IRIO driver "
							 "error if cRIO modules are missing";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="9159") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "NI 9159";

	// Original bitfile. Check that all cRIO modules are connected to avoid errors
	string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	string bitfileName = "cRIODAQDMA_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/*
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	myStatus = irio_initDriver("functionalcRIODAQDMATest",
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
	EXPECT_NE(myStatus, IRIO_success);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	/*
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
