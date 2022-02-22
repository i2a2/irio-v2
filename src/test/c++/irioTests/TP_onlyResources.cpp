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

// Always max verbosity
int verbosity = 1;

using namespace std;

// TODO: Plantear cómo mostrarle al usuario, si es que hay que mostrárselo,
//       los recursos que hay implementados en cada FPGA en función del bitfile
//       Comentarlo con Víctor
//       Opcion 1: printf tal cual como en IRIO que imprime en pantalla
//       Opcion 2: tabla de hash e imprimir en pantalla unos recursos u otros en función del bitfile y del dispositivo RIO a testear
//       Opcion 3: Volcar en un fichero de log los recursos en lugar de por pantalla y que el usuario lo lea si quiere o necesita


/**
 * Test verifies driver’s ability to read and testing resources in the FPGA.
 * This test is related to the following requirements:
 *
 * PXIE7966R
 * PXIE7965R
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio.py command
 * execute export rioDevice=0x..........
 * execute export rioSerial=xxxx, donde xxxx = 7961, 7965, 7966...
 */

//TODO: Implementar nuevos GoogleTests para CPUIMAQ, MAXIO, onlyResources
//      una vez el test CPUDAQ sea robusto

TEST(TP_onlyResources, CPUDAQ)
{
	const testing::TestInfo* const test_info =
	      testing::UnitTest::GetInstance()->current_test_info();

	std::string testName(test_info->name());
	std::string testDescription = "Test verifies driver’s ability to read and testing resources in the FPGA.";

	// User don't have to know what FPGA Version is used
	std::string FPGAversion = "V1.0";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	std::string rioDevice = TestUtilsIRIO::getEnvVar("rioDevice");
	std::string rioSerial = TestUtilsIRIO::getEnvVar("rioSerial");

	std::string NIriomodel = "PXIe-"+rioDevice+"R";
	// TODO: Mejorar path, no puede ir hardcodeado
	std::string filePath = "../../../main/c/examples/resourceTest/"+rioDevice+"/";
	std::string bitfileName = "FlexRIO_"+testName+"_"+rioDevice;

	int myStatus;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	myStatus = irio_initDriver(const_cast<char*>(testName.c_str()),
							   const_cast<char*>(rioSerial.c_str()),
							   const_cast<char*>(NIriomodel.c_str()),
							   const_cast<char*>(bitfileName.c_str()),
							   const_cast<char*>(FPGAversion.c_str()),
							   verbosity,
							   const_cast<char*>(filePath.c_str()),
							   const_cast<char*>(filePath.c_str()),
							   &p_DrvPvt,
							   &status);

	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
}
