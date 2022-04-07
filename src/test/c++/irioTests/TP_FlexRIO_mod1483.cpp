#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

// IRIO Library
extern "C" {
#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>
}

#include "testUtils-IRIO.h"

// Always max verbosity
static int verbosity = 1;

using std::cout; using std::endl;
using std::string; using std::cerr;
using std::cin;

/**
 * Test verifies driver’s ability to acquire data image from a CameraLink compatible device
 * and UART availability in the CameraLink interface
 * This test is related to the following requirements:
 *
 * PXIE7966R. 7966R device can not be sustituted by any other model
 *
 * NI1483 adapter module
 *
 * This test must be executed with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=7966
 */

TEST(TP_FlexRIO_mod1483, functionalUART){
	string testName = "TP_FlexRIO_mod1483: Functional test of bitfile FlexRIOMod1483 focused on UART profile";
	string testDescription = "Test checks the UART available in the CameraLink interface";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("functionalMod1483UartTest",
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
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 1
	 * PRE FPGA START CONFIG
	 */
	int32_t value;
	cout << endl << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
	myStatus=irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getUARTBaudRate(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getUARTBaudRate function] Default baud rate: " << value << endl;

	myStatus=irio_getUARTBreakIndicator(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getUARTBreakIndicator function] UART Break indicator: " << value << endl;

	myStatus=irio_getUARTFrammingError(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getUARTFrammingError function] UART framing error: " << value << endl;

	myStatus=irio_getUARTOverrunError(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getUARTOverrunError function] UART overrun eror: " << value << endl;

	/*
	 * TEST 2
	 * Set up DMAs to host
	 */
	cout << endl << "TEST 2: Setting up DMAs to host" << endl;
	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 3
	 * FPGA START
	 */
	cout << endl << "TEST 3: Starting FPGA" << endl;
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	/*
	 * TEST 4
	 * SET BAUD RATE
	 */
	cout << endl << "TEST 4: Setting FPGA UART baud rate" << endl << endl;
	cout << "[irio_setUARTBaudRate function] Setting FPGA UART BaudRate to 9600" << endl;

	myStatus = irio_setUARTBaudRate(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 5
	 * SEND UART MESSAGE
	 */
	cout << endl << "TEST 5: Sending data to CameraLink" << endl << endl;
	cout << "[irio_sendCLuart function]" << endl;
	cout << "Introduce 4 characters and pulse intro, to send data to the camera" << endl;

	string message;
	getline(cin, message);

	cout << endl << "Sending UART Message: " << message << endl;

	myStatus=irio_sendCLuart(&p_DrvPvt,message.c_str(), (int) message.size(),&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	cout << "Please check the message displayed in the EDTpdv terminal "
				 "application. Press intro to continue" << endl;
	while(cin.get()!='\n');

	cout << "Introduce 4 characters into the EDTpdv terminal application. "
				 "After writing the 4 characters, press intro in this terminal." << endl;

	getline(cin, message);

	/*
	 * TEST 6
	 * RECEIVE UART MESSAGE
	 */
	cout << endl << "TEST 6: Receiving data from CameraLink" << endl << endl;
	int len = 0;
	// TODO: Supuestamente debería permitir almacenar solo 4 caracteres + '\0'
	//       pero en el test se traga muchos más. Revisar
	char* msg = new char[5];
	cout << "Receiving UART message" << endl;
	myStatus=irio_getCLuart(&p_DrvPvt,msg,&len,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	message.assign(msg,strlen(msg));

	if (len>0)
		cout << "MSG received: " << message << endl;
	else
		cout << "MSG not received" << endl;

	delete [] msg;

	/**
	 * TEST 7
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 7: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}


TEST(TP_FlexRIO_mod1483, failInitDriverUART) {
	string testName = "TP_FlexRIO_mod1483: Test of bitfile FlexRIOMod1483 when failure";
	string testDescription = "Test tries to check the UART available in the CameraLink interface "
			                 "after a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod1483",
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
	// Expect warnings or error so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

	// Core dumped when configure after failure initializing the driver
	// Don't make sente to use this function on this test
	int32_t value;
//	cout << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
//	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
//	myStatus=irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 1: Testing DMA's configuration" << endl << endl;

	cout << "[irio_getUARTBaudRate] Trying to read UART baud rate..." << endl;
	myStatus=irio_getUARTBaudRate(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getUARTBreakIndicator] Trying to read UART break indicator..." << endl;
	myStatus=irio_getUARTBreakIndicator(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getUARTFrammingError] Trying to read UART framming error..." << endl;
	myStatus=irio_getUARTFrammingError(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getUARTOverrunError] Trying to read UART overrun error..." << endl;
	myStatus=irio_getUARTOverrunError(&p_DrvPvt,&value,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Setting up DMAs to host" << endl << endl;
	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Starting FPGA" << endl << endl;
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 4: Setting FPGA UART baud rate" << endl << endl;
	cout << "[irio_setUARTBaudRate function] Setting FPGA UART BaudRate to 9600" << endl;
	myStatus = irio_setUARTBaudRate(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	// Core dumped when sending data after failure initializing the driver
	// Don't make sente to use this function on this test
//	cout << endl << "TEST 5: Sending data to CameraLink" << endl << endl;
//	cout << "[irio_sendCLuart function]" << endl;
//	cout << "Introduce 4 characters and pulse intro, to send data to the camera" << endl;
//
//	string message;
//	getline(cin, message);
//
//	cout << endl << "Sending UART Message: " << message << endl;
//
//	myStatus=irio_sendCLuart(&p_DrvPvt,message.c_str(), (int) message.size(),&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	// Core dumped when receiving data after failure initializing the driver
	// Don't make sente to use this function on this test
//	cout << endl << "TEST 6: Receiving data from CameraLink" << endl << endl;
//	int len = 0;
//	// TODO: Ver cuánta memoria almaceno en esta variable
//	char* msg = (char*) malloc(5*sizeof(char));
//	cout << "Receiving UART message" << endl;
//	myStatus=irio_getCLuart(&p_DrvPvt,msg,&len,&status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_EQ(myStatus, IRIO_success);
//
//	message.assign(msg,strlen(msg));
//
//	if (len>0)
//		cout << "MSG received: " << message << endl;
//	else
//		cout << "MSG not received" << endl;

	/**
	 * TEST 7
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 7: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}

TEST(TP_FlexRIO_mod1483, functionalIMAQ) {
	string testName = "TP_FlexRIO_mod1483: Functional test of bitfile FlexRIOMod1483 focused on IMAQ profile";
	string testDescription = "Test checks the image acquisition from a CameraLink compatible device";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
	cout << "**************** IRIO IMAQ Profile Example ****************" << endl;
	cout << "**************** Image Acquisition Example ****************" << endl;
	myStatus = irio_initDriver("functionalMod1483ImaqTest",
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
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	 /*
	 * TEST 1
	 * PRE FPGA START CONFIG
	 */
	cout << endl << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
	myStatus=irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/*
	 * TEST 2
	 * FPGA START
	 */
	cout << endl << "TEST 2: Starting FPGA" << endl << endl;
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	/*
	 * TEST 3
	 * ACQUIRING IMAGES
	 */
	cout << endl << "TEST 3: Acquiring 1000 images from CameraLink" << endl << endl;
	int i=0;
	uint16_t fc=0;
	uint16_t *fc2 = nullptr;
	int firstImage=1;
	int count=0;
	int imageSize = 256*256; // Dimension X x Dimension Y
	// Allocate Memory for image
	uint64_t* dataBuffer = new uint64_t[imageSize/8];

	//Start Acquisition
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	// The example is going to acquire 1000 images. This loop needs a variable
	// time depending on the framerate programmed in the camera.
	cout << "1000 frames are going to be acquired. The counter embedded in "
			     "the frame will be recovered and printed. The counter value can "
			     "start at any number but all the values must be consecutive" << endl;

	while(myStatus==IRIO_success && i<1000){
		myStatus = irio_getDMATtoHostImage(&p_DrvPvt, imageSize, 0, dataBuffer, &count, &status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		if(myStatus==IRIO_success){
			if(count==imageSize){
//In this example the CameraLink simulator is injecting a counter in the first two pixels (bytes)
				fc2=(uint16_t*)dataBuffer;
				if(firstImage){
					firstImage=0;
				}else if((fc+1)%imageSize!=fc2[0]){
					irio_mergeStatus(&status,Generic_Error,verbosity,
							         "\nFrameCounter Error at Image fc[i]=%d, fc[i-1]=%d, img: %d\n",fc2[0],fc, i);
					myStatus=IRIO_error;
					break;
				}
				fc=fc2[0];
				cout << "Frame acquired [" << std::setw(3) << std::setfill('0') << i << "], "
						"counter value obtained from image: " << fc << endl;
				i++;
			}else{
				usleep(1000);
			}
		}
	}
	delete [] dataBuffer;

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 4
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 3: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

TEST(TP_FlexRIO_mod1483, failInitDriverIMAQ) {
	string testName = "TP_FlexRIO_mod1483: Test of bitfile FlexRIOMod1483 when failure";
	string testDescription = "Test tries to check image acquisition from a CameraLink "
			                 " compatible device after a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Makes no sense to execute IRIO Library if rioDevice is not correct
	ASSERT_TRUE(RIODevice=="7966") << "Use the correct model of your FlexRIO device";

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod1483",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   "wrongPath",
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	// Expect warnings or error so use this verification when check error tests to allow moving forward to next tests
	EXPECT_NE(myStatus, IRIO_success);

    // Core dumped when configure after failure initializing the driver
	// Don't make sente to use this function on this test
//	cout << endl << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
//	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
//	myStatus=irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
//	if (myStatus > IRIO_success) {
//		TestUtilsIRIO::getErrors(status);
//	}
//	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 2: Starting FPGA" << endl << endl;
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 3: Acquiring image" << endl << endl;
	int i=0;
	uint16_t fc=0;
	uint16_t *fc2 = nullptr;
	int firstImage=1;
	int count=0;
	int imageSize = 256*256; // Dimension X x Dimension Y
	// Allocate Memory for image
	uint64_t* dataBuffer = new uint64_t[imageSize/8];

	//Start Acquisition
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	// The example is going to acquire 2 images. This loop needs a variable
	// time depending on the framerate programmed in the camera.
	cout << "Trying to acquire 2 frames. The counter embedded in "
			"the frame will be recovered and printed. " << endl;

	while(i<2){
		myStatus = irio_getDMATtoHostImage(&p_DrvPvt, imageSize, 0, dataBuffer, &count, &status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_NE(myStatus, IRIO_success);
		if(myStatus==IRIO_success){
			if(count==imageSize){
				fc2=(uint16_t*)dataBuffer;
				if(firstImage){
					firstImage=0;
				}else if((fc+1)%imageSize!=fc2[0]){
					irio_mergeStatus(&status,Generic_Error,verbosity,
									 "\nFrameCounter Error at Image fc[i]=%d, fc[i-1]=%d, img: %d\n",fc2[0],fc, i);
					break;
				}
				fc=fc2[0];
				cout << "Frame acquired [" << std::setw(3) << std::setfill('0') << i << "], "
						"counter value obtained from image: " << fc << endl;
				i++;
			}else{
				usleep(1000);
			}
		}
		else {
			cout << "The frame " << i << " has not been acquired. Something has fail" << endl;
			i++;
		}
	}

	delete [] dataBuffer;

	/**
	 * TEST 4
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 3: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}



