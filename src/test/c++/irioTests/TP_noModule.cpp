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

/**
 * Test verifies driver’s ability to check the correct communication and interconnection
 * between auxiliar I/O analog/digital FPGA registers.
 * This test is related to the following requirements:
 *
 * PXIE7965R
 * PXIE7966R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

TEST(TP_onlyResources, functional) {

}
