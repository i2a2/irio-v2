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
 * InitCloseCPUDAQ
 */
TEST(FlexRIO, InitCloseCPUDAQ) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    initDriver(std::string("FlexRIO_CPUDAQ_"), &drv);

    // Expected resources:
    // 2 DMAs
    int dmas = (drv.DMATtoHOSTNo.found ? drv.DMATtoHOSTNo.value : 0) + (drv.DMATtoGPUNo.found ? drv.DMATtoGPUNo.value : 0);
    if (verbose_test) cout << "[TEST] Found " << dmas << " DMAs. Expected 2" << endl;
    EXPECT_EQ(dmas , 2);
    // 2 AI
	int AIs = getResourceCount(drv.enumAnalogInput, drv.max_analoginputs);
	if (verbose_test) cout << "[TEST] Found " << AIs << " AIs. Expected 2" << endl;
	EXPECT_EQ(AIs, 2);
    // 2 AO
	int AOs = getResourceCount(drv.enumAnalogOutput, drv.max_analogoutputs);
	if (verbose_test) cout << "[TEST] Found " << AOs << " AOs. Expected 2" << endl;
	EXPECT_EQ(AOs, 2);
    // 2 auxAI
	int auxAIs = getResourceCount(drv.enumauxAI, drv.max_auxanaloginputs);
	if (verbose_test) cout << "[TEST] Found " << auxAIs << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(auxAIs, 2);
    // 2 auxAO
	int auxAOs = getResourceCount(drv.enumauxAO, drv.max_auxanalogoutputs);
	if (verbose_test) cout << "[TEST] Found " << auxAOs << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(auxAOs, 2);
    // 2 DI
	int DIs = getResourceCount(drv.enumDigitalInput, drv.max_digitalsinputs);
	if (verbose_test) cout << "[TEST] Found " << DIs << " DIs. Expected 2" << endl;
	EXPECT_EQ(DIs, 2);
    // 2 DO
	int DOs = getResourceCount(drv.enumDigitalOutput, drv.max_digitalsoutputs);
	if (verbose_test) cout << "[TEST] Found " << DOs << " DOs. Expected 2" << endl;
	EXPECT_EQ(DOs, 2);
    // 2 auxDI
	int auxDIs = getResourceCount(drv.enumauxDI, drv.max_auxdigitalsinputs);
	if (verbose_test) cout << "[TEST] Found " << auxDIs << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(auxDIs, 2);
    // 2 auxDO
	int auxDOs = getResourceCount(drv.enumauxDO, drv.max_auxdigitalsoutputs);
	if (verbose_test) cout << "[TEST] Found " << auxDOs << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(auxDOs, 2);
    // 2 SG
	int SGs = drv.NoOfSG;
	if (verbose_test) cout << "[TEST] Found " << SGs << " SGs. Expected 2" << endl;
	EXPECT_EQ(SGs, 2);

    closeDriver(&drv);
}
