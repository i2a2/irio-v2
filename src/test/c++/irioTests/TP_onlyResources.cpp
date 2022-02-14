#include <gtest/gtest.h>

#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <boost/version.hpp>
#include <boost/regex.hpp>

// IRIO Library
#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>


/**
 * Test verifies driver’s ability to read resources in the FPGA.
 * This test is related to the following requirements:
 *
 * PXIE7966R
 * PXIE7965R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export RIOserial=0x..........
 */


TEST(TP_NID_1000, CPUDAQ)
{

}

TEST(TP_NID_1000, CPUIMAQ)
{

}

TEST(TP_NID_1000, MAXIO)
{

}

TEST(TP_NID_1000, missingResources)
{

}

