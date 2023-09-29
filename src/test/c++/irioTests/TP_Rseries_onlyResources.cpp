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

/**
 * Test verifies driver’s ability to check the correct communication and interconnection
 * between auxiliar I/O analog/digital FPGA registers.
 * This test is related to the following requirements:
 *
 * PXI-7831R, PXI-7833R, PXI-7841R, PXI-7842R, PXI-7851R, PXI-7852R, PXI-7853R, PXI-7854R
 * 7831R/7833R/7841R/7842R/7851R/7852R/7853R/7854R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio/lsni -v command,
 * the first for the ITER open version and the second one for the closed version

 * In the ITER open version:
 * 		execute "$ export RIOSerial=0x.........."
 * In the closed version:
 * 		execute "$ export RIOSerial=.........."
 *
 * It is also necessary another environment variable to indicate the RIO board to be used
 *
 * For both version:
 * 		execute "$ export RIODevice=xxxx", where xxxx = 7831, 7833, 7841, 7842, 7851, 7852, 7853, 7854
 */

// CPUDAQ bitfile
TEST(TP_Rseries_onlyResources, CPUDAQ)
{
	string testName = "TP_Rseries_onlyResources: Functional test of bitfile CPUDAQ";
	string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalCPUDAQTest";
	string NIRIOmodel = "PXI-"+RIODevice+"R";
	string bitfileName = "Rseries_CPUDAQ_"+RIODevice;
	string FPGAversion = "V1.2"; // User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/*
	 * TEST 0
	 * IRIO DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	cout << "Resources user should found:" << endl;
//	cout << "2 DMAs" << endl;
//	cout << "2 AI" << endl;
//	cout << "2 AO" << endl;
//	cout << "2 auxAI" << endl;
//	cout << "2 auxAO" << endl;
//	cout << "2 DI" << endl;
//	cout << "2 DO" << endl;
//	cout << "2 auxDI" << endl;
//	cout << "2 auxDO" << endl;
//	cout << "2 SG" << endl << endl;

	cout << "3 DMAs" << endl;
	cout << "2 AI" << endl;
	cout << "4 AO" << endl;
	cout << "2 auxAI" << endl;
	cout << "2 auxAO" << endl;
	cout << "2 DI" << endl;
	cout << "2 DO" << endl;
	cout << "2 auxDI" << endl;
	cout << "2 auxDO" << endl;
	cout << "2 SG" << endl << endl;

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
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 1
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 1: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// onlyResources bitfile
TEST(TP_Rseries_onlyResources, onlyResources)
{
	string testName = "TP_Rseries_onlyResources: Functional test of bitfile onlyResources";
	string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalOnlyResourcesTest";
	string NIRIOmodel = "PXI-"+RIODevice+"R";
	string bitfileName = "Rseries_onlyResources_"+RIODevice;
	string FPGAversion = "V1.2"; // User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/*
	 * TEST 0
	 * IRIO DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	cout << "Resources user should found:" << endl;
	cout << "1 DMA" << endl;
	cout << "2 AO" << endl;
	cout << "16 32bits-auxAI" << endl;
	cout << "16 32bits-auxAO" << endl;
	cout << "16 64bits-auxAI" << endl;
	cout << "16 64bits-auxAO" << endl;
	cout << "16 auxDI" << endl;
	cout << "16 auxDO" << endl;
	cout << "54 DI" << endl;
	cout << "54 DO" << endl;
	cout << "2 SG" << endl << endl;

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
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 1
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 1: Closing IRIO driver" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Bitfile used in this test does not have auxiliary 64bits analog I/O ports implemented
// It is the original bitfile also used in C examples
//TEST(TP_Rseries_onlyResources, wrongBitfileResources)
//{
//	string testName = "TP_Rseries_onlyResources: Wrong bitfile resources";
//	string testDescription = "Test verifies driver’s ability to detect that all "
//			                 "expected resources are not implemented in the bitfile";
//
//	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
//	TestUtilsIRIO::displayTitle(testDescription);
//
//	// Environment variables
//	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
//	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");
//
//	string appCallID = "wrongBitfileResourcesTest";
//	string NIRIOmodel = "PXI-"+RIODevice+"R";
//	string bitfileName = "Rseries_onlyResources_"+RIODevice;
//	string FPGAversion = "V1.2"; // User doesn't have to know what FPGA Version is used
//	string filePath = "../resources/failResources/"+RIODevice+"/";
//
//	int myStatus = 0;
//	irioDrv_t p_DrvPvt;
//	TStatus status;
//	irio_initStatus(&status);
//
//	/*
//	 * TEST 0
//	 * IRIO DRIVER INITIALIZATION
//	 */
//	// TODO: REVISAR!!!!!!
//	cout << "TEST 0: Testing driver initialization and resources when there are "
//			"missing resources on the bitfile" << endl << endl;
//	cout << "[Bug7516] This tests checks the correct order in error messages when verbosity=1" << endl;
//	cout << "Resources user should found:" << endl;
//	cout << "2 AO + Error finding AO1Enable" << endl;
//	cout << "16 32bits-auxAI" << endl;
//	cout << "16 32bits-auxAO" << endl;
//	cout << "16 64bits-auxAI" << endl;
//	cout << "16 64bits-auxAO" << endl;
//	cout << "16 auxDI" << endl;
//	cout << "16 auxDO" << endl;
//	cout << "54 DI" << endl;
//	cout << "54 DO" << endl;
//	cout << "0 SG + Error finding SGFref0 and SGSignalType1" << endl;
//	cout << "0 DMAs + Error finding DMATtoHOSTSamplingRate0" << endl << endl;
//
//	myStatus = irio_initDriver(appCallID.c_str(),
//							   RIOSerial.c_str(),
//							   NIRIOmodel.c_str(),
//							   bitfileName.c_str(),
//							   FPGAversion.c_str(),
//							   verbosity,
//							   filePath.c_str(),
//							   filePath.c_str(),
//							   &p_DrvPvt,
//							   &status);
//
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_TRUE(myStatus!=IRIO_success);
//
//	/**
//	 * TEST 1
//	 * IRIO DRIVER CLOSING
//	 */
//	cout << endl << "TEST 1: Closing IRIO driver" << endl << endl;
//	cout << "[irio_closeDriver function] Closing driver..." << endl;
//	myStatus = irio//TEST(TP_Rseries_onlyResources, wrongBitfileResources)
//{
//	string testName = "TP_Rseries_onlyResources: Wrong bitfile resources";
//	string testDescription = "Test verifies driver’s ability to detect that all "
//			                 "expected resources are not implemented in the bitfile";
//
//	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
//	TestUtilsIRIO::displayTitle(testDescription);
//
//	// Environment variables
//	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
//	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");
//
//	string appCallID = "wrongBitfileResourcesTest";
//	string NIRIOmodel = "PXI-"+RIODevice+"R";
//	string bitfileName = "Rseries_onlyResources_"+RIODevice;
//	string FPGAversion = "V1.2"; // User doesn't have to know what FPGA Version is used
//	string filePath = "../resources/failResources/"+RIODevice+"/";
//
//	int myStatus = 0;
//	irioDrv_t p_DrvPvt;
//	TStatus status;
//	irio_initStatus(&status);
//
//	/*
//	 * TEST 0
//	 * IRIO DRIVER INITIALIZATION
//	 */
//
//
//	// TODO: REVISAR!!!!!!
//
//
//
//	cout << "TEST 0: Testing driver initialization and resources when there are "
//			"missing resources on the bitfile" << endl << endl;
//	cout << "[Bug7516] This tests checks the correct order in error messages when verbosity=1" << endl;
//	cout << "Resources user should found:" << endl;
//	cout << "2 AO + Error finding AO1Enable" << endl;
//	cout << "16 32bits-auxAI" << endl;
//	cout << "16 32bits-auxAO" << endl;
//	cout << "16 64bits-auxAI" << endl;
//	cout << "16 64bits-auxAO" << endl;
//	cout << "16 auxDI" << endl;
//	cout << "16 auxDO" << endl;
//	cout << "54 DI" << endl;
//	cout << "54 DO" << endl;
//	cout << "0 SG + Error finding SGFref0 and SGSignalType1" << endl;
//	cout << "0 DMAs + Error finding DMATtoHOSTSamplingRate0" << endl << endl;
//
//	myStatus = irio_initDriver(appCallID.c_str(),
//							   RIOSerial.c_str(),
//							   NIRIOmodel.c_str(),
//							   bitfileName.c_str(),
//							   FPGAversion.c_str(),
//							   verbosity,
//							   filePath.c_str(),
//							   filePath.c_str(),
//							   &p_DrvPvt,
//							   &status);
//
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_TRUE(myStatus!=IRIO_success);
//
//	/**
//	 * TEST 1
//	 * IRIO DRIVER CLOSING
//	 */
//	cout << endl << "TEST 1: Closing IRIO driver" << endl << endl;
//	cout << "[irio_closeDriver function] Closing driver..." << endl;
//	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_EQ(myStatus, IRIO_success);
//}_closeDriver(&p_DrvPvt,0, &status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_EQ(myStatus, IRIO_success);
//}

