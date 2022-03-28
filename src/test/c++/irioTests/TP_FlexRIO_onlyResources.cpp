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
 * Test verifies driver’s ability to read and testing resources in the FPGA.
 * This test is related to the following requirements:
 *
 * PXIE7965R
 * PXIE7966R
 *
 * This test is executed only with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

// CPUDAQ bitfile
TEST(TP_onlyResources, CPUDAQ)
{
	string testName = "TP_onlyResources: Functional test of bitfile CPUDAQ";
	string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.0";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIO_CPUDAQ_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	cout << "Resources user should found:" << endl;
	cout << "2 DMAs" << endl;
	cout << "2 AI" << endl;
	cout << "2 AO" << endl;
	cout << "2 auxAI" << endl;
	cout << "2 auxAO" << endl;
	cout << "2 DI" << endl;
	cout << "2 DO" << endl;
	cout << "2 auxDI" << endl;
	cout << "2 auxDO" << endl;
	cout << "2 SG" << endl << endl;

	myStatus = irio_initDriver("functionalCPUDAQTest",
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

	cout << endl << "TEST 1: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// CPUIMAQ bitfile
TEST(TP_onlyResources, CPUIMAQ)
{
	string testName = "TP_onlyResources: Functional test of bitfile CPUIMAQ";
	string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.0";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIO_CPUIMAQ_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	cout << "Resources user should found:" << endl;
	cout << "1 IMAQ DMAs" << endl;
	cout << "1 CLConfig" << endl;
	cout << "1 CLUART" << endl;
	cout << "2 auxAI" << endl;
	cout << "2 auxAO" << endl;
	cout << "2 DI" << endl;
	cout << "2 DO" << endl;
	cout << "2 auxDI" << endl;
	cout << "2 auxDO" << endl << endl;

	myStatus = irio_initDriver("functionalCPUIMAQTest",
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

	cout << endl << "TEST 1: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Hereafter onlyResources bitfile
TEST(TP_onlyResources, onlyResources)
{
	string testName = "TP_onlyResources: Functional test of bitfile onlyResources";
	string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

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

	myStatus = irio_initDriver("functionalOnlyResourcesTest",
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

	cout << endl << "TEST 1: Closing IRIO driver" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_onlyResources, wrongFPGAVersion)
{
	string testName = "TP_onlyResources: Wrong FPGA version test";
	string testDescription = "Test verifies driver’s ability to detect that FPGA version is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources after failure "
			"on driver initialization" << endl << endl;
	myStatus = irio_initDriver("wrongFPGAVersionTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   "wrongVersion",
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "IRIO Library is designed to create a session before check FPGA version, "
			"so user should finalize IRIO driver. irio_closeDriver() must run successfully" << endl;
	cout << "Closing driver... " << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_onlyResources, wrongFilePath)
{
	string testName = "TP_onlyResources: Wrong file path test";
	string testDescription = "Test verifies driver’s ability to detect that path where bitfiles are located is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources after failure "
			"on driver initialization" << endl << endl;
	myStatus = irio_initDriver("wrongFilePathTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongFilePath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Closing IRIO driver after failure on initialization" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_onlyResources, wrongBitfileName)
{
	string testName = "TP_onlyResources: Wrong bitfile name test";
	string testDescription = "Test verifies driver’s ability to detect that bitfile name is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources after failure "
			"on driver initialization" << endl << endl;
	myStatus = irio_initDriver("wrongBitfileNameTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   "wrongBitfileName",
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << endl << "TEST 1: Closing IRIO driver after failure on initialization" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_onlyResources, wrongRIOSerial)
{
	string testName = "TP_onlyResources: Wrong RIO serial number test";
	string testDescription = "Test verifies driver’s ability to detect that FlexRIO device's serial number is not correct.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources after failure "
			"on driver initialization" << endl << endl;
	myStatus = irio_initDriver("wrongRIOSerialTest",
							   "0x00001",
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
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Closing IRIO driver after failure on initialization" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_onlyResources, wrongRIODevice)
{
	string testName = "TP_onlyResources: Wrong RIO device test";
	string testDescription = "Test verifies driver’s ability to detect that FlexRIO device is not correct";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = "0001";
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice!="7965" && RIODevice!="7966");

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources after failure "
			"on driver initialization" << endl << endl;
	myStatus = irio_initDriver("wrongRIODeviceTest",
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
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Closing IRIO driver after failure on initialization" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

// This test is supossed to fail if not a 7966 device model is passed as environment variable
TEST(TP_onlyResources, differentModel_Serial)
{
	string testName = "TP_onlyResources: RIO device model does not match its serial number";
	string testDescription = "Test verifies driver’s ability to detect that RIO device model does not match its serial number";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	// 7966 model as a environment variable
	string RIODevice = "7966";
	// Serial number of a 7965R model
	string RIOSerial = "0x01666C59";

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";

	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources after failure "
			"on driver initialization" << endl << endl;
	myStatus = irio_initDriver("differentModel_Serial",
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
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Closing IRIO driver after failure on initialization" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

// TODO: Revisar este ultimo test.
//       Por qué no hay warning cuando no encuentra las auxAnalog de 64 bits
//       pero sí cuando faltan el resto de recursos?

// Bitfile used in this test does not have auxiliary 64bits analog I/O ports implemented
// It is the original bitfile also used in C examples
TEST(TP_onlyResources, wrongBitfileResources)
{
	string testName = "TP_onlyResources: Wrong bitfile resources";
	string testDescription = "Test verifies driver’s ability to detect that all "
			                 "expected resources are not implemented in the bitfile";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/failResources/"+RIODevice+"/";

	string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources when there are "
			"missing resources on the bitfile" << endl << endl;
	cout << "Resources user should found:" << endl;
	cout << "1 AO + Error finding AO1Enable" << endl;
	cout << "16 auxAI" << endl;
	cout << "16 auxAO" << endl;
	cout << "16 auxDI" << endl;
	cout << "16 auxDO" << endl;
	cout << "54 DI" << endl;
	cout << "54 DO" << endl;
	cout << "0 SG + Error finding SGFref0 and SGSignalType1" << endl;
	cout << "0 DMAs + Error finding DMATtoHOSTSamplingRate0" << endl << endl;

	myStatus = irio_initDriver("wrongBitfileResourcesTest",
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
	EXPECT_TRUE((myStatus!=IRIO_success) &&
				((status.detailCode==ResourceNotFound_Error) || (status.detailCode==ResourceNotFound_Warning)));

	cout << endl << "TEST 1: Closing IRIO driver" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

