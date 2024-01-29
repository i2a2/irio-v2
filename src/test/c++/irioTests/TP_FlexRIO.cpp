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

	// Test for errors:
	// Error finding AO1Enable

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources:
    // 1 AO + TODO: Error finding AO1Enable : ResourceNotFoundWarning
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
    // 0 SG + TODO: Error finding SGFref0 and SGSignalType1
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 0" << endl;
	EXPECT_EQ(res.SG, 2);
    // 0 DMAs + TODO: Error finding DMATtoHOSTSamplingRate0
    if (verbose_test) cout << "[TEST] Found " << res.DMA << " DMAs. Expected 0" << endl;
    EXPECT_EQ(res.DMA , 0);

    closeDriver(&drv);
}