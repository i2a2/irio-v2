#include <gtest/gtest.h>

#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <boost/version.hpp>
#include <boost/regex.hpp>

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

/**
 * Test verifies driver’s ability to read and testing resources in the FPGA.
 * This test is related to the following requirements:
 *
 * PXIE7965R
 * PXIE7966R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

// CPUDAQ bitfile
TEST(TP_onlyResources, CPUDAQ)
{
	std::string testName = "TP_onlyResources: Functional test of bitfile CPUDAQ";
	std::string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.0";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIO_CPUDAQ_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Resources user should found:" << std::endl;
	std::cout << "2 DMAs" << std::endl;
	std::cout << "2 AI" << std::endl;
	std::cout << "2 AO" << std::endl;
	std::cout << "2 auxAI" << std::endl;
	std::cout << "2 auxAO" << std::endl;
	std::cout << "2 DI" << std::endl;
	std::cout << "2 DO" << std::endl;
	std::cout << "2 auxDI" << std::endl;
	std::cout << "2 auxDO" << std::endl;
	std::cout << "2 SG" << std::endl << std::endl;

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

	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}
}

// CPUIMAQ bitfile
TEST(TP_onlyResources, CPUIMAQ)
{
	std::string testName = "TP_onlyResources: Functional test of bitfile CPUIMAQ";
	std::string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.0";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIO_CPUIMAQ_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Resources user should found:" << std::endl;
	std::cout << "1 IMAQ DMAs" << std::endl;
	std::cout << "1 CLConfig" << std::endl;
	std::cout << "1 CLUART" << std::endl;
	std::cout << "2 auxAI" << std::endl;
	std::cout << "2 auxAO" << std::endl;
	std::cout << "2 DI" << std::endl;
	std::cout << "2 DO" << std::endl;
	std::cout << "2 auxDI" << std::endl;
	std::cout << "2 auxDO" << std::endl << std::endl;

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

	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}
}

// It is supposed that every parameter that can fail (rioSerial, rioDevice, filePath, bitfileName, ...)
// are correct. In next GoogleTests this parameters are going to be tested
// when its values are wrong to catch different IRIO errors

// Hereafter onlyResources bitfile
TEST(TP_onlyResources, onlyResources)
{
	std::string testName = "TP_onlyResources: Functional test of bitfile onlyResources";
	std::string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Resources user should found:" << std::endl;
	std::cout << "1 DMA" << std::endl;
	std::cout << "2 AO" << std::endl;
	std::cout << "16 auxAI" << std::endl;
	std::cout << "16 auxAO" << std::endl;
	std::cout << "16 auxDI" << std::endl;
	std::cout << "16 auxDO" << std::endl;
	std::cout << "54 DI" << std::endl;
	std::cout << "54 DO" << std::endl;
	std::cout << "2 SG" << std::endl << std::endl;

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

	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_EQ(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}
}

TEST(TP_onlyResources, wrongFPGAVersion)
{
	std::string testName = "TP_onlyResources: Wrong FPGA version test";
	std::string testDescription = "Test verifies driver’s ability to detect that FPGA version is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// Wrong FPGA Version
	std::string FPGAversion = "V1.0";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";

	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongFPGAVersionTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "IRIO Library is designed to create a session before check FPGA version, so irio_closeDriver() must run successfully" << std::endl;
		std::cerr << "Closing driver... " << std::endl;
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver (twice)... " << std::endl;
	}
}

TEST(TP_onlyResources, wrongFilePath)
{
	std::string testName = "TP_onlyResources: Wrong file path test";
	std::string testDescription = "Test verifies driver’s ability to detect that path where bitfiles are located is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// Wrong bitfile path
	std::string filePath = "WrongPath/";

	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongFilePathTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver..." << std::endl;
	}
}

TEST(TP_onlyResources, wrongBitfileName)
{
	std::string testName = "TP_onlyResources: Wrong bitfile name test";
	std::string testDescription = "Test verifies driver’s ability to detect that bitfile name is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";

	// Wrong bitfile name
	std::string bitfileName = "WrongBitfileName";

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongBitfileNameTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver..." << std::endl;
	}
}

TEST(TP_onlyResources, wrongRIOSerial)
{
	std::string testName = "TP_onlyResources: Wrong RIO serial number test";
	std::string testDescription = "Test verifies driver’s ability to detect that FlexRIO device's serial number is not correct.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = "0x0001";

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongRIOSerialTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver..." << std::endl;
	}
}

TEST(TP_onlyResources, wrongRIODevice)
{
	std::string testName = "TP_onlyResources: Wrong RIO device test";
	std::string testDescription = "Test verifies driver’s ability to detect that FlexRIO device is not correct";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = "0001";
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice!="7965" && RIODevice!="7966");

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

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

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver..." << std::endl;
	}
}

// This test is supossed to fail if not a 7966 device model is passed as environment variable
TEST(TP_onlyResources, differentModel_Serial)
{
	std::string testName = "TP_onlyResources: RIO device model does not match its serial number";
	std::string testDescription = "Test verifies driver’s ability to detect that RIO device model does not match its serial number";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	// 7966 model as a environment variable
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	// Serial number of a 7965R model
	std::string RIOSerial = "0x01666C59";

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";

	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

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

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver..." << std::endl;
	}
}

TEST(TP_onlyResources, wrongBitfileResources)
{
	std::string testName = "TP_onlyResources: Wrong bitfile resources";
	std::string testDescription = "Test verifies driver’s ability to detect that all expected resources are not implemented in the bitfile";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";
	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	// Path of  a bitfile with missing resources
	std::string filePath = "../../../main/c/examples/resourceFail/"+RIODevice+"/";

	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	std::cout << "Resources user should found:" << std::endl;
	std::cout << "1 AO + Error finding AO1Enable" << std::endl;
	std::cout << "16 auxAI" << std::endl;
	std::cout << "16 auxAO" << std::endl;
	std::cout << "16 auxDI" << std::endl;
	std::cout << "16 auxDO" << std::endl;
	std::cout << "54 DI" << std::endl;
	std::cout << "54 DO" << std::endl;
	std::cout << "0 SG + Error finding SGFref0 and SGSignalType1" << std::endl;
	std::cout << "0 DMAs + Error finding DMATtoHOSTSamplingRate0" << std::endl << std::endl;

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

	EXPECT_TRUE(((int)status.detailCode!=IRIO_success) &&
			    ((status.detailCode==ResourceNotFound_Error) || (status.detailCode==ResourceNotFound_Warning)));
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "IRIO Library is designed to create a session before check bitfile resources, so irio_closeDriver() must run successfully" << std::endl;
		std::cerr << "Closing driver... " << std::endl;
	}

	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE(status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		char* detailStr = nullptr;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		free(detailStr);
		std::cerr << "Closing driver (twice)... " << std::endl;
	}
}

