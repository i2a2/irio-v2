#include <gtest/gtest.h>

#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <boost/version.hpp>
#include <boost/regex.hpp>

/**
 * Test verifies driver’s ability to read and testing resources in the FPGA.
 * This test is related to the following requirements:
 *
 * PXIE7966R
 * PXIE7965R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export RIOSerial="0x.........."
 */

TEST(TP_onlyResources, testCPUDAQ)
{

}

TEST(TP_onlyResources, testCPUIMAQ)
{

}

TEST(TP_onlyResources, testMAXIO)
{

}

TEST(TP_onlyResources, testmissingResources)
{

}



