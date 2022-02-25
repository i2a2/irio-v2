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
int verbosity = 1;

using namespace std;

// Modified IRIO Library function
std::string getErrorString(const TStatus& status);

/**
 * Test verifies driver’s ability to read and testing resources in the FPGA.
 * This test is related to the following requirements:
 *
 * PXIE7966R
 * PXIE7965R
 * PXIE7961R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

TEST(TP_onlyResources, functional)
{
	// It is supposed that every parameter that can fail (rioSerial, rioDevice, filePath, bitfileName)
	// are correct. In next GoogleTest this parameters are going to be tested
	// when its values are wrong to catch different exceptions

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
		std::cerr << "Runtime error code(0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl;
	}

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_EQ((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl << std::endl;
		std::cerr << "Closing driver..." << std::endl;
	}
}

TEST(TP_onlyResources, wrongPath)
{
	std::string exception;
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
	std::string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";

	std::string bitfileName = "wrongPathTest";

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
		std::cerr << "Runtime error code(0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl;
		}

	irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code(0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl << std::endl;
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
		std::cerr << "Runtime error code(0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl;
	}

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl << std::endl;
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

	std::string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	std::string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
//	ASSERT_FALSE(RIODevice!="7965" && RIODevice!="7966");
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
		std::cerr << "Runtime error code(0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl;
	}

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);

	EXPECT_NE((int) status.detailCode, IRIO_success);
	if (myStatus > IRIO_success) {
		std::cerr << "Runtime error code (0-Success; 1-Warning; 2-Error): " << status.code << std::endl;
		std::cerr << "Runtime error detail code: " << status.detailCode << ", " << getErrorString(status) << std::endl << std::endl;
		std::cerr << "Closing driver..." << std::endl;
	}
}


std::string getErrorString(const TStatus& status){
	std::string error;
	switch(status.detailCode){
		case Generic_Error:
			error = "Error generated outside irio. Check error messages for more detailed information";
			break;
		case HardwareNotFound_Error:
			error="Specified RIO device not found. Review specified serial number.";
			break;
		case BitfileDownload_Error:
			error="Error occurr downloading the bitfile. Check if bitfile was compiled for the specified target.";
			break;
		case InitDone_Error://!< Could not configure adapter module
			error="Init done wait ended in timeout. Check if the connected adapter module is the intended and is properly connected.\n Check the initialization logic in of the project.";
			break;
		case IOModule_Error:			//!< IO module/s check error
			error="Connected IO module is not the expected IO Module\n Review bitfile downloaded.";
			break;
		case NIRIO_API_Error:
			error="NI-RIO C API returned error. Check returned error in error log.";
			break;
		case ListRIODevicesCommand_Error:
			error="System call for listing connected RIO devices failed. Check if NI-RIO driver is installed and the executable is accessible.";
			break;
		case ListRIODevicesParsing_Error:
			error="Failed to parse system call for listing connected RIO. Check version of NI-RIO Driver installed.";
			break;
		case SignatureNotFound_Error:
			error="Can't find bitfile signature string in the given header file. Check NI FPGA C API Generator version used.";
			break;
		case ResourceNotFound_Error:
			error="A mandatory resource wasn't found. Check error messages to find missing resources.";
			break;
		case SignatureValueNotValid_Error:
			error="Can't read value of bitfile signature in the given header file. Check NI FPGA C API Generator version used.";
			break;
		case ResourceValueNotValid_Error:
			error="Can't read address of a resource in the given header file. Check NI FPGA C API Generator version used.";
			break;
		case MemoryAllocation_Error:
			error="IRIO tried to allocate memory unsuccessfully. Check possible memory leaks in user application.";
			break;
		case BitfileNotFound_Error:
			error="Bitfile file for specified project was not found. Check project name and search path.";
			break;
		case HeaderNotFound_Error:
			error="Header file for specified project was not found. Check project name and search path.";
			break;
		case FileAccess_Error:
			error="A problem occur while reading a file. Check file permissions.";
			break;
		case FileNotFound_Error:
			error="Tried to open a file and failed. Check file name and search path.";
			break;
		case FeatureNotImplemented_Error:
			error="The target device or device profile are not supported. Check values in the project with the design rules";
			break;
		case Success:
			error="Device status is OK";
			break;
		case TemporaryFileDelete_Warning:
			error="Could not delete temporary file. Check /tmp/ folder permissions or previous warnings/errors.";
			break;
		case ResourceNotFound_Warning: //!< Part of optional resource not found (e.g. AO0 Present but AOEnable0 not present)
			error="An optional resource (or part of it for complex resources) wasn't found. Check error messages to find missing resources.";
			break;
		case FPGAAlreadyRunning_Warning:	//!< Tried to change FPGAStart after FPGA was started/FPGA was started (bitfile downloaded was already in the FPGA)
			error="Tried change FPGAStart value after FPGA was started or FPGA was started before downloading the bitfile (previous session). If this is not the intended behaviour, reset the FPGA.";
			break;
		case Read_NIRIO_Warning:
			error="Tried to access an existing FPGA resource unsuccessfully. Check error messages to find NI-RIO driver error code.";
			break;
		case Read_Resource_Warning:		//!< Tried to access a not-present resource
			error="Tried to access a non-existing FPGA resource. Check implemented resources in the project or enabling verbosity mode in irio_initDriver call.";
			break;
		case Write_NIRIO_Warning:
			error="Tried to write in an existing FPGA resource unsuccessfully. Check error messages to find NI-RIO driver error code.";
			break;
		case Write_Resource_Warning:
			error="Tried to write in a non-existing FPGA resource. Check implemented resources in the project or enabling verbosity mode in irio_initDriver call.";
			break;
		case ConfigDMA_Warning:			//!< Attempt to Configure, Start or Stop DMA failed
			error="Tried to configure/start/stop DMA FIFO unsuccessfully. Check error messages to find NI-RIO driver error code.";
			break;
		case ConfigUART_Warning:			//!< Timeout for setting baudrate
			error="Tried to configure UART baud rate unsuccessfully. UART seems busy.";
			break;
		case ValueOOB_Warning:
			error="Tried to write a value Out of Bounds. Check design rules for operational range.";
			break;
		case Generic_Warning:
			error="Warning generated outside irio. Check warning messages for more detailed information";
			break;
		case DAQtimeout_Warning:
			error="Timeout reached when waiting for data from data acquisition";
			break;
		case CLSLinetimeout_Warning:
			error="Timeout reached when waiting for cameralink serial data";
			break;
		case ResourceRelease_Warning:
			error="NIRIO error while releasing Fifo elements";
			break;
		default:
			error="Error code not defined";
			break;
	}
	return error;
}


