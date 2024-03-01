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
 * - cRIO.StartFPGA
*/
TEST(cRIO, InitCloseDriver) {
    irioDrv_t drv;
    initCRIODriver("cRIO_PBP", &drv);
    closeDriver(&drv);
}
TEST(cRIO, StartFPGA) {
    irioDrv_t drv;
    initCRIODriver("cRIO_PBP", &drv);
    startFPGA(&drv);
    closeDriver(&drv);
}