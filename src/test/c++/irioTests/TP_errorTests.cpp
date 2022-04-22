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
#include <irioHandlerImage.h>
#include <limits.h>
}

#include "testUtils-IRIO.h"

// Always max verbosity
static int verbosity = 1;

using std::cout; using std::endl;
using std::string;

/**
 * These tests are designed to improve IRIO code coverage
 *
 * PXIE7965R
 * PXIE7966R
 *
 * The execution of some of these tests requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 *
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */


// Set of tests using onlyResources bitfile
TEST(TP_errorTests, wrongFPGAVersion)
{
	string testName = "TP_errorTests, onlyResources bitfile: Wrong FPGA version test";
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

	cout << endl << "TEST 1: Closing IRIO driver" << endl << endl;
	cout << "Closing driver... " << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_errorTests, wrongFilePath)
{
	string testName = "TP_errorTests, onlyResources bitfile: Wrong file path test";
	string testDescription = "Test verifies driver’s ability to detect that path "
							 "where bitfiles are located is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
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
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_errorTests, wrongRIOSerial)
{
	string testName = "TP_errorTests, onlyResources bitfile: Wrong RIO serial number test";
	string testDescription = "Test verifies driver’s ability to detect that FlexRIO device's serial number is not correct.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
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
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_errorTests, differentModel_Serial)
{
	string testName = "TP_errorTests, onlyResources bitfile: RIO device model does not match its serial number";
	string testDescription = "Test verifies driver’s ability to detect that RIO device model does not match its serial number";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// This serial number match with the 7965R device, not the 7966R
	string RIOSerial = "0x01666C59";
	string RIODevice = "7966";

	// User doesn't have to know what FPGA Version is used
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
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_errorTests, wrongBitfileName)
{
	string testName = "TP_errorTests, onlyResources bitfile: Wrong bitfile name test";
	string testDescription = "Test verifies driver’s ability to detect that bitfile name is not correct";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
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
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_errorTests, wrongRIODevice)
{
	string testName = "TP_errorTests, onlyResources bitfile: Wrong RIO device test";
	string testDescription = "Test verifies driver’s ability to detect that FlexRIO device is not correct";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = "0001";
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// User doesn't have to know what FPGA Version is used
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
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

// Set of tests using noModule bitfile
TEST(TP_errorTests, setFPGATwice) {
	string testName = "TP_errorTests, noModule bitfile: Set FPGA twice test";
	string testDescription = "Test verifies driver’s ability to check that the "
							 "FPGA is started twice in a row";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("setFPGATwiceTest",
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

	cout << endl << "TEST 1: Testing FPGA start mode twice" << endl << endl;
	cout << "First start of the FPGA, no error expected" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	int aivalue = 0;

	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl << endl;

	cout << "Second start of the FPGA, error/warning expected" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_errorTests, setFPGAInitError) {
	string testName = "TP_errorTests, noModule bitfile: Start FPGA after a failure on the driver's initialization test";
	string testDescription = "Test verifies driver’s ability to check that the FPGA can start if "
							 "there is a failure on driver's initialization";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("setFPGAInitErrorTest",
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

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int aivalue=0;

	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl << endl;
	if (aivalue == 0)
		cout << "IRIO can not initialize the FPGA if there is not a driver's session open" << endl;

	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_errorTests, manageNonImplementedPorts) {
	string testName = "TP_errorTests, noModule bitfile: non implemented auxiliary analog/digital I/O ports test";
	string testDescription = "Test verifies driver’s ability to check the error "
			                 "if user tries to write on a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("nonImplementedPortsTest",
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

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	usleep(100);
	int analogValue = 0;

	cout << endl << "TEST 2: Testing an auxiliary analog output port not "
			        "implemented in the FPGA setting a value" << endl << endl;

	cout << "[irio_setAuxAO function] Value " << analogValue << " is set in not "
			"implemented auxAO6. Error/warning expected" << endl;
	myStatus = irio_setAuxAO(&p_DrvPvt,6,analogValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Testing an auxiliary analog output port not "
		 	        "implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxAO function] Trying to read auxAO6..." << endl;
	myStatus = irio_getAuxAO(&p_DrvPvt,6,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 4: Testing an auxiliary analog input port not "
			        "implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxAI function] Trying to read auxAI10..." << endl;
	myStatus = irio_getAuxAI(&p_DrvPvt,10,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

//	TODO: Commented until noModule bitfile is designed with auxiliary 64 bits analog I/O ports
	usleep(100);
	cout << endl << "TEST 5: Testing an auxiliary 64 bits analog output port not "
					"implemented in the FPGA setting a value" << endl << endl;

	int64_t avalue = 0;
	cout << "[irio_setAuxAO_64 function] Value " << avalue << " is set in not "
			"implemented aux64AO6. Error/warning expected" << endl;
	myStatus=irio_setAuxAO_64(&p_DrvPvt,6,avalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}

	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 6: Testing an auxiliary 64 bits analog output port not "
					"implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxAO_64 function] Trying to read aux64AO6..." << endl;
	myStatus = irio_getAuxAO_64(&p_DrvPvt,6,&avalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 7: Testing an auxiliary 64 bits analog input port not "
					"implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxAI_64 function] Trying to read aux64AI10..." << endl;
	myStatus = irio_getAuxAI_64(&p_DrvPvt,10,&avalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	int digitalValue = 0;

	cout << endl << "TEST 8: Testing an auxiliary digital output port not "
			        "implemented in the FPGA setting a value" << endl << endl;

	cout << "[irio_setAuxDO function] Value " << digitalValue << " is set in not "
			"implemented auxDO6. Error/warning expected" << endl;
	myStatus=irio_setAuxDO(&p_DrvPvt,6,digitalValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 9: Testing an auxiliary digital output port not "
		 	        "implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxDO function] Trying to read auxDO6..." << endl << endl;
	myStatus = irio_getAuxDO(&p_DrvPvt,6,&digitalValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 10: Testing an auxiliary digital input port not "
			        "implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxDI function] Trying to read auxDI6..." << endl;
	myStatus = irio_getAuxDI(&p_DrvPvt,6,&digitalValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 11: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Test using FlexRIO mod5761 bitfile
TEST(TP_errorTests, failInitDriver5761) {
	string testName = "TP_errorTests, mod5761 bitfile. Failure on driver initilization test";
	string testDescription = "Test verifies several warnings and errors if "
			                 "user tries to configure the FPGA when there is a "
			                 "failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7966";
	string RIOSerial = "0x0177A2AD";
	string Coupling  = TestUtilsIRIO::getEnvVar("Coupling");

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod5761_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod5761",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongPath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	// Expect warnings or error so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int valueReadI32 = 0;
	usleep(100);
	cout << endl << "TEST 1: Testing debug mode configuration, OFF mode after failure on initializing the driver" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 0 (OFF)" << endl;

	myStatus = irio_setDebugMode(&p_DrvPvt,0,&status); // Debug mode set to OFF, value 0
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDebugMode function] Trying to read DebugMode..." << endl;
	myStatus = irio_getDebugMode(&p_DrvPvt,&valueReadI32,&status); // DebugMode FPGA register is read
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 2: Configuring Signal Generator Signal type after failure on initializing the driver" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 0 (DC)" << endl;
	myStatus = irio_setSGSignalType(&p_DrvPvt,0,0,&status); // DC signal configured, value 0
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGSignalType function] Trying to read SG Signal type..." << endl;
	myStatus = irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	int digitalValue = 2048;
	cout << endl << "TEST 3: Configuring the Analog output on port 0 with a digital value after failure on initializing the driver" << endl << endl;
	cout << "[irio_setAO function] AO0 set to 2048 (digital value)" << endl;

	myStatus = irio_setAO(&p_DrvPvt,0,digitalValue,&status); // Set AO0 terminal to 2048 digital value
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAO function] Trying to read Analog Output 0..." << endl;
	myStatus = irio_getAO(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 4: Configuring the Analog output enable on port 0 after failure on initializing the driver" << endl << endl;
	cout << "[irio_setAOEnable] AOEnable0 set to 1 (ENABLE)" << endl;

	myStatus = irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAOEnable function] Trying to read AOEnable0... " << endl;
	myStatus = irio_getAOEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);


	int valueReadAuxAI9 = 0;
	usleep(100);
	cout << endl << "TEST 5: auxAI9 register after failure on initializing "
				 "the driver" << endl << endl;

	cout << "[irio_getAuxAI function] Trying to read auxAI9... " << endl;
	myStatus = irio_getAuxAI(&p_DrvPvt,9,&valueReadAuxAI9,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 6: Testing DMAs cleaning after failure on initializing the driver. "
			     "No output if DMAs have been cleaned successfully" << endl;

	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 7: Testing configurations of DMAs after failure on initializing "
			     "the driver. No output if DMAs have been configured successfully" << endl;

	myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int32_t samplingRate = 500000; // 500k samples/seg
	int32_t Fref = 0;
	usleep(100);
	cout << endl << "TEST 8: Testing DMAs sampling rate configuration after failure on initializing the driver." << endl << endl;

	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		cout << "IRIO can not take the Clock reference of the FPGA for signal generation" << endl;
	}
	EXPECT_EQ(Fref, 0);
	cout << "FPGA Clock reference (Fref value) is: " << Fref << " Hz" << endl;
	cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to: "
			  << samplingRate << " Samples/s"<< endl;

	int32_t decimationFactor = Fref/samplingRate;
	myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,decimationFactor,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHostSamplingRate function] Trying to read sampling rate for DMA0..."  << endl;
	myStatus=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int coupling = std::stoi(Coupling);
	usleep(100);
	cout << endl << "TEST 9: Testing coupling configuration for NI5761 adapter module" << endl << endl;

	cout << "[irio_setAICoupling function] AICoupling (AC = 0, DC = 1) set to : " << coupling << endl;
	myStatus = irio_setAICoupling(&p_DrvPvt,(TIRIOCouplingMode) coupling,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAICoupling function] Trying to read AICoupling..." << endl;
	myStatus = irio_getAICoupling(&p_DrvPvt,(TIRIOCouplingMode*) &coupling,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 10: Testing DMA Enable" << endl << endl;

	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHostEnable function] Trying to read DMATtoHostEnable0..." << endl;
	myStatus=irio_getDMATtoHostEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 11: Set DAQ Start" << endl << endl;

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDAQStartStop function] Trying to read DAQStartStop..." << endl;
	myStatus = irio_getDAQStartStop(&p_DrvPvt,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(1000);
	int sampleCounter = 0;
	int elementsRead = 0;
	int positiveTest = 0;
	int negativeTest = 0;
	uint64_t *dataBuffer = new uint64_t[4096*8]; //4096 data block size
	uint16_t DMATtoHOSTBlockNWords = 0;
	uint16_t DMATtoHOSTNCh = 0;

	cout << endl << "TEST 12: Get 1 block from DMA" << endl << endl;

	cout << "[irio_getDMATtoHOSTBlockNWords function] Trying to read DMATtoHOSTBlockNWords..." << endl << endl;
	myStatus = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt,&DMATtoHOSTBlockNWords,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHOSTNCh function] Trying to read DMATtoHOSTNCh..." << endl << endl;
	myStatus = irio_getDMATtoHOSTNCh(&p_DrvPvt,&DMATtoHOSTNCh,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	do{
		cout << "[irio_getDMATtoHostData function] Trying to read DMATtoHostData..." << endl << endl;
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer,&elementsRead,&status); //1 block of 4096 64 bit words are expected to be acquired
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else {
			if(elementsRead==1){ //one block is available
				short int* auxDataBuffer = new short int [4096*8];
				auxDataBuffer=(short int *)dataBuffer;
				sampleCounter++;
				for (int i=0;i<DMATtoHOSTBlockNWords;i++){
					if((auxDataBuffer[(i*DMATtoHOSTNCh)+2]==2048)) //Reads FPGA channel 2
						positiveTest++;
					else
						negativeTest++;
				}
			}
			else
				usleep(((1/((double)samplingRate))*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds

			cout << "Signal generator configured with DC signal and digital value 2048, is connected "
						 "(internally in the FPGA) to the AI channel 2" << endl;
			cout << "4096 samples has been read from channel 2, and 2048 value is expected in all "
						 "of them" << endl;
			cout << "Samples correctly read: " << positiveTest << endl << "Samples incorrectly "
						 "read: " << negativeTest << endl << endl;
		}
		EXPECT_NE(myStatus, IRIO_success);
	} while ((myStatus ? 0 : 1) && sampleCounter < 1);

	usleep(100000); // This sleep time is to ensure that FIFO is full of data. FIFO will be cleaned below

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 13: Testing DMAs cleaning after failure on initializing the driver. "
			     "No output if DMAs have been cleaned successfully" << endl;

	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	uint32_t SGFref = 0;
	int SGChannel = 0;
	samplingRate = 10000000;

	cout << endl << "TEST 14: Set Signal Generator update rate" << endl << endl;
	myStatus=irio_getSGFref(&p_DrvPvt,SGChannel,&SGFref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "FPGA SignalGenerator Fref has the value: " << SGFref << endl;
	// TODO: Ver cómo poner esto para evitar core dumped
//	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to " << SGFref/samplingRate <<
//				 " MSamples/s " << endl;
	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to SGFref/samplingRate MSamples/s " << endl;
	myStatus=irio_setSGUpdateRate(&p_DrvPvt,0,SGFref/samplingRate,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGUpdateRate function] Trying to read SGUpdateRate0..." << endl;
	myStatus=irio_getSGUpdateRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 15: Set Signal Generator frequency" << endl << endl;
	uint32_t freqDesired = 100000;
	int accIncr=freqDesired*(UINT_MAX/(10000000));
	cout << "[irio_setSGFreq function] SGFreq0 set to " << accIncr << ", meaning " << freqDesired << " Hz" << endl;
	myStatus=irio_setSGFreq(&p_DrvPvt,0,accIncr,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	 myStatus=irio_getSGFreq(&p_DrvPvt,0,&valueReadI32, &status);
	 cout << "[irio_getSGFreq function] Trying to read SGFreq0..." << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 16: Set Signal Generator amplitude" << endl << endl;
	int32_t amplitude = 4096;
	double CVDAC = 0.0;

	cout << "[irio_getSGCVDAC function] Trying to read SGCVDAC..." << endl << endl;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
	// TODO: Ver cómo poner esto para evitar core dumped
//	cout << "[irio_setSGAmp function] SGAmp0 set to " << amplitude << ", meaning " << amplitude/CVDAC << " V" << endl;
	cout << "[irio_setSGAmp function] SGAmp0 set to 0.517563 V" << endl;
	myStatus=irio_setSGAmp(&p_DrvPvt,0,amplitude,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGAmp function] Trying to read SGAmp0..." << endl;
	 myStatus=irio_getSGAmp(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 17: Set Signal Generator type" << endl;
	cout << "First is necessary to disable Analog output 0" << endl << endl;
	cout << "[irio_setAOEnable function] AO0 enable set to 0 (DISABLE) " << endl;
	myStatus=irio_setAOEnable(&p_DrvPvt,0,0,&status); // AO is disabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_setSGSignalType function] SGSignalType0 set to 1 (SINE) " << endl;
	usleep(100);
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGSGSignalType function] Trying to read SGSignalType0... " << endl;
	myStatus=irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100000);
	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 18: Get data from DMA" << endl << endl;
	cout << "Inside the following 60 samples must be approximately 50 samples "
				 "that represent one sine cycle with a maximum and minimum of +-" <<
				 amplitude/CVDAC << " raw_value +-4096" << endl;

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	elementsRead = 0;
	sampleCounter = 0;
	do {
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer,&elementsRead,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else {
			if(elementsRead==1) {
				int numOfSamplesToShow=60; // 50 Samples must represent 1 sine cycle. 10e more are required to check that entire cycle is acquired
				short int* auxDataBuffer = new short int [4096*8];
				auxDataBuffer=(short int *)dataBuffer;
				sampleCounter++;
				double CVADC = 0.0;
				myStatus=irio_getSGCVADC(&p_DrvPvt,&CVADC,&status);
				for (int i=0;i<numOfSamplesToShow;i++) //only one part of the block is displayed to simplify the output
					//one period has 100 samples
					cout << "Sample["<< std::setw(2) << std::setfill('0') << i
					     << "]=" << auxDataBuffer[(i*DMATtoHOSTNCh)+2]*CVADC << endl;
			}
			else
				usleep(((1/(double)samplingRate)*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds
		}
		EXPECT_NE(myStatus, IRIO_success);
	} while ((myStatus ? 0 : 1) && sampleCounter < 1);

	delete [] dataBuffer;

	cout << endl << "TEST 19: Closing DMAS" << endl << endl;
	cout << "[irio_closeDMAsTtoHost function] No output if DMAs have been closed successfully" << endl;
	myStatus = irio_closeDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 20: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

// Test using FlexRIO mod5761-perf bitfile
TEST (TP_errorTests, failInitDriver) {
	string testName = "TP_errorTests: Test of bitfile FlexRIO_perf "
			          "when there is a failure on driver initialization";
	string testDescription = "Test verifies several warnings and errors if "
            "user tries to configure the FPGA when there is a failure on driver initialization";
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
	EXPECT_NE(myStatus, IRIO_success);
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

TEST(TP_errorTests, failInitDriver6581) {
	string testName = "TP_errorTests: Configuring FPGA when there is a failure on driver initialization";
	string testDescription = "Test verifies several warnings and errors if "
                                  "user tries to configure the FPGA when there is a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7965";
	string RIOSerial = "0x01666C59";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "wrongPath";
	string bitfileName = "FlexRIOMod6581_"+RIODevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization, expected failure" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod6581",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongPath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	// Expect warnings or errors so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Testing auxiliary digital output port 6. "
				    "It allows to write on whole digital output port 2" << endl << endl;

	int32_t valueReadI32;
	cout << "[irio_setAuxDO function] write '1' value into auxDO6" << endl;
	myStatus=irio_setAuxDO(&p_DrvPvt,6,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAuxDO function] Trying to read AuxDO6 value..." << endl << endl;
	myStatus = irio_getAuxDO(&p_DrvPvt,6,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(10);
	cout << endl << "TEST 3: This test write 0 and 1, into a random digital output, "
			  	    "and it is read from DO register. Warnings or errors expected" << endl;

	cout << "[irio_setDO function] Write 0 value in implemented port DO5" << endl;
	myStatus=irio_setDO(&p_DrvPvt,5,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDO function] Trying to read DO5" << endl;
	myStatus=irio_getDO(&p_DrvPvt,5,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(20);
	cout << "[irio_getDI function] Trying to read DI5" << endl << endl;
	myStatus = irio_getDI(&p_DrvPvt,5,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "Trying to read FPGA temperature..." << endl;
	int FPGATemp = -1;

	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

// Set of tests using mod1483 bitfile
TEST(TP_errorTests, failInitDriverUART) {
	string testName = "TP_errorTests: Test of bitfile FlexRIOMod1483 when failure";
	string testDescription = "Test tries to check the UART available in the CameraLink interface "
			                 "after a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7966";
	string RIOSerial = "0x01A34CC7";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod1483",
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
	// Expect warnings or error so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

	// Core dumped when configure after failure initializing the driver
	// Don't make sente to use this function on this test
	int32_t value;
//	cout << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
//	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
//	myStatus=irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing DMA's configuration" << endl << endl;

	cout << "[irio_getUARTBaudRate] Trying to read UART baud rate..." << endl;
	myStatus = irio_getUARTBaudRate(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getUARTBreakIndicator] Trying to read UART break indicator..." << endl;
	myStatus = irio_getUARTBreakIndicator(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getUARTFrammingError] Trying to read UART framming error..." << endl;
	myStatus = irio_getUARTFrammingError(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getUARTOverrunError] Trying to read UART overrun error..." << endl;
	myStatus = irio_getUARTOverrunError(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Setting up DMAs to host" << endl << endl;
	myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Starting FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 4: Setting FPGA UART baud rate" << endl << endl;
	cout << "[irio_setUARTBaudRate function] Setting FPGA UART BaudRate to 9600" << endl;
	myStatus = irio_setUARTBaudRate(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	// Core dumped when sending data after failure initializing the driver
	// Don't make sente to use this function on this test
//	cout << endl << "TEST 5: Sending data to CameraLink" << endl << endl;
//	cout << "[irio_sendCLuart function]" << endl;
//	cout << "Introduce 4 characters and pulse intro, to send data to the camera" << endl;
//
//	string message;
//	getline(cin, message);
//
//	cout << endl << "Sending UART Message: " << message << endl;
//
//	myStatus=irio_sendCLuart(&p_DrvPvt,message.c_str(), (int) message.size(),&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	// Core dumped when receiving data after failure initializing the driver
	// Don't make sente to use this function on this test
//	cout << endl << "TEST 6: Receiving data from CameraLink" << endl << endl;
//	int len = 0;
//	// TODO: Ver cuánta memoria almaceno en esta variable
//	char* msg = (char*) malloc(5*sizeof(char));
//	cout << "Receiving UART message" << endl;
//	myStatus=irio_getCLuart(&p_DrvPvt,msg,&len,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_EQ(myStatus, IRIO_success);
//
//	message.assign(msg,strlen(msg));
//
//	if (len>0)
//		cout << "MSG received: " << message << endl;
//	else
//		cout << "MSG not received" << endl;

	/**
	 * TEST 7
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 7: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_FlexRIO_mod1483, failInitDriverIMAQ) {
	string testName = "TP_FlexRIO_mod1483: Test of bitfile FlexRIOMod1483 when failure";
	string testDescription = "Test tries to check image acquisition from a CameraLink "
			                 " compatible device after a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7966";
	string RIOSerial = "0x01A34CC7";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod1483",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongPath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	// Expect warnings or error so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

    // Core dumped when configure after failure initializing the driver
	// Don't make sente to use this function on this test
//	cout << endl << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
//	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
//	myStatus=irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Starting FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Acquiring image" << endl << endl;
	int i=0;
	uint16_t fc=0;
	uint16_t *fc2 = nullptr;
	int firstImage=1;
	int count=0;
	int imageSize = 256*256; // Dimension X x Dimension Y
	// Allocate Memory for image
	uint64_t* dataBuffer = new uint64_t[imageSize/8];

	//Start Acquisition
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "Trying to acquire 2 frames. The counter embedded in "
			"the frame will be recovered and printed. " << endl;
	while(i<2){
		myStatus = irio_getDMATtoHostImage(&p_DrvPvt, imageSize, 0, dataBuffer, &count, &status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_NE(myStatus, IRIO_success);
		if(myStatus==IRIO_success){
			if(count==imageSize){
				fc2=(uint16_t*)dataBuffer;
				if(firstImage){
					firstImage=0;
				}else if((fc+1)%imageSize!=fc2[0]){
					irio_mergeStatus(&status,Generic_Error,verbosity,
									 "\nFrameCounter Error at Image fc[i]=%d, fc[i-1]=%d, img: %d\n",fc2[0],fc, i);
					break;
				}
				fc=fc2[0];
				cout << "Frame acquired [" << std::setw(3) << std::setfill('0') << i << "], "
						"counter value obtained from image: " << fc << endl;
				i++;
			}else{
				usleep(1000);
			}
		}
		else {
			cout << "The frame " << i << " has not been acquired. Something has fail" << endl;
			i++;
		}
	}

	delete [] dataBuffer;

	/**
	 * TEST 4
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 3: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

// Test using cRIO PBP profile bitfile
TEST(TP_errorTests, failInitDriverCRIOPBP) {
	string testName = "TP_cRIO_PBP: Test of bitfile cRIO_PBP when failure";
	string testDescription = "Test verifies driver’s ability to check error messages "
							 "after failure on driver's initialization";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "9159";
	string RIOSerial = "0x01C10FAC";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.0";
	string NIRIOmodel = "NI 9159";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "cRIO_PBP";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
	myStatus = irio_initDriver("failInitDriverPBPTest",
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
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	cout << endl << "TEST 2: Testing FPGA VI Version" << endl << endl;
	char* VIVersion = new char[strlen(FPGAversion.c_str())];
	unsigned long int valueLength;
	cout << "[irio_getFPGAVIVersion function] Trying to read FPGA VI version..." << endl;
	myStatus = irio_getFPGAVIVersion(&p_DrvPvt,VIVersion,4,&valueLength,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getFPGAVIVersion function] Trying to read FPGA VI version..." <<  VIVersion<< endl;

	cout << endl << "TEST 3: Testing device quality status" << endl << endl;
	cout << "[irio_getDevQualityStatus function] Trying to read device quality status..." << endl;
	myStatus = irio_getDevQualityStatus(&p_DrvPvt,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 4: Testing FPGA temperature" << endl << endl;
	int FPGATemp=0;
	cout << "[irio_getDevTemp function] Trying to read FPGA temperature..." << endl;
	myStatus = irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 5: Testing device profile" << endl << endl;
	cout << "[irio_getDevProfile function] Trying to read device profile..." << endl;
	myStatus = irio_getDevProfile(&p_DrvPvt,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 6: Testing PBP sampling rate configuration" << endl << endl;
	int32_t Fref;
	cout << "[irio_getFref function] Trying to read Fref..." << endl;
	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_NE(myStatus, IRIO_success);
	if (Fref != 0) {
		cout << "FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
	}
	aivalue = 500; // 500 Samples/s
	cout << "[irio_setSamplingRate function] Sampling rate wrote: " << aivalue << endl;
	myStatus = irio_setSamplingRate(&p_DrvPvt,0,aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSamplingRate function] Trying to read sampling rate..." << endl;
	myStatus = irio_getSamplingRate(&p_DrvPvt,0,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 7: Testing Analog Inputs" << endl << endl;
	cout << "Analog I/O ports are not connected so they are independent. Value 100 is going to be write "
			"on analog output AO0 and other different value is going to be read on analog input AI0" << endl;
	cout << "[irio_setAO function] AO0 set to 2048 (digital value)" << endl;
	aivalue = 2048;
	int channel = 0;
	myStatus = irio_setAO(&p_DrvPvt,channel,aivalue,&status); // Set AO channel 0 terminal to 2048 digital value
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAO function] Trying to read AO0..." << endl;
	myStatus = irio_getAO(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAI function] Trying to read AI0..."<< endl;
	myStatus = irio_getAI(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 8: Testing Signal Generator phase" << endl << endl;
	aivalue = 90;
	cout << "[irio_setSGPhase function] Signal Generator phase wrote: " << aivalue << endl;
	myStatus = irio_setSGPhase(&p_DrvPvt,channel,aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSamplingRate function] Trying to read sampling rate..." << endl;
	myStatus = irio_getSGPhase(&p_DrvPvt,channel,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 9: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}
