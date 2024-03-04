#include <gtest/gtest.h>

#include <cstdlib>
#include <string>
#include <random>
#include <chrono>

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

/**
 * TP-IRL-1000 Driver Initialization
 * 
 * When the test program is executed, the first thing to be checked is the Driver Initialization.
 * The version of the installed library will be tested also. irio_initDriver and irio_getVersion
 * functions are tested
 * 
 * Requisites:
 * - NI9264 Connected in Slot1
 * - NI9205 Connected in Slot2
 * - NI9401 Connected in Slot3
 * - NI9477 Connected in Slot4
 * - NI9426 Connected in Slot5
 * - NI9476 Connected in Slot6
 * - NI9425 Connected in Slot7
 * 
 * Implemented in:
 * - cRIO.InitCloseDriver
 * - cRIO.IOResources
 * - cRIO.StartFPGA
 * - cRIO.GetIRIOVersion
 * - cRIO.GetFPGAVIVersion
 * - cRIO.GetDevQuality
 * - cRIO.GetDevProfile
*/
TEST(cRIO, InitCloseDriver) {
    irioDrv_t drv;
    initCRIODriver("cRIOIO_9159", &drv);
    closeDriver(&drv);
}
TEST(cRIO, IOResources) {
    irioDrv_t drv;
    irioResources_t res;
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIOIO_9159", &drv);
    getResources(&drv, &res);

    // Expected resources:
    // 3 AI
	if (verbose_test) cout << "[TEST] Found " << res.AI << " AIs. Expected 3" << endl;
	EXPECT_EQ(res.AI, 3);
    // 3 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 3" << endl;
	EXPECT_EQ(res.AO, 3);
    // 3 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 3" << endl;
	EXPECT_EQ(res.DI, 3);
    // 3 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 3" << endl;
	EXPECT_EQ(res.DO, 3);
    // 2 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(res.auxAI, 2);
    // 2 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(res.auxAO, 2);
    // 2 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(res.auxDI, 2);
    // 2 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(res.auxDO, 2);
    // 0 SG
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 0" << endl;
	EXPECT_EQ(res.SG, 0);
    // 1 Sampling Rate
	if (verbose_test) cout << "[TEST] Found " << res.samplingRate << " sampling rates. Expected 1" << endl;
	EXPECT_EQ(res.samplingRate, 1);

    closeDriver(&drv);
}
TEST(cRIO, StartFPGA) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIOIO_9159", &drv);
    startFPGA(&drv);

    int32_t start = -1;
    if (verbose_test) cout << "[TEST] Getting FPGAStart" << endl;
    int st = irio_getFPGAStart(&drv, &start, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] FPGAStart read = " << start << endl;
    EXPECT_EQ(st, IRIO_success);
    EXPECT_EQ(start, 1);

    closeDriver(&drv);
}
TEST(cRIO, GetIRIOVersion) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIO_PBP", &drv);
    startFPGA(&drv);

    std::array<char, 7> version;
    int st = irio_getVersion(version.data(), &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] IRIO version = " << std::string(version.begin(), version.end()) << endl;
    EXPECT_EQ(st, IRIO_success);

    closeDriver(&drv);
}
TEST(cRIO, GetFPGAVIVersion) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIO_PBP", &drv);
    startFPGA(&drv);

    const int VIVersionLength = 4;
    std::array<char, VIVersionLength> version;
    uint64_t valueLength;
    int st = irio_getFPGAVIVersion(&drv, version.data(), VIVersionLength, &valueLength, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] VIVersionLength version = " << std::string(version.begin(), version.end()) << endl;
    EXPECT_EQ(st, IRIO_success);
    EXPECT_GT(valueLength, 0);

    closeDriver(&drv);
}
TEST(cRIO, GetDevQualityStatus) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIO_PBP", &drv);
    startFPGA(&drv);

    int quality = -1;
    if (verbose_test) cout << "[TEST] Reading DevQualityStatus" << endl;
    int st = irio_getDevQualityStatus(&drv, &quality, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] DevQualityStatus = " << quality << endl;
    EXPECT_EQ(st, IRIO_success);
    EXPECT_EQ(quality, 0);

    closeDriver(&drv);
}
TEST(cRIO, GetDevTemp) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIO_PBP", &drv);
    startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Reading temperature from device" << endl;
	int32_t reading = -1;
	int st = irio_getDevTemp(&drv, &reading, &status);
	float temp = 0.25f * reading;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) cout << "[TEST] Temperature = " << std::setprecision(4) << temp << "°C" << endl;

    closeDriver(&drv);
}
TEST(cRIO, GetDevProfile) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    initCRIODriver("cRIO_PBP", &drv);
    startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Reading device profile" << endl;
	int32_t reading = -1;
	int st = irio_getDevProfile(&drv, &reading, &status);
	logErrors(st, status);
	if (verbose_test) cout << "[TEST] Device profile = " << reading << "(0: DAQ DMA, 1: Point by Point)" << endl;
	EXPECT_EQ(st, IRIO_success);
    EXPECT_EQ(reading, 1);

    closeDriver(&drv);
}