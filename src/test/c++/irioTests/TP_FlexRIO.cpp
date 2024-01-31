#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

// IRIO Library
extern "C" {
#include <irioDataTypes.h>
#include <irioDriver.h>
#include <irioError.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDMA.h>
#include <irioHandlerDigital.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioResourceFinder.h>
#include <limits.h>
#include <math.h>
}

#include "testUtils-IRIO.h"

using namespace TestUtilsIRIO;
using std::cout; 
using std::endl;

// Test init and close functions
TEST(FlexRIO, InitClose) {
    irioDrv_t drv;
    initDriver("FlexRIOnoModule_", &drv);
    closeDriver(&drv);
}

/**
 * TP-IRL-3000 Checking for FlexRIO basic initialization and close functions.
 * This test checks the IRIO library functions: irio_initDriver and
 * irio_closeDriver in the FlexRIO platform. The objective is to check that
 * all terminals are identified correctly by the irio_initDriver. This test uses
 * a set of multiple bitfiles containing multiple resources. Besides, the test
 * try to show that the IRIO library detects inconsistences in the FPGA
 * configuration.
 *
 * Implemented in tests:
 * - ResourcesCPUDAQ
 * - ResourcesCPUIMAQ
 * - ResourcesMAXIO
 * - ResourcesMissing
 */
TEST(FlexRIO, ResourcesCPUDAQ) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    initDriver(std::string("FlexRIO_CPUDAQ_"), &drv);

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources:
    // 2 DMAs
    if (verbose_test) cout << "[TEST] Found " << res.DMA << " DMAs. Expected 2" << endl;
    EXPECT_EQ(res.DMA, 2);
    // 2 AI
	if (verbose_test) cout << "[TEST] Found " << res.AI << " AIs. Expected 2" << endl;
	EXPECT_EQ(res.AI, 2);
    // 2 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 2" << endl;
	EXPECT_EQ(res.AO, 2);
    // 2 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(res.auxAI, 2);
    // 2 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(res.auxAO, 2);
    // 2 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 2" << endl;
	EXPECT_EQ(res.DI, 2);
    // 2 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 2" << endl;
	EXPECT_EQ(res.DO, 2);
    // 2 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(res.auxDI, 2);
    // 2 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(res.auxDO, 2);
    // 2 SG
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 2" << endl;
	EXPECT_EQ(res.SG, 2);

    closeDriver(&drv);
}
TEST(FlexRIO, ResourcesIMAQ) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    initDriver(std::string("FlexRIO_CPUIMAQ_"), &drv);

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources
	// 1 IMAQ DMA
	if (verbose_test) cout << "[TEST] Found " << res.DMA << " IMAQ DMAs. Expected 1" << endl;
	EXPECT_EQ(res.DMA, 1);
	// 1 CLConfig
	if (verbose_test) cout << "[TEST] CLConfig " << (res.CLConfig ? "found" : "not found") << endl;
	EXPECT_EQ(res.CLConfig, 1);
	// 1 CLUART
	if (verbose_test) cout << "[TEST] CLUART " << (res.CLUART ? "found" : "not found") << endl;
	EXPECT_EQ(res.CLUART, 1);
    // 2 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(res.auxAI, 2);
    // 2 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(res.auxAO, 2);
    // 2 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 2" << endl;
	EXPECT_EQ(res.DI, 2);
    // 2 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 2" << endl;
	EXPECT_EQ(res.DO, 2);
    // 2 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(res.auxDI, 2);
    // 2 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(res.auxDO, 2);

    closeDriver(&drv);
}
TEST(FlexRIO, ResourcesMAXIO) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    initDriver(std::string("FlexRIOonlyResources_"), &drv);

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources:
    // 1 DMAs
    if (verbose_test) cout << "[TEST] Found " << res.DMA << " DMAs. Expected 1" << endl;
    EXPECT_EQ(res.DMA , 1);
    // 2 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 2" << endl;
	EXPECT_EQ(res.AO, 2);
    // 16 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 16" << endl;
	EXPECT_EQ(res.auxAI, 16);
    // 16 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 16" << endl;
	EXPECT_EQ(res.auxAO, 16);
    // 16 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 16" << endl;
	EXPECT_EQ(res.auxDI, 16);
    // 16 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 16" << endl;
	EXPECT_EQ(res.auxDO, 16);
    // 54 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 54" << endl;
	EXPECT_EQ(res.DI, 54);
    // 54 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 54" << endl;
	EXPECT_EQ(res.DO, 54);
    // 2 SG
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 2" << endl;
	EXPECT_EQ(res.SG, 2);

    closeDriver(&drv);
}
TEST(FlexRIO, ResourcesMissing) {
	irioDrv_t drv;

	// Manual initialization to load a failResources file and test for error instead of success
    int st = IRIO_success;

    int verbose_init = std::stoi(TestUtilsIRIO::getEnvVar("VerboseInit"));
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
    string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

    string NIRIOmodel = "PXIe-" + RIODevice + "R";
    string bitfileName = "FlexRIOonlyResources_" + RIODevice;
    string filePath = "../resources/failResources/" + RIODevice + "/";
    string testName = ("Test_" + bitfileName);

    TStatus status;
    irio_initStatus(&status);

    if (verbose_test) cout << "[TEST] Initializing driver with bitfile \"failResources/" << bitfileName << "\"" << endl;
    st = irio_initDriver(testName.c_str(), RIOSerial.c_str(),
                         NIRIOmodel.c_str(), bitfileName.c_str(), "V1.2",
                         verbose_init, filePath.c_str(), filePath.c_str(), &drv,
                         &status);
    if (verbose_test) cout << "[TEST] Driver initialized " << ((st == IRIO_success) ? "successfully (Error)" : "unsuccessfully (Expected)") << endl;
    EXPECT_NE(st, IRIO_success);

	// Test for errors: Load header file again to avoid Segmentation Fault
	TResourcePort tmpPort;
	if (verbose_test) cout << "[TEST] Loading header file into driver" << endl;
	st = loadHeaderFile(&drv, filePath, &status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) {
		if (st == IRIO_success) {
			cout << "[TEST] Header file loaded successfully" << endl;
		} else {
			cout << "[TEST] Header file loaded unsuccessfully" << endl;
			logErrors(st, status);
		}
	}
	TStatus local_status; 
	irio_initStatus(&local_status);
	// Error finding AO1Enable
	int AO1Found  = irio_findResourceEnum(&drv, "_ControlI32_AO", 0, &tmpPort,&local_status, 0);
	int AO1Enable = irio_findResourceEnum(&drv, "_ControlBool_AOEnable", 0, &tmpPort, &local_status, 0);
	if (verbose_test) cout << "[TEST] AO1Found = " << (AO1Found ? "Error" : "Success") << endl << "[TEST] AO1Enable = " << (AO1Enable ? "Error" : "Success") << endl;
	EXPECT_EQ(AO1Found, IRIO_success); 
	EXPECT_NE(AO1Found, AO1Enable);
	irio_resetStatus(&local_status);

	// Error finding SGFref0 and 
	irio_findResourceEnum(&drv,"_IndicatorU32_SGFref",0,&tmpPort,&local_status,0);
	if (verbose_test) cout << "[TEST] SGFref0 status code = " << (local_status.code ? "Error" : "Success") << endl;
	EXPECT_NE(local_status.code, IRIO_success);
	irio_resetStatus(&local_status);

	// Error finding SGSignalType1
	irio_findResourceEnum(&drv,"_ControlU8_SGSignalType",1,&tmpPort,&local_status,0);
	if (verbose_test) cout << "[TEST] SGSignalType status code = " << (local_status.code ? "Error" : "Success") << endl;
	EXPECT_NE(local_status.code, IRIO_success);
	irio_resetStatus(&local_status);

	// Error finding DMATtoHOSTSamplingRate0
	irio_findResourceEnum(&drv, "_ControlU16_DMATtoHOSTSamplingRate", 0, &tmpPort, &local_status, 0);
	if (verbose_test) cout << "[TEST] DMAtoHOSTSamplingRate0 status code = " << (local_status.code ? "Error" : "Success") << endl;
	EXPECT_NE(local_status.code, IRIO_success);
	irio_resetStatus(&local_status);

	freeHeaderFile(&drv);

	// Test for resources
	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources:
    // 1 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 1" << endl;
	EXPECT_EQ(res.AO, 1);
    // 16 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 16" << endl;
	EXPECT_EQ(res.auxAI, 16);
    // 16 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 16" << endl;
	EXPECT_EQ(res.auxAO, 16);
    // 16 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 16" << endl;
	EXPECT_EQ(res.auxDI, 16);
    // 16 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 16" << endl;
	EXPECT_EQ(res.auxDO, 16);
    // 54 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 54" << endl;
	EXPECT_EQ(res.DI, 54);
    // 54 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 54" << endl;
	EXPECT_EQ(res.DO, 54);
    // 0 SG 
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 0" << endl;
	EXPECT_EQ(res.SG, 2);
    // 0 DMAs 
    if (verbose_test) cout << "[TEST] Found " << res.DMA << " DMAs. Expected 0" << endl;
    EXPECT_EQ(res.DMA , 0);

    closeDriver(&drv);
}