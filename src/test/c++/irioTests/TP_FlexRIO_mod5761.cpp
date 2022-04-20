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
#include <irioHandlerDMA.h>
#include <limits.h>
}

#include "testUtils-IRIO.h"

// Always max verbosity
static int verbosity = 1;

using std::cout; using std::endl;
using std::string; using std::cerr;

/**
 * Test verifies the data acquisition profile in the FlexRIO device. No external
 * generator is needed because the example uses the internal signal generator implemented in the FPGA
 * This test is related to the following requirements:
 *
 * PXIE7966R. 7966R device can not be sustituted by any other model
 *
 * NI5761 adapter module
 *
 * This test must be executed with a PXIE7966R
 *
 * The execution of this test requires to have an environment variable indicating the
 * coupling mode that user want to use
 * execute export Coupling=X, X=0 (Coupling mode supported by ITER), 1 (Not supported by ITER)
 */

TEST(TP_FlexRIO_mod5761, functional) {
	string testName = "TP_FlexRIO_mod5761: Functional test of bitfile FlexRIOMod5761";
	string testDescription = "Test verifies the data acquisition profile in the FlexRIO device";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7966";
	string RIOSerial = "0x0177A2AD";
	string Coupling  = TestUtilsIRIO::getEnvVar("Coupling");

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod5761_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("functionalMod5761Test",
							   RIOSerial.c_str(),
							   NIRIOmodel.c_str(),
							   bitfileName.c_str(),
							   FPGAversion.c_str(),
							   verbosity,
							   filePath.c_str(),
							   filePath.c_str(),
							   &p_DrvPvt,
							   &status);

	// In TP_FlexRIO_onlyResources test all parameters of irio_initDriver has been tested, so
	// in all these tests it is suppose that they are not going to be incorrect.
	// Critical failure and closing driver if something fail

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "FPGA must not be started if driver is not initialized correctly." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 1
	 * FPGA START
	 */
	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_NE(myStatus, IRIO_error);

	/**
	 * TEST 2
	 * DEBUG MODE CONFIGURATION: OFF MODE
	 */
	int valueReadI32 = 0;
	usleep(100);
	cout << endl << "TEST 2: Testing debug mode configuration, OFF mode" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 0 (OFF)" << endl;

	myStatus=irio_setDebugMode(&p_DrvPvt,0,&status); // Debug mode set to OFF, value 0
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getDebugMode(&p_DrvPvt,&valueReadI32,&status); // DebugMode FPGA register is read
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getDebugMode function] DebugMode read: " << valueReadI32 << endl << endl;
	EXPECT_EQ(myStatus, IRIO_success);


	cout << "##############################################################" << endl <<
	        "The purpose of tests: 3, 4, 5 and 6 consist of configuring internal "
	        "waveform-generator 0 " << endl << "with a DC (digital value), and read "
	        "the output of the waveform-generator from and auxiliary register" << endl <<
	        "##############################################################" << endl;

	/**
	 * TEST 3
	 * SIGNALTYPE FOR WG 0 CONFIGURATION: DC TYPE
	 */
	usleep(100);
	cout << endl << "TEST 3: Configuring Signal Generator Signal type" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 0 (DC)" << endl;
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,0,&status); // DC signal configured, value 0
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getSGSignalType function] SGSignalType0 read: " << valueReadI32 << endl;
	EXPECT_EQ(myStatus, IRIO_success);

   /**
    * TEST 4
	* AO0 (FROM WG0) IS CONFIGURED WITH 2048 DIGITAL DC VALUE
	*/
	usleep(100);
	int digitalValue = 2048;
	cout << endl << "TEST 4: Configuring the Analog output on port 0 with a digital value" << endl << endl;
	cout << "[irio_setAO function] AO0 set to 2048 (digital value)" << endl;

	myStatus=irio_setAO(&p_DrvPvt,0,digitalValue,&status); // Set AO terminal to 2048 digital value
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getAO(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAO function] AO0 read: " << valueReadI32 << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 5
	 * AO0_ENABLE
	 */
	usleep(100);
	cout << endl << "TEST 5: Configuring the Analog output enable on port 0" << endl << endl;
	cout << "[irio_setAOEnable function] AOEnable0 set to 1 (ENABLE)" << endl;

	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getAOEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAOEnable function] AOEnable0 read: " << valueReadI32 << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 6
	 * auxAI9 WILL BE READ, AND 2048 DIGITAL VALUE IS EXPECTED
	 */
	int valueReadAuxAI9 = 0;
	usleep(100);
	cout << endl << "TEST 6: auxAI9 register must have the same value as the output of the "
			     "signal generator: in this case 2048" << endl << endl;

	myStatus=irio_getAuxAI(&p_DrvPvt,9,&valueReadAuxAI9,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAuxAI function] auxAI9 value read: " << valueReadAuxAI9 << endl;
	EXPECT_EQ(myStatus, IRIO_success);

/**
 * DMA FUNCTION TESTS
 */

	/**
	 * TEST 7
	 * DMA CLEANING
	 */
	usleep(100);
	cout << endl << "TEST 7: Testing DMAs' cleaning." << endl << endl;
	cout << "[irio_cleanDMAsTtoHost function] No output if DMAs have been cleaned successfully" << endl;

	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 8
	 * DMA CONFIGURATION
	 */
	usleep(100);
	cout << endl << "TEST 8: Testing DMAs' set up configuration." << endl;
	cout << "[irio_setUpDMAsTtoHost function] No output if DMAs have been configured successfully" << endl;

	// All DMAs are configured. In this case there is only one DMA implemented in the FPGA
	// with four channels (every channel has a digital value of 16 bits)
	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 9
	 * DMA SAMPLING RATE CONFIGURATION
	 */
	int32_t samplingRate = 500000; // 500k samples/seg
	int32_t Fref = 0;
	usleep(100);
	// Equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - SamplingRate is the sampling rate desired to be configured
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
	cout << endl << "TEST 9: Testing DMAs' sampling rate configuration." << endl << endl;

	myStatus = irio_getFref(&p_DrvPvt, &Fref, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_NE(Fref, 0);
	if (Fref != 0) {
		cout << "FPGA Clock reference (Fref value) is: " << Fref << " Hz" << endl;
		cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to: "
				  << samplingRate << " Samples/s"<< endl;

		int32_t decimationFactor = Fref/samplingRate;
		myStatus=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,decimationFactor,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		myStatus=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueReadI32,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		cout << "[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA0 read: "
				  << Fref/valueReadI32 << " Samples/s" << endl;
		EXPECT_EQ(myStatus, IRIO_success);
	}

	/*
	 * TEST 10
	 * Coupling configuration for NI5761
	 */
	int coupling = std::stoi(Coupling); // std::stoi(str) convert std::string str to integer
	usleep(100);
	cout << endl << "TEST 10: Testing coupling configuration for NI5761 adapter module" << endl << endl;

	cout << "[irio_setAICoupling function] AICoupling (AC = 0, DC = 1) set to : " << coupling << endl;
	myStatus=irio_setAICoupling(&p_DrvPvt, (TIRIOCouplingMode) coupling, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getAICoupling(&p_DrvPvt, (TIRIOCouplingMode*) &coupling, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getAICoupling function] AICoupling read: " << coupling << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 11
	 * DMA ENABLE
	 */
	usleep(100);
	cout << endl << "TEST 11: Testing DMAs' Enable" << endl << endl;

	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	myStatus=irio_getDMATtoHostEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDMATtoHostEnable function] DMATtoHostEnable0 read: " << valueReadI32 << endl;

	/**
	 * TEST 12
	 * DAQ START
	 */
	usleep(100);
	cout << endl << "TEST 12: Set DAQ Start" << endl << endl;

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	myStatus=irio_getDAQStartStop(&p_DrvPvt,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDAQStartStop function] DAQStartStop read: " << valueReadI32 << endl;

	/**
	 * LOOP FOR ACQUIRING ONLY 1 BLOCK
	 */
	usleep(1000);
	int sampleCounter = 0;
	int elementsRead = 0;
	int positiveTest = 0;
	int negativeTest = 0;
	uint64_t* dataBuffer = new uint64_t[4096*8];
	uint16_t DMATtoHOSTBlockNWords = 0;
	uint16_t DMATtoHOSTNCh = 0;

	/**
	 * TEST 13
	 * GETTING DATA FROM DMA
	 */
	cout << endl << "TEST 13: Get 1 block from DMA" << endl << endl;

    myStatus = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt, &DMATtoHOSTBlockNWords, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on getting blocks of "
				     "data from the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	myStatus = irio_getDMATtoHOSTNCh(&p_DrvPvt, &DMATtoHOSTNCh, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on getting blocks of "
				"data from the DMA. Closing driver..." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	do{
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer, &elementsRead,&status); //1 block of 4096 64 bit words are expected to be acquired
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		ASSERT_EQ(myStatus, IRIO_success);

		if(elementsRead==1){ //one block is available
			short int* auxDataBuffer = new short int[4096*8];
			auxDataBuffer = (short int*) dataBuffer;
			sampleCounter++;
			for (int i=0;i<DMATtoHOSTBlockNWords;i++){
				//We are indexing channel 2. In this example CH0 and CH1 are physical channels 0 and 1.
				//CH2 is the digital data generated by the signal generator implemented in the FPGA
				//Signal generator is outputting always 2048. We check if this is correct or not.
				if((auxDataBuffer[(i*DMATtoHOSTNCh)+2]==2048)) //Reads FPGA channel 2
					positiveTest++;
				else
					negativeTest++;
			}
		}
		else
			usleep(((1/((double)samplingRate))*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds
	} while (sampleCounter<1);

	cout << "Signal generator configured with DC signal and digital value 2048, is connected "
			     "(internally in the FPGA) to the AI channel 2" << endl;
	cout << "4096 samples has been read from channel 2, and 2048 value is expected in all "
		         "of them" << endl;
	cout << "Samples correctly read: " << positiveTest << endl << "Samples incorrectly "
			     "read: " << negativeTest << endl << endl;

	usleep(100000); // This sleep time is to ensure that FIFO is full of data. FIFO will be cleaned below
	myStatus=irio_setDAQStartStop(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on stopping the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	usleep(100);
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on cleaning the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * WAVEFORM GENERATOR RECONFIGURATION FOR TESTING SAMPLINGRATE AND SIGNAL GENERATION FREQUENCIES
	 */
	cout << "##############################################################" << endl;
	cout << "The purpose of tests: 14, 15, 16, 17 and 18 consist of configuring "
			     "internal waveform-generator 0 with a SINE signal pattern with an amplitude "
			     "of 4096 (digital value), and read the output from DMA0" << endl;
	cout << "##############################################################" << endl;

	usleep(100);
	uint32_t SGFref = 0;
	int SGChannel = 0;
	samplingRate = 10000000;
	/**
	 * TEST 14
	 * SG Update rate
	 * */
	cout << endl << "TEST 14: Set Signal Generator update rate" << endl << endl;
	myStatus=irio_getSGFref(&p_DrvPvt,SGChannel,&SGFref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "FPGA SignalGenerator Fref has the value: " << SGFref << endl;
	// SGUpdateRate=(SGFref/(Samples/s)). In this case SG0 will generate 10 MS/s
	// At this version, user has to apply this calculus
	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to " << SGFref/samplingRate <<
			     " MSamples/s " << endl;
	myStatus=irio_setSGUpdateRate(&p_DrvPvt,0,SGFref/samplingRate,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getSGUpdateRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	cout << "[irio_getSGUpdateRate function] SGUpdateRate0 read: " << valueReadI32 << " MSamples/s"
			  << endl;
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 15
	 * SG Frequency
	 */
	cout << endl << "TEST 15: Set Signal Generator frequency" << endl << endl;
	// We want program signal generator with 10kHz periodic signal
	// Equation to apply to obtain freq_desired is:
	//       SGFreq = Freq_desired*((2to32)/(SGFref/(Samples/s)))
	uint32_t freqDesired = 10000;
	int accIncr=freqDesired*(UINT_MAX/samplingRate);
	cout << "[irio_setSGFreq function] SGFreq0 set to " << accIncr << ", meaning " << freqDesired << " Hz" << endl;
	myStatus=irio_setSGFreq(&p_DrvPvt, 0, accIncr, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	 myStatus=irio_getSGFreq(&p_DrvPvt, 0, &valueReadI32, &status);
	 cout << "[irio_getSGFreq function] SGFreq0 read " << valueReadI32 << ", meaning " <<
			      valueReadI32/(UINT_MAX/(10000000)) << " Hz" << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 16
	 * SG Amplitude
	 */
	cout << endl << "TEST 16: Set Signal Generator amplitude" << endl << endl;
	int32_t amplitude = 4096;
	double CVDAC = 0.0;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_setSGAmp function] SGAmp0 set to " << amplitude << ", meaning " << amplitude/CVDAC << " V" << endl;
	myStatus=irio_setSGAmp(&p_DrvPvt,0,amplitude,&status); // y(t)=4096sin(2*pi*10000*t) signal configured
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	 myStatus=irio_getSGAmp(&p_DrvPvt, 0, &valueReadI32, &status);
	 cout << "[irio_getSGAmp function] SGAmp0 read " << valueReadI32 << ", meaning " <<
			      valueReadI32/CVDAC << " V" << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 *
	 * TEST 17
	 * SG Type
	 */
	cout << endl << "TEST 17: Set Signal Generator type" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 1 (SINE) " << endl;
	myStatus=irio_setAOEnable(&p_DrvPvt,0,0,&status); // AO is disabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	usleep(100);
	myStatus=irio_setSGSignalType(&p_DrvPvt, 0, 1, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	myStatus=irio_getSGSignalType(&p_DrvPvt,0, &valueReadI32,&status);
	cout << "[irio_getSGSGSignalType function] SGSignalType0 read " << valueReadI32 << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	usleep(100000);
	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 18
	 * DMA DATA
	 */
	cout << endl << "TEST 18: Get data from DMA" << endl << endl;
	cout << "Inside the following 60 samples must be approximately 50 samples "
			     "that represent one sine cycle with a maximum and minimum of +-" <<
				 amplitude/CVDAC << " raw_value +-4096" << endl;

	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	elementsRead = 0;
	sampleCounter = 0;
	do {
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer,&elementsRead,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
			cout << "Test can not continue if there is a failure on getting data from the DMA." << endl;
		}
		ASSERT_EQ(myStatus, IRIO_success);

		if(elementsRead==1) {
			int numOfSamplesToShow=60; // 50 Samples must represent 1 sine cycle. 10e more are required to check that entire cycle is acquired
			short int* auxDataBuffer = new short int [4096*8];
			auxDataBuffer=(short int *)dataBuffer;
			sampleCounter++;
			double CVADC = 0.0;
			myStatus=irio_getSGCVADC(&p_DrvPvt,&CVADC,&status);
			if (myStatus > IRIO_success) {
				TestUtilsIRIO::getErrors(status);
			}
			EXPECT_EQ(myStatus, IRIO_success);
			for (int i=0;i<numOfSamplesToShow;i++){ //only one part of the block is displayed to simplify the output
				cout << "Sample["<< std::setw(2) << std::setfill('0') << i
				     << "]=" << auxDataBuffer[(i*DMATtoHOSTNCh)+2]*CVADC << endl;
			}
		}
		else
			usleep(((1/(double)samplingRate)*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds
	}
	while (sampleCounter<1);

	delete [] dataBuffer;
	/**
	 * TEST 19
	 * IRIO CLOSE DRIVER
	 */
	cout << endl << "TEST 19: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Test to check errors

TEST(TP_FlexRIO_mod5761, failInitDriver) {
	string testName = "TP_FlexRIO_mod5761: Configuring FPGA when there is a failure on driver initialization";
	string testDescription = "Test verifies several warnings and errors if "
			                      "user tries to configure the FPGA when there is a failure on driver initialization";
	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	string RIODevice = "7966";
	string RIOSerial = "0x0177A2AD";
	string Coupling  = TestUtilsIRIO::getEnvVar("Coupling");

	// User doesn't have to know what FPGA Version is used
	string FPGAversion = "V1.1";
	string NIRIOmodel = "PXIe-"+RIODevice+"R";
	string filePath = "../resources/"+RIODevice+"/";
	string bitfileName = "FlexRIOMod5761_"+RIODevice;

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	cout << "TEST 0: Testing driver initialization" << endl << endl;
	myStatus = irio_initDriver("failInitDriverMod5761",
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

	cout << endl << "TEST 1: Start the execution of the FPGA" << endl << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);

	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int valueReadI32 = 0;
	usleep(100);
	cout << endl << "TEST 1: Testing debug mode configuration, OFF mode after failure on initializing the driver" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to 0 (OFF)" << endl;

	myStatus=irio_setDebugMode(&p_DrvPvt,0,&status); // Debug mode set to OFF, value 0
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDebugMode function] Trying to read DebugMode..." << endl;
	myStatus=irio_getDebugMode(&p_DrvPvt,&valueReadI32,&status); // DebugMode FPGA register is read
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);


	usleep(100);
	cout << endl << "TEST 2: Configuring Signal Generator Signal type after failure on initializing the driver" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 0 (DC)" << endl;
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,0,&status); // DC signal configured, value 0
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGSignalType function] Trying to read SG Signal type..." << endl;
	myStatus=irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	int digitalValue = 2048;
	cout << endl << "TEST 3: Configuring the Analog output on port 0 with a digital value after failure on initializing the driver" << endl << endl;
	cout << "[irio_setAO function] AO0 set to 2048 (digital value)" << endl;

	myStatus=irio_setAO(&p_DrvPvt,0,digitalValue,&status); // Set AO0 terminal to 2048 digital value
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAO function] Trying to read Analog Output 0..." << endl;
	myStatus=irio_getAO(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 4: Configuring the Analog output enable on port 0 after failure on initializing the driver" << endl << endl;
	cout << "[irio_setAOEnable] AOEnable0 set to 1 (ENABLE)" << endl;

	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAOEnable function] Trying to read AOEnable0... " << endl;
	myStatus=irio_getAOEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);


	int valueReadAuxAI9 = 0;
	usleep(100);
	cout << endl << "TEST 5: auxAI9 register after failure on initializing "
				 "the driver" << endl << endl;

	cout << "[irio_getAuxAI function] Trying to read auxAI9... " << endl;
	myStatus=irio_getAuxAI(&p_DrvPvt,9,&valueReadAuxAI9,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

/**
 * DMA FUNCTION TESTS
 */

	usleep(100);
	cout << endl << "TEST 6: Testing DMAs cleaning after failure on initializing the driver. "
			     "No output if DMAs have been cleaned successfully" << endl;

	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);


	usleep(100);
	cout << endl << "TEST 7: Testing configurations of DMAs after failure on initializing "
			     "the driver. No output if DMAs have been configured successfully" << endl;

	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int32_t samplingRate = 500000; // 500k samples/seg
	int32_t Fref = 0;
	usleep(100);
	cout << endl << "TEST 8: Testing DMAs sampling rate configuration after failure on initializing the driver." << endl << endl;

	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		cout << "IRIO can not take the Clock reference of the FPGA for signal generation" << endl;
	}
	EXPECT_EQ(Fref, 0);
	cout << "FPGA Clock reference (Fref value) is: " << Fref << " Hz" << endl;
	cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to: "
			  << samplingRate << " Samples/s"<< endl;

	int32_t decimationFactor = Fref/samplingRate;
	myStatus=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,decimationFactor,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHostSamplingRate function] Trying to read sampling rate for DMA0..."  << endl;
	myStatus=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	int coupling = std::stoi(Coupling);
	usleep(100);
	cout << endl << "TEST 9: Testing coupling configuration for NI5761 adapter module" << endl << endl;

	cout << "[irio_setAICoupling function] AICoupling (AC = 0, DC = 1) set to : " << coupling << endl;
	myStatus=irio_setAICoupling(&p_DrvPvt,(TIRIOCouplingMode) coupling,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getAICoupling function] Trying to read AICoupling..." << endl;
	myStatus=irio_getAICoupling(&p_DrvPvt,(TIRIOCouplingMode*) &coupling,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 10: Testing DMA Enable" << endl << endl;

	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHostEnable function] Trying to read DMATtoHostEnable0..." << endl;
	myStatus=irio_getDMATtoHostEnable(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100);
	cout << endl << "TEST 11: Set DAQ Start" << endl << endl;

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDAQStartStop function] Trying to read DAQStartStop..." << endl;
	myStatus=irio_getDAQStartStop(&p_DrvPvt,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	/**
	 * LOOP FOR ACQUIRING ONLY 1 BLOCK
	 */
	usleep(1000);
	int sampleCounter = 0;
	int elementsRead = 0;
	int positiveTest = 0;
	int negativeTest = 0;
	uint64_t *dataBuffer = new uint64_t[4096*8]; //4096 data block size
	uint16_t DMATtoHOSTBlockNWords = 0;
	uint16_t DMATtoHOSTNCh = 0;

	cout << endl << "TEST 12: Get 1 block from DMA" << endl << endl;

	cout << "[irio_getDMATtoHOSTBlockNWords function] Trying to read DMATtoHOSTBlockNWords..." << endl << endl;
	myStatus = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt,&DMATtoHOSTBlockNWords,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getDMATtoHOSTNCh function] Trying to read DMATtoHOSTNCh..." << endl << endl;
	myStatus = irio_getDMATtoHOSTNCh(&p_DrvPvt, &DMATtoHOSTNCh, &status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}

	do{
		cout << "[irio_getDMATtoHostData function] Trying to read DMATtoHostData..." << endl << endl;
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer,&elementsRead,&status); //1 block of 4096 64 bit words are expected to be acquired
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else {
			if(elementsRead==1){ //one block is available
				short int* auxDataBuffer = new short int [4096*8];
				auxDataBuffer=(short int *)dataBuffer;
				sampleCounter++;
				for (int i=0;i<DMATtoHOSTBlockNWords;i++){
					//We are indexing channel 2. In this example CH0 and CH1 are physical channels 0 and 1.
					//CH2 is the digital data generated by the signal generator implemented in the FPGA
					//Signal generator is outputting always 2048. We check if this is correct or not.
					if((auxDataBuffer[(i*DMATtoHOSTNCh)+2]==2048)) //Reads FPGA channel 2
						positiveTest++;
					else
						negativeTest++;
				}
			}
			else
				usleep(((1/((double)samplingRate))*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds

			cout << "Signal generator configured with DC signal and digital value 2048, is connected "
						 "(internally in the FPGA) to the AI channel 2" << endl;
			cout << "4096 samples has been read from channel 2, and 2048 value is expected in all "
						 "of them" << endl;
			cout << "Samples correctly read: " << positiveTest << endl << "Samples incorrectly "
						 "read: " << negativeTest << endl << endl;
		}
		EXPECT_NE(myStatus, IRIO_success);
	} while ((myStatus ? 0 : 1) && sampleCounter < 1);

	usleep(100000); // This sleep time is to ensure that FIFO is full of data. FIFO will be cleaned below

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 0 (OFF)" << endl;
	myStatus=irio_setDAQStartStop(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 13: Testing DMAs cleaning after failure on initializing the driver. "
			     "No output if DMAs have been cleaned successfully" << endl;

	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	/**
	 * WAVEFORM GENERATOR RECONFIGURATION FOR TESTING SAMPLING RATE AND SIGNAL GENERATION FREQUENCIES
	 */

	usleep(100);
	uint32_t SGFref = 0;
	int SGChannel = 0;
	samplingRate = 10000000;

	cout << endl << "TEST 14: Set Signal Generator update rate" << endl << endl;
	myStatus=irio_getSGFref(&p_DrvPvt,SGChannel,&SGFref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "FPGA SignalGenerator Fref has the value: " << SGFref << endl;
	// TODO: Ver cómo poner esto para evitar core dumped
//	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to " << SGFref/samplingRate <<
//				 " MSamples/s " << endl;
	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to SGFref/samplingRate MSamples/s " << endl;
	myStatus=irio_setSGUpdateRate(&p_DrvPvt,0,SGFref/samplingRate,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGUpdateRate function] Trying to read SGUpdateRate0..." << endl;
	myStatus=irio_getSGUpdateRate(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);


	cout << endl << "TEST 15: Set Signal Generator frequency" << endl << endl;
	// We want program signal generator with 10kHz periodic signal
	// Equation to apply to obtain freq_desired is:
	//       SGFreq = Freq_desired*((2to32)/(SGFref/(Samples/s)))
	uint32_t freqDesired = 100000;
	int accIncr=freqDesired*(UINT_MAX/(10000000));
	cout << "[irio_setSGFreq function] SGFreq0 set to " << accIncr << ", meaning " << freqDesired << " Hz" << endl;
	myStatus=irio_setSGFreq(&p_DrvPvt,0,accIncr,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	 myStatus=irio_getSGFreq(&p_DrvPvt, 0, &valueReadI32, &status);
	 cout << "[irio_getSGFreq function] Trying to read SGFreq0..." << endl;
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 16: Set Signal Generator amplitude" << endl << endl;
	int32_t amplitude = 4096;
	double CVDAC = 0.0;

	cout << "[irio_getSGCVDAC function] Trying to read SGCVDAC..." << endl << endl;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
	// TODO: Ver cómo poner esto para evitar core dumped
//	cout << "[irio_setSGAmp function] SGAmp0 set to " << amplitude << ", meaning " << amplitude/CVDAC << " V" << endl;
	cout << "[irio_setSGAmp function] SGAmp0 set to 0.517563 V" << endl;
	myStatus=irio_setSGAmp(&p_DrvPvt,0,amplitude,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGAmp function] Trying to read SGAmp0..." << endl;
	 myStatus=irio_getSGAmp(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 17: Set Signal Generator type" << endl;
	cout << "First is necessary to disable Analog output 0" << endl << endl;
	cout << "[irio_setAOEnable function] AO0 enable set to 0 (DISABLE) " << endl;
	myStatus=irio_setAOEnable(&p_DrvPvt,0,0,&status); // AO is disabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_setSGSignalType function] SGSignalType0 set to 1 (SINE) " << endl;
	usleep(100);
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << "[irio_getSGSGSignalType function] Trying to read SGSignalType0... " << endl;
	myStatus=irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	usleep(100000);
	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	cout << endl << "TEST 18: Get data from DMA" << endl << endl;
	cout << "Inside the following 60 samples must be approximately 50 samples "
				 "that represent one sine cycle with a maximum and minimum of +-" <<
				 amplitude/CVDAC << " raw_value +-4096" << endl;

	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl;
	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);

	elementsRead = 0;
	sampleCounter = 0;
	do {
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer,&elementsRead,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		else {
			if(elementsRead==1) {
				int numOfSamplesToShow=60; // 50 Samples must represent 1 sine cycle. 10e more are required to check that entire cycle is acquired
				short int* auxDataBuffer = new short int [4096*8];
				auxDataBuffer=(short int *)dataBuffer;
				sampleCounter++;
				double CVADC = 0.0;
				myStatus=irio_getSGCVADC(&p_DrvPvt, &CVADC, &status);
				for (int i=0;i<numOfSamplesToShow;i++) //only one part of the block is displayed to simplify the output
					//one period has 100 samples
					cout << "Sample["<< std::setw(2) << std::setfill('0') << i
					     << "]=" << auxDataBuffer[(i*DMATtoHOSTNCh)+2]*CVADC << endl;
			}
			else
				usleep(((1/(double)samplingRate)*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds
		}
		EXPECT_NE(myStatus, IRIO_success);
	} while ((myStatus ? 0 : 1) && sampleCounter < 1);

	delete [] dataBuffer;

	cout << endl << "TEST 19: Closing IRIO DRIVER" << endl << endl;
	cout << "Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_NE(myStatus, IRIO_success);
}
