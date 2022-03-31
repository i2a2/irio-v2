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
	string testName = "TP_cRIO_DAQ: Functional test of bitfile cRIO_DAQ";
	string testDescription = "Test verifies driver’s ability to check cRIO DAQ profile.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// TODO: confParams se va a diseñar como un array
	//       Acceso a un valor del array en el bash: ${confParams[X]}, X = 0,1,2,...
	//       Acceso a todo el array en el bash: ${confParams[@]}
	string confParams = TestUtilsIRIO::getEnvVar("confParams");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="9159") << "Use the correct model of your cRIO chassis";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIO_CPUDAQ_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);



	// TODO: Usar ejemplo ya diseñado pero cambiando el bitfile, a ver qué hace

	// TODO: Cuando se tire del cable en el chasis cRIO habria que ver que hace
	//       la libreria de NI con el driver, especialmente con la sesion
	//       Mantiene la misma sesion, la cierra, la cierra y abre otra nueva...?












}
