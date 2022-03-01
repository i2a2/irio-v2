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

using namespace std;

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

TEST(TP_onlyResources, functional)
{
	// It is supposed that every parameter that can fail (rioSerial, rioDevice, filePath, bitfileName)
	// are correct. In next GoogleTests this parameters are going to be tested
	// when its values are wrong to catch different IRIO errors

	std::string testName = "TP_onlyResources: Functional test";
	std::string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	printf("Resources user should found: \n");
	printf("1 DMA\n");
	printf("2 AO\n");
	printf("16 auxAI\n");
	printf("16 auxAO\n");
	printf("16 auxDI\n");
	printf("16 auxDO\n");
	printf("54 DI \n");
	printf("54 DO \n");
	printf("2 SG\n");

	myStatus = irio_initDriver("functionalTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_EQ((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;

		// Formato C
//		char * detailStr = NULL;
//		irio_getErrorString(status.detailCode, &detailStr);
//		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;

		// Formato C++
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
	}

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_EQ((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;

		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
	}
}

TEST(TP_onlyResources, wrongFPGAVersion)
{
	std::string testName = "TP_onlyResources: Wrong FPGA version test";
	std::string testDescription = "Test verifies driver’s ability to detect that the NIRIO model is not correct";

	// Wrong FPGA Version
	std::string FPGAversion = "V1.0";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";

	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongFPGAVersion",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
	}

	// TODO: Este test es el único que cierra la sesión correctamente, debido a que en IRIO se pasa el parámetro p_DrvPvt->session inicializado
	//       en algún lugar en vez de pasarlo como al inicializarse mal la FPGA. Revisar IRIO
	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
	}
}

TEST(TP_onlyResources, wrongFilePath)
{
	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;

	std::string testName = "TP_onlyResources: Wrong path test";
	std::string testDescription = "Test verifies driver’s ability to detect that path where bitfiles are located is not correct";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	// Wrong bitfile path
	std::string filePath = "WrongPath/";

	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongPathTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
		}

	irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
		std::cerr << "Closing driver..." << std::endl;
		}
}

TEST(TP_onlyResources, wrongBitfileName)
{
	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;

	std::string testName = "TP_onlyResources: Wrong bitfile name test";
	std::string testDescription = "Test verifies driver’s ability to detect that bitfile name is not correct";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";

	// Wrong bitfile name
	std::string bitfileName = "WrongBitfileName";

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

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
		}

	irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
		std::cerr << "Closing driver..." << std::endl;
		}
}

TEST(TP_onlyResources, wrongRIOSerial)
{
	std::string testName = "TP_onlyResources: Wrong RIO serial number test";
	std::string testDescription = "Test verifies driver’s ability to detect that FlexRIO device's serial number is not correct.";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = "0x0001";

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongRIOSerial",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
	}

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
		std::cerr << "Closing driver..." << std::endl;
	}
}

TEST(TP_onlyResources, wrongRIODevice)
{
	std::string testName = "TP_onlyResources: Wrong RIO device test";
	std::string testDescription = "Test verifies driver’s ability to detect that FlexRIO device is not correct";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.1";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string RIODevice = "0001";
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice!="7965" && RIODevice!="7966");


	std::string NIRIOmodel = "PXIe-"+RIODevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";
	std::string bitfileName = "FlexRIOonlyResources_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver("wrongRIODevice",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
	}

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		char* detailStr = new char;
		irio_getErrorString(status.detailCode, &detailStr);
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << detailStr << std::endl;
		delete detailStr;
		std::cerr << "Closing driver..." << std::endl;
	}
}
