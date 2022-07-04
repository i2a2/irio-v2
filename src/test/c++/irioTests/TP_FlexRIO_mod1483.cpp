#include <gtest/gtest.h>

#include <cstdlib>
#include <string>
#include <iostream>

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
 * PXIe-7966R or PXIe-7975R
 *
 * NI1483 adapter module
 *
 * Two simulator options:
 *    - GIDEL: Don't support UART test. Counter frame embedded vía software.
 *    - EDT: Support UART and IMAQ test. Counter frame embedded vía hardware.
 *
 * The execution of this test requires to have an environment variable indicating the
 * serial number of the RIO board to be used. Execute in a command shell the lsrio command
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 7966, 7975
 * execute export Simulator = yyyy, yyyy = GIDEL or SW (Sowftware simulator)
 * 										   EDT or HW (Hardware simulator)
 * execute export maxCounter = number  (GIDEL = 10, EDT = 65536)
 */

TEST(TP_FlexRIO_mod1483, functionalUART){
	string testName = "TP_FlexRIO_mod1483: Functional test of bitfile FlexRIOMod1483 focused on UART profile";
	string testDescription = "Test checks the UART available in the CameraLink interface";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Two options: GIDEL simulator or EDT simulator
	string Simulator = TestUtilsIRIO::getEnvVar("Simulator");
	int simulator = 1;

	if (Simulator.compare("HW") == 0 || Simulator.compare("EDT") == 0)
		simulator = 0;

	if (!simulator) {   // UART test only supported with Hardware-EDT simulator
		string appCallID = "functionalMod1483UartTest";
		string NIRIOmodel = "PXIe-"+RIODevice+"R";
		string bitfileName = "FlexRIOMod1483_"+RIODevice;
		string FPGAversion = "V1.1"; // User doesn't have to know what FPGA Version is used
		string filePath = "../resources/"+RIODevice+"/";

		int myStatus = 0;
		irioDrv_t p_DrvPvt;
		TStatus status;
		irio_initStatus(&status);

		/**
		 * TEST 0
		 * DRIVER INITIALIZATION
		 */
		cout << "TEST 0: Testing driver initialization" << endl << endl;
		myStatus = irio_initDriver(appCallID.c_str(),
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

		/**
		 * TEST 1
		 * CAMERALINK CONFIGURATION
		 */
		int32_t value = 0;
		cout << endl << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
		cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
		myStatus = irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		myStatus = irio_getUARTBaudRate(&p_DrvPvt,&value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getUARTBaudRate function] Default baud rate: " << value << endl;

		value = -1;
		myStatus = irio_getUARTBreakIndicator(&p_DrvPvt,&value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getUARTBreakIndicator function] UART Break indicator: " << value << endl;

		value = -1;
		myStatus = irio_getUARTFrammingError(&p_DrvPvt,&value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getUARTFrammingError function] UART framing error: " << value << endl;

		value = -1;
		myStatus = irio_getUARTOverrunError(&p_DrvPvt,&value,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getUARTOverrunError function] UART overrun eror: " << value << endl;

		/**
		 * TEST 2
		 * Set up DMAs to host
		 */
		cout << endl << "TEST 2: Testing DMAs' set up configuration." << endl << endl;
		cout << "[irio_setUpDMAsTtoHost function] Set up DMAs" << endl;
		myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		/**
		 * TEST 3
		 * FPGA START
		 */
		cout << endl << "TEST 3: Starting FPGA" << endl << endl;
		cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value 1" << endl;
		myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
		// IRIO can manage success or warning after starting the FPGA, not error
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
			if (myStatus == IRIO_error)
				irio_closeDriver(&p_DrvPvt,0,&status);
		}
		ASSERT_NE(myStatus, IRIO_error);

		// This function does not modify status neither myStatus, it is not necessary to check that variables
		int aivalue=0;
		irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
		cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
			 << aivalue << " (0-stopped, 1-running)" << endl;

		/**
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

		/**
		 * TEST 5
		 * SEND UART MESSAGE
		 */
		cout << endl << "TEST 5: Sending data to CameraLink" << endl << endl;
		cout << "[irio_sendCLuart function]" << endl;
		cout << "Introduce 4 characters and pulse intro, to send data to the camera" << endl;

		string message;
		getline(cin, message);

		cout << endl << "Sending UART Message: " << message << endl;

		myStatus = irio_sendCLuart(&p_DrvPvt,message.c_str(), (int) message.size(),&status);
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

		/**
		 * TEST 6
		 * RECEIVE UART MESSAGE
		 */
		cout << "TEST 6: Receiving data from CameraLink" << endl << endl;
		int len = 0;
		int msg_len = 4;  // User can decide how long can be the message received
		char* msg = new char[msg_len+1]; //+1 because of terminator character \0

		cout << "Receiving UART message" << endl;
		myStatus = irio_getCLuart(&p_DrvPvt,msg_len,msg,&len,&status);
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
		 * IRIO DRIVER CLOSING
		 */
		cout << endl << "TEST 7: Closing IRIO DRIVER" << endl << endl;
		cout << "[irio_closeDriver function] Closing driver..." << endl;
		myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
	}
	else
		cout << "[ERROR] Image Simulator: Select the correct simulator, EDT. "
				"GIDEL Simulator doesn't support UART test." << endl << endl;
}

TEST(TP_FlexRIO_mod1483, functionalIMAQ) {
	string testName = "TP_FlexRIO_mod1483: Functional test of bitfile FlexRIOMod1483 focused on IMAQ profile";
	string testDescription = "Test checks the image acquisition from a CameraLink compatible device";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	// Two options: GIDEL simulator or EDT simulator
	string Simulator = TestUtilsIRIO::getEnvVar("Simulator");
	int frame_counter_mode = -1;

	if (Simulator.compare("SW") == 0 || Simulator.compare("GIDEL") == 0){
		//The simulator doesn't provide the HW counter functionality but the images contains the first two bytes as counter
		frame_counter_mode = 1;
	}
	else{
		//Simulator with framecounter included in the HW functionality
		frame_counter_mode = 0;
	}
	// Counter frame for each image simulator
	int maxCounter = stoi(TestUtilsIRIO::getEnvVar("maxCounter"));

	string appCallID = "functionalMod1483ImaqTest";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string bitfileName = "FlexRIOMod1483_"+RIODevice;
	string FPGAversion = "V1.1"; // User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

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
	myStatus = irio_initDriver(appCallID.c_str(),
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

	/**
	 * TEST 1
	 * PRE FPGA START CONFIG
	 */
	cout << endl << "TEST 1: Configuring CameraLink adapter module" << endl << endl;
	cout << "[irio_configCL function] Configuring cameraLink adapter module" << endl;
	myStatus = irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 2
	 * FPGA START
	 */
	cout << endl << "TEST 2: Testing FPGA start mode" << endl << endl;
	cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value 1" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		if (myStatus == IRIO_error)
			irio_closeDriver(&p_DrvPvt,0,&status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	int aivalue=0;
	irio_getFPGAStart(&p_DrvPvt,&aivalue,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << aivalue << " (0-stopped, 1-running)" << endl;

	/**
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
		myStatus = irio_getDMATtoHostImage(&p_DrvPvt,imageSize,0,dataBuffer,&count,&status);
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
				}else if((fc+1)%maxCounter!=fc2[0]){
					irio_mergeStatus(&status,Generic_Error,verbosity,
							         "\nFrameCounter Error at Image fc[i]=%d, fc[i-1]=%d, img: %d\n"
							         "Review your counter used: %d\n",fc2[0],fc,i,maxCounter);
					TestUtilsIRIO::getErrors(status);
					if (frame_counter_mode) {
						cout << "[ERROR] Image Simulator: check that the first two bytes of each image frame contains the counter "
								"or review that your frame counter parameter is correct. "
								"Frame counter: " << maxCounter << endl << endl;
					}
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
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 4: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}
