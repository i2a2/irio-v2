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
using std::string; using std::cerr;

/**
 * Test verifies driver’s ability to check the correct communication and interconnection
 * between auxiliar I/O analog/digital FPGA registers.
 * This test is related to the following requirements:
 *
 * PXIE7965R
 * PXIE7966R
 *
 * This test is executed only with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7965, 7966
 */

TEST(TP_noModule, functional) {
	string testName = "TP_noModule: Functional test of bitfile noModule";
	string testDescription = "Test verifies driver’s ability to check the "
			"correct communication and interconnection between auxiliary I/O analog/digital FPGA registers";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("functionalNoModuleTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	// In TP_onlyResources test all parameters of irio_initDriver has been tested, so
	// in this test they are suppose that are not going to be incorrect.
	// Critical failure and closing driver if something fail

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly. Closing driver..." << endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	usleep(100);

	cout << endl << "TEST 2: Testing auxiliary 32 bits analog I/O ports" << endl << endl;
	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
	// but only six auxiliary analog output ports instantiated
	for(int i=0;i<6;i++){
		int analogValue = 0;
		cout << "[irio_setAuxAO function] Value " << analogValue << " is set in auxAO" << i << endl;
		myStatus=irio_setAuxAO(&p_DrvPvt,i,analogValue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAO function] Value read from auxAO" << i << "is: " << aivalue << endl;

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAI function] Value read from auxAI" << i << "is: " << aivalue << endl << endl;

		EXPECT_NE(myStatus, IRIO_error);

		if(aivalue!=analogValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 0, read value: %d [ERROR]\n",i,aivalue);

		analogValue = 100;
		cout << "[irio_setAuxAO function] Value " << analogValue << " is set in auxAO" << i << endl;
		myStatus=irio_setAuxAO(&p_DrvPvt,i,analogValue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAO function] Value read from auxAO" << i << "is: " << aivalue << endl;

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxAI function] Value read from auxAI" << i << "is: " << aivalue << endl << endl;

		EXPECT_NE(myStatus, IRIO_error);

		if(aivalue!=analogValue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 1,read value: %d [ERROR]\n",i,aivalue);
	}


// TODO: Commented until noModule bitfile is designed with auxiliary 64 bits analog I/O ports
//
//	cout << endl << "TEST ***: Testing auxiliary 64 bits analog I/O ports" << endl << endl;
//	int64_t ai64value = 0;
//	// It is known prior to the execution of the test that there are ten auxiliary analog input ports
//	// but only six auxiliary analog output ports instantiated
//	for(int i=0;i<6;i++){
//		int analogValue = 0;
//		cout << "[irio_setAuxAO_64 function] value " << analogValue << " is set in aux64AO" << i << endl;
//		myStatus=irio_setAuxAO_64(&p_DrvPvt,i,analogValue,&status);
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//
//		EXPECT_NE(myStatus, IRIO_error);
//
//		myStatus=irio_getAuxAO_64(&p_DrvPvt,i,&ai64value,&status);
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//		else
//			cout << "[irio_getAuxAO_64 function] value read from aux64AO" << i << "is: " << aivalue << endl;
//
//		EXPECT_NE(myStatus, IRIO_error);
//
//		myStatus=irio_getAuxAI_64(&p_DrvPvt,i,&ai64value,&status);
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//		else
//			cout << "[irio_getAuxAI_64 function] value read from aux64AI" << i << "is: " << ai64value << endl << endl;
//
//		EXPECT_NE(myStatus, IRIO_error);
//
//		if(ai64value!=analogValue)
//			irio_mergeStatus(&status,Generic_Error,verbosity,"Aux64AI%d expected value 0, read value: %d [ERROR]\n",i,ai64value);
//
//		analogValue = 100;
//		cout << "[irio_setAuxAO_64 function] value " << analogValue << " is set in aux64AO" << i << endl;
//		myStatus=irio_setAuxAO_64(&p_DrvPvt,i,analogValue,&status);
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//
//		EXPECT_NE(myStatus, IRIO_error);
//
//		myStatus=irio_getAuxAO_64(&p_DrvPvt,i,&ai64value,&status);
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//		else
//			cout << "[irio_getAuxAO_64 function] value read from aux64AO" << i << "is: " << ai64value << endl;
//
//		EXPECT_NE(myStatus, IRIO_error);
//
//		myStatus=irio_getAuxAI_64(&p_DrvPvt,i,&ai64value,&status);
//		if (myStatus > IRIO_success) {
//			TestUtilsIRIO::getErrors(status);
//		}
//		else
//			cout << "[irio_getAuxAI_64 function] value read from aux64AI" << i << "is: " << ai64value << endl << endl;
//
//		EXPECT_NE(myStatus, IRIO_error);
//
//		if(aivalue!=analogValue)
//			irio_mergeStatus(&status,Generic_Error,verbosity,"Aux64AI%d expected value 1,read value: %d [ERROR]\n",i,ai64value);
//	}


	cout << endl << "TEST 3: Testing auxiliary digital I/O ports" << endl << endl;
	aivalue = 0;
	// It is known prior to the execution of the test that there are only six auxiliary digital
	//input and six auxiliary digital output ports instantiated
	for(int i=0;i<6;i++) {
		int digitalvalue = 0;

		cout << "[irio_setAuxDO function] Value " << digitalvalue << " is set in auxDO" << i << endl;
		myStatus=irio_setAuxDO(&p_DrvPvt,i,digitalvalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDO function] Value read from auxDO" << i << "is: " << aivalue << endl;

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDI function] Value read from auxDI" << i << "is: " << aivalue << endl << endl;

		EXPECT_NE(myStatus, IRIO_error);

		if(aivalue!=digitalvalue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 0, value read:%d [ERROR]\n",i,aivalue);

		digitalvalue = 1;

		cout << "[irio_setAuxDO function] Value " << digitalvalue << " is set in auxDO" << i << endl;
		myStatus=irio_setAuxDO(&p_DrvPvt,i,digitalvalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDO function] Value read from auxDO" << i << "is: " << aivalue << endl;

		EXPECT_NE(myStatus, IRIO_error);

		myStatus=irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else
			cout << "[irio_getAuxDI function] Value read from auxDI" << i << "is: " << aivalue << endl << endl;

		EXPECT_NE(myStatus, IRIO_error);

		if(aivalue!=digitalvalue)
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 1, value read:%d [ERROR]\n",i,aivalue);
	}

	cout << endl << "TEST 4: Reading FPGA temperature" << endl << endl;
	int FPGATemp=0;
	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] Temperature value read from FPGA: "
	     << std::setprecision(4) << (float) FPGATemp*0.25 << "ºC" << endl << endl;
	EXPECT_NE(myStatus, IRIO_error);

	cout << endl << "TEST 5: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}


// Tests to catch errors

TEST(TP_noModule, setFPGATwice) {
	string testName = "TP_noModule: Set FPGA twice test";
	string testDescription = "Test verifies driver’s ability to check that the "
			                 "FPGA is started twice in a row";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("setFPGATwiceTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly. Closing driver..." << endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing FPGA start mode twice" << endl << endl;
	cout << "First start of the FPGA, no error expected" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	int aivalue = 0;

	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl << endl;

	cout << "Second start of the FPGA, error/warning expected" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_noModule, setFPGAInitErrorSessionClose) {
	string testName = "TP_noModule: Start FPGA after a failure on the driver's initialization. Condition: there has to be the driver's session close";
	string testDescription = "Test verifies driver’s ability to *************************************";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User don't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("setFPGAInitErrorSessionCloseTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongFilePath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int aivalue=0;

	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl << endl;
	if (aivalue == 0)
		cout << "IRIO can not initialize the FPGA if there is not a driver's session open" << endl;

	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_noModule, setFPGAInitErrorSessionOpen) {
	string testName = "TP_noModule: Start FPGA after a failure on the driver's initialization. Condition: there has to be the driver's session open";
	string testDescription = "Test verifies driver’s ability to **********************************";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;

	// A wrong version passed as parameter to keep driver's session open
	// despite of a failure initializing the driver
	myStatus = irio_initDriver("setFPGAInitErrorSessionOpenTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   "wrongVersion",
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	// TODO: IRIO should close the session after failure initializing the driver
	//       because of wrong FPGA version but it does not do.
	EXPECT_EQ(myStatus, IRIO_success);

	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl << endl;
	if (aivalue == 1)
		cout << "IRIO can initialize the FPGA if there is a driver's session open "
				"despite off the driver initialization failure. "
				"User should close IRIO driver." << endl;

	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_noModule, manageNonImplementedPorts) {
	string testName = "TP_noModule: writing on non implemented auxiliary analog/digital I/O ports.";
	string testDescription = "Test verifies driver’s ability to check the error if user tries to write on "
			"a port that is not implemented in the FPGA";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7965" || RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOnoModule_"+RIODevice;

	int aivalue=0;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("writingNonImplementedPortTest",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly. Closing driver..." << endl;
		myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	cout << "FPGA hardware logic is started (\"Running\") Value " << 1 << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "FPGA State is: " << aivalue << ". 1-->\"running\", 0-->\"stopped\"" << endl;

	usleep(100);
	int analogValue = 0;

	cout << endl << "TEST 2: Testing an auxiliary analog output port not "
			        "implemented in the FPGA setting a value" << endl << endl;

	cout << "[irio_setAuxAO function] Value " << analogValue << " is set in not "
			"implemented auxAO6. Error/warning expected" << endl;
	myStatus=irio_setAuxAO(&p_DrvPvt,6,analogValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Testing an auxiliary analog output port not "
		 	        "implemented in the FPGA getting its value" << endl;

	cout << "[irio_getAuxAO function] Trying to read auxAO6..." << endl;
	myStatus=irio_getAuxAO(&p_DrvPvt,6,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 4: Testing an auxiliary analog input port not "
			        "implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxAI function] Trying to read auxAI10..." << endl;
	myStatus=irio_getAuxAI(&p_DrvPvt,10,&aivalue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

//	TODO: Commented until noModule bitfile is designed with auxiliary 64 bits analog I/O ports
//	usleep(100);
//	cout << endl << "TEST ***: Testing an auxiliary 64 bits analog output port not "
//					"implemented in the FPGA setting a value" << endl << endl;
//
//	int analogValue = 0;
//	cout << "[irio_setAuxAO_64 function] Value " << analogValue << " is set in not "
//			"implemented aux64AO6. Error/warning expected" << endl;
//	myStatus=irio_setAuxAO_64(&p_DrvPvt,6,analogValue,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//
//	EXPECT_NE(myStatus, IRIO_success);
//
//	cout << endl << "TEST ***: Testing an auxiliary 64 bits analog output port not "
//					"implemented in the FPGA getting its value" << endl;
//
//	cout << "[irio_getAuxAO_64 function] Trying to read aux64AO6..." << endl;
//	myStatus=irio_getAuxAO_64(&p_DrvPvt,6,&aivalue,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);
//
//	cout << endl << "TEST ***: Testing an auxiliary 64 bits analog input port not "
//					"implemented in the FPGA getting its value" << endl << endl;
//
//	cout << "[irio_getAuxAI_64 function] Trying to read aux64AI10..." << endl;
//	myStatus=irio_getAuxAI_64(&p_DrvPvt,10,&aivalue,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	int digitalValue = 0;

	cout << endl << "TEST 5: Testing an auxiliary digital output port not "
			        "implemented in the FPGA setting a value" << endl << endl;

	cout << "[irio_setAuxDO function] Value " << digitalValue << " is set in not "
			"implemented auxDO6. Error/warning expected" << endl;
	myStatus=irio_setAuxDO(&p_DrvPvt,6,digitalValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 6: Testing an auxiliary digital output port not "
		 	        "implemented in the FPGA getting its value" << endl;

	cout << "[irio_getAuxDO function] Trying to read auxDO6..." << endl;
	myStatus=irio_getAuxDO(&p_DrvPvt,6,&digitalValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 7: Testing an auxiliary digital input port not "
			        "implemented in the FPGA getting its value" << endl << endl;

	cout << "[irio_getAuxDI function] Trying to read auxDI6..." << endl;
	myStatus=irio_getAuxDI(&p_DrvPvt,6,&digitalValue,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 8: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

