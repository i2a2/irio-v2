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

/*
 * Test verifies driver’s ability to test cRIO DAQ (Data Acquisition) Profile.
 *
 * This test is related to the following requirements:
 *
 * NI9159 Chassis
 * 9264, 9205, 9401, 9477, 9426, 9476 and 9425 cRIO modules
 *
 * This test is executed only with 9205, 9401, 9477 and 9426 modules
 *
 */

TEST(TP_cRIO_DAQ, functional) {

}
