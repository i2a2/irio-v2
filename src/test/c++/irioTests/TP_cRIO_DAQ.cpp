#include <gtest/gtest.h>

#include <cstdlib>

// C++ <csignal> header is the equivalent from C <signal> header
#include <csignal>

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
using std::string;

/*
 * Test verifies driver’s ability to test cRIO DAQ (Data Acquisition) Profile.
 *
 * This test is related to the following requirements:
 *
 * NI9159 Chassis
 *
 * /**
 * Main Test program.
 * Input parameters expected as environment variables:
 * Param 1<s/n>: Device serial number, uses lsrio command to get it.
 * Param 2<cRIO model>: Device model, uses lsrio command to get it.
 * Param 3<debug_mode>: configures real or simulated acquisition; debug_mode=1-> analog input 0 and 1 will have +5.0 and -5.0
 * Param 4<voltage_NI9264_CH0>: value in volts for configuring NI9264 CH0 output
 * Param 5<voltage_NI9264_CH1>: value in volts for configuring NI9264 CH1 output
 * Param 6<samples_to_read>: Number of samples to be read when acquisition_mode parameter is 0 (finite acquisition)
 * Param 7<acquisition_mode>: Acquisition mode can be finite: this is with value 0, or it can be continuous: this is with value 1. to stop it use Ctrl+C
 * Param 8<N_blocks>: When acquisition mode is continuous, N_blocks indicates the number of blocs that are read from DMA FIFO (Host side) at once.
 *
 *
 * execute export RIOSerial=0x..........
 * execute export RIODevice=xxxx, where xxxx = 9159
 * execute export debugMode= 0 (or 1)
 * execute export CH0Voltage=xxxx
 * execute export CH1Voltage=xxxx
 * execute export samples=xxxx
 * execute export acquisitionMode=xxxx
 * execute export blockLength=xxxx
 */

int flagExit=0; //Global variable used for checking if continuous acquisition must be stopped.
/**
 *  SIGNAL handler. In case of user type Ctrl+C while acquisition is continuous, then acquisition stops, and program continues
 */
static void signal_handler (int);

static void signal_handler (int signal){
	switch (signal)	{
	case SIGINT:
		flagExit = 1;
		break;
	default:
		break;
	}
}

// Test for ITER hardware: all cRIO modules available
TEST(TP_cRIO_DAQ, functional) {
	string testName = "TP_cRIO_DAQ: Functional test of bitfile cRIO_DAQDMA";
	string testDescription = "Test verifies driver’s ability to check cRIO DAQ profile. "
							 "This example requires a specific chassis configuration. \n"
							 "Below there is information about the input parameters required: \n"
			        		 "\t<s/n>: Device serial number, uses lsrio.py command to get it.\n"
			        		 "\t<cRIO model>: Device model, uses lsrio.py command to get it.\n"
			        		 "\t<debug_mode>: configures real or simulated acquisition; debug_mode=1-> analog input 0 and 1 will have +5.0 and -5.0.\n"
			        		 "\t<voltage_NI9264_CH0>: value in volts for configuring NI9264 CH0 output.\n"
			        		 "\t<voltage_NI9264_CH1>: value in volts for configuring NI9264 CH1 output.\n"
			                 "\t<Total samples_to_read>: Total number of samples to be read when data acquisition_mode is finite. Min: blocksize implemented in the FPGA\n"
			                 "\t<Acquisition_mode>: Acquisition mode can be finite (0) or continuous (1). Continuous is is stopped with Ctrl+C\n"
			        		 "\t<block length>: Number of samples to be read from data acquisition device. Minimun: blocksize\n"
							 "It returns IRIO driver error if cRIO modules are missing";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables

	// TODO: Comentado para probar codigo mas rapido.
	//       Descomentar cuando este terminado definitivamente
//	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
//	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");
//	string debugMode = TestUtilsIRIO::getEnvVar("debugMode");
//	string CH0Voltage = TestUtilsIRIO::getEnvVar("CH0Voltage");
//	string CH1Voltage = TestUtilsIRIO::getEnvVar("CH1Voltage");
//	string Samples = TestUtilsIRIO::getEnvVar("samples");
//	string acquisitionMode = TestUtilsIRIO::getEnvVar("acquisitionMode");
//	string blockLength = TestUtilsIRIO::getEnvVar("blockLength");

	string RIODevice = "9159";
	string RIOSerial = "0x01C10FAC";
	string debugMode = "1";
	string CH0Voltage = "+5.0";
	string CH1Voltage = "-5.0";
	string Samples = "128";
	string acquisitionMode = "0";
	string blockLength = "128";

	string appCallID = "functionalcRIODAQDMATest";
	string NIRIOmodel = "NI 9159";
	string bitfileName = "cRIODAQDMA_"+RIODevice;
	string FPGAversion = "V1.1"; // User doesn't have to know what FPGA Version is used
	// Original bitfile. Check that all cRIO modules are connected to avoid errors
	string filePath = "../../../main/c/examples/resourceTest/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	//Signal handler registration. It permits stops continuous acquisition with Ctrl+C
	std::signal(SIGINT,signal_handler);

	/*
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
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
	}
	ASSERT_EQ(myStatus, IRIO_success);

	// Storage getter methods values
	int valueGetter = -1;

	/**
	 * TEST 1
	 * DMA SET UP
	 */
	cout << endl << "TEST 1: Testing DMAs' set up configuration." << endl << endl;
	cout << "[irio_setUpDMAsTtoHost function] Set up DMAs" << endl;

	// All DMAs are configured. In this case there is only one DMA implemented in the FPGA
	// with four channels (every channel has a digital value of 16 bits)
	myStatus = irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

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
	}
	ASSERT_NE(myStatus, IRIO_error);

	// This function does not modify status neither myStatus, it is not necessary to check that variables
	irio_getFPGAStart(&p_DrvPvt,&valueGetter,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << valueGetter << " (0-stopped, 1-running)" << endl;

	/**
	 * TEST 3
	 * DMA SAMPLING RATE CONFIGURATION
	 */
	int32_t samplingRate = 1;
	int32_t Fref = 0;
	// Equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - SamplingRate is the sampling rate desired to be configured
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000 Samples/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
	cout << endl << "TEST 3: Testing I/O Sampling rate configuration." << endl << endl;

	myStatus = irio_getFref(&p_DrvPvt,&Fref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if value of FPGA clock reference for "
				 "signal generation is 0 because of core dumped exception excepted." << endl;
	}
	EXPECT_NE(Fref, 0);
	if (Fref != 0) {
		samplingRate = Fref; // 1000 Samples/seg, maximum sampling rate
		cout << "[irio_getFref function] FPGA Clock reference, Fref: " << Fref << " Hz" << endl;
		cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to "
			 << Fref/samplingRate << ", meaning " << samplingRate << " Samples/s"<< endl;

		myStatus = irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,Fref/samplingRate,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);

		valueGetter = -1;
		myStatus = irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&valueGetter,&status);
		if (myStatus > IRIO_success) {
			TestUtilsIRIO::getErrors(status);
		}
		EXPECT_EQ(myStatus, IRIO_success);
		cout << "[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA0 read "
			 << valueGetter << ", meaning " << Fref*valueGetter << " Samples/s" << endl;
	}

	// TODO: Usar metodos de informacion de la FPGA? No dan nada de info interesante...
	//       Profile sí, para que diga que es DAQ profile
	/*
	 * TEST 4
	 * DEVICE PROFILE
	 */
	cout << endl << "TEST 4: Testing device profile" << endl << endl;
	valueGetter = -1;
	myStatus = irio_getDevProfile(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDevProfile function] Device Profile configured in the FPGA is: "
		 <<	valueGetter << ". This Device should be 0. (0->DAQ DMA, 1->Point by Point)" << endl;

	/**
	 * TEST 5
	 * DEBUG MODE CONFIGURATION
	 */
	int32_t debugmode = std::stoi(debugMode);
	cout << endl << "TEST 5: Testing debug mode configuration" << endl << endl;
	cout << "[irio_setDebugMode function] DebugMode set to " << debugmode
		 << ". (0-->OFF, 1-->ON)" << endl;
	myStatus = irio_setDebugMode(&p_DrvPvt,debugmode,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDebugMode(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDebugMode function] DebugMode read: " << valueGetter << endl;

   /**
	* TEST 6
	* Configuring AO0 and AO1
	*/
	cout << endl << "TEST 6: Configuring the Analog outputs on CH0, CH1" << endl << endl;
	double CVDAC = 0.0;
	myStatus = irio_getSGCVDAC(&p_DrvPvt,&CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGCVDAC function] CVDAC (conversion from Volts for AO): "
		 << std::fixed << CVDAC << endl;

	double CH0voltage = std::stod(CH0Voltage);

	cout << "[irio_setAO function] AO0 set to " << (int)(CH0voltage*CVDAC) <<
			", meaning " << CH0voltage << " V" << endl;
	myStatus = irio_setAO(&p_DrvPvt,0,CH0voltage*CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getAO(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAO function] AO0 read " << valueGetter << ", meaning "
		 << valueGetter/CVDAC << " V" << endl;

	double CH1voltage = std::stod(CH1Voltage);

	cout << "[irio_setAO function] AO1 set to " << (int)(CH1voltage*CVDAC) <<
			", meaning " << CH1voltage << " V" << endl;
	myStatus = irio_setAO(&p_DrvPvt,1,CH1voltage*CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getAO(&p_DrvPvt,1,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAO function] AO1 read " << valueGetter << ", meaning "
		 << valueGetter/CVDAC << " V" << endl;

	/**
	 * TEST 7
	 * STARTING DATA ACQUISITION
	 */
	cout << endl << "TEST 7: Set DAQ Start" << endl << endl;
	cout << "[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)" << endl << endl;
	myStatus = irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDAQStartStop(&p_DrvPvt,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DAQ." << endl;
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDAQStartStop function] DAQStartStop read: " << valueGetter << endl;

/**
 * WAVEFORM GENERATOR CONFIGURATION
 */

	/**
	 * TEST 8
	 * SG Update rate
	 */
	uint32_t SGFref = 0;
	int updateRate = 1000; // 1000 Samples/seg

	cout << endl << "TEST 8: Set Signal Generator update rate" << endl << endl;
	myStatus = irio_getSGFref(&p_DrvPvt,0,&SGFref,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGFref function] FPGA SignalGenerator Clock Reference, SGFref: " << SGFref << " Hz" << endl;
	// Equation applied to set SGUpdateRate: SGUpdateRate=(SGFref/(Samples/s))
	// Where - SGFref is p_DrvPvt.SGfref, this value is read from FPGA by irioDriver initialization
	//		 - samplesPerSeg is the amount of samples that are going to be generated
	// E.g., If you want 100000 Samples/s then configure (p_DrvPvt.SGfref/10000) in third parameter of irio_setSGUpdateRate
	cout << "[irio_setSGUpdateRate function] SGUpdateRate0 set to " << SGFref/updateRate
		 << ", meaning " << updateRate << " Samples/s" << endl;
	myStatus = irio_setSGUpdateRate(&p_DrvPvt,0,SGFref/updateRate,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getSGUpdateRate(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGUpdateRate function] SGUpdateRate0 read "
		 << valueGetter << ", meaning " << SGFref/valueGetter << " Samples/s" << endl;

	/**
	 * TEST 9
	 * SG Frequency
	 */
	cout << endl << "TEST 9: Set Signal Generator frequency" << endl << endl;
	// We want program signal generator with 100Hz periodic signal
	// Equation to apply to obtain freq_desired is:
	//       SGFreq = Freq_desired*((2to32)/(Samples/s))
	uint32_t freqDesired = 100;
	int32_t SGFreq = freqDesired*(UINT_MAX/updateRate); //100 Hz sampled with 1kHz means 10 samples per cycle
	cout << "[irio_setSGFreq function] SGFreq0 set to " << SGFreq << ", meaning " << freqDesired << " Hz" << endl;
	myStatus = irio_setSGFreq(&p_DrvPvt,0,SGFreq,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getSGFreq(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGFreq function] SGFreq0 read " << valueGetter << ", meaning "
		 << valueGetter/(UINT_MAX/updateRate) << " Hz" << endl;

	/**
	 * TEST 10
	 * SG Type
	 */
	cout << endl << "TEST 10: Set Signal Generator type" << endl << endl;
	cout << "[irio_setSGSignalType function] SGSignalType0 set to 2 (SQUARE) " << endl;
	myStatus = irio_setSGSignalType(&p_DrvPvt,0,2,&status); //0=DC, 1=SINE 2=SQUARE 3=TRIANGULAR
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getSGSignalType(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGSGSignalType function] SGSignalType0 read: " << valueGetter << endl;

	/**
	 * TEST 10
	 * SG Amplitude
	 */
	cout << endl << "TEST 10: Set Signal Generator amplitude" << endl << endl;
	double amplitude = 0.4; // Amplitude in V

	cout << "[irio_setSGAmp function] SGAmp0 set to " << (int)(amplitude*CVDAC)
		 << ", meaning " << amplitude << " V" << endl;
	myStatus = irio_setSGAmp(&p_DrvPvt,0,amplitude*CVDAC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getSGAmp(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGAmp function] SGAmp0 read: " << valueGetter
		 << ", meaning " << valueGetter/CVDAC << " V" << endl;

	/**
	 * TEST 11
	 * SG Phase
	 */
	cout << endl << "TEST 11: Testing Signal Generator phase" << endl << endl;
	int phase = 90;
	cout << "[irio_setSGPhase function] SGPhase0 set to " << phase << endl;
	myStatus = irio_setSGPhase(&p_DrvPvt,0,phase,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getSGPhase(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGPhase function] SGPhase0 read: " << valueGetter << endl;

	/**
	 * TEST 12
	 * AO ENABLE
	 */
	cout << endl << "TEST 12: Configuring the Analog output enable on CH0, CH1" << endl << endl;
	cout << "[irio_setAOEnable function] AOEnable0 set to 1 (ENABLE)" << endl;

	myStatus = irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO0 is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getAOEnable(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAOEnable function] AOEnable0 read: " << valueGetter << endl;

	cout << "[irio_setAOEnable function] AOEnable1 set to 1 (ENABLE)" << endl;

	myStatus = irio_setAOEnable(&p_DrvPvt,1,1,&status); // AO1 is enabled
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getAOEnable(&p_DrvPvt,1,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAOEnable function] AOEnable1 read: " << valueGetter << endl;

	/**
	 * TEST 13
	 * ANALOG INPUT PORTS
	 */

	cout << endl << "TEST 13: Testing Analog Input on CH0, CH1" << endl << endl;
	double CVADC = 0.0;
	myStatus = irio_getSGCVADC(&p_DrvPvt,&CVADC,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getSGCVADC function] CVADC (conversion from AO to Volts): "
		 << std::setw(10) << CVADC << endl;

	valueGetter = -1;
	myStatus = irio_getAI(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAI function] AO0 has been configured previously to 5.0V, "
			"AI0 read: " << valueGetter*CVADC << endl;

	valueGetter = -1;
	myStatus = irio_getAI(&p_DrvPvt,1,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	cout << "[irio_getAI function] AO1 has been configured previously to -5.0V, "
			"AI1 read: " << valueGetter*CVADC << endl;

	/**
	 * TEST 14
	 * DMA ENABLE
	 */
	cout << endl << "TEST 14: Testing DMAs' Enable" << endl << endl;

	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)" << endl;
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);

	valueGetter = -1;
	myStatus = irio_getDMATtoHostEnable(&p_DrvPvt,0,&valueGetter,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on setting up the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDMATtoHostEnable function] DMATtoHostEnable0 read: " << valueGetter << endl;

	/**
	 * TEST 15
	 * DMA PARAMETERS
	 */
	cout << endl << "TEST 15: Getting parameters needed to read data from DMA" << endl << endl;
	uint16_t DMATtoHOSTBlockNWords = 0;
	uint16_t DMATtoHOSTNCh = 0;

	myStatus = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt,&DMATtoHOSTBlockNWords,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on getting number of "
				"data blocks from the DMA." << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDMATtoHOSTBlockNWords function] Acquiring number of words for each DMA block. "
			"BlockNWords: " << DMATtoHOSTBlockNWords << " (MAXIMUM: " << SIZE_HOST_DMAS << ")" << endl;

	myStatus = irio_getDMATtoHOSTNCh(&p_DrvPvt,&DMATtoHOSTNCh,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
		cout << "Test can not continue if there is a failure on getting number of "
				"DMA channels" << endl;
	}
	ASSERT_EQ(myStatus, IRIO_success);
	cout << "[irio_getDMATtoHOSTNCh function] Acquiring number of DMA channels. "
			"DMATtoHOSTNCh: " << DMATtoHOSTNCh << endl;


	/**
	 * TEST 16
	 * GETTING DATA FROM DMA
	 */
	cout << endl << "TEST 16: Getting data from DMA" << endl << endl;

	// Total samples to acquire
	int TotalsamplesToAcq = -1;
	int samples = std::stoi(Samples);
	int blocklength = std::stoi(blockLength);
	int nBlocksPerDMAAcc = -1;

	if (samples <= DMATtoHOSTBlockNWords)
		TotalsamplesToAcq = DMATtoHOSTBlockNWords; //This is the minimum length that can be acquired
	else
		TotalsamplesToAcq = samples;

	//Block length
	if (blocklength > 0 && blocklength <= SIZE_HOST_DMAS){
		nBlocksPerDMAAcc = blocklength/DMATtoHOSTBlockNWords; //number if blocks read at once from FIFO (Host) DMA.
		if (blocklength%DMATtoHOSTBlockNWords > 0)
			nBlocksPerDMAAcc++;
	}

	int acquisitionmode = std::stoi(acquisitionMode);
	int continueAcq = -1;
	//continues acquisition parameter is set
	if (acquisitionmode == 1) {
		continueAcq = 1;
		cout << "DEBUG: Continuous data acquisition reading buffers of length "
			 << DMATtoHOSTBlockNWords*nBlocksPerDMAAcc << ", " << nBlocksPerDMAAcc
			 << " blocks of " << DMATtoHOSTBlockNWords << " samples" << endl;
	}
	else {
		continueAcq = 0;
		cout << "DEBUG: Finite data acquisition reading buffers of length "
			 << DMATtoHOSTBlockNWords*nBlocksPerDMAAcc << ", " << nBlocksPerDMAAcc
			 << " blocks of " << DMATtoHOSTBlockNWords << " samples" << endl;
	}

	cout << "NI9264 AO0 is connected to NI9205 AI0 physically. AO0 is configured with a "
		 << freqDesired << "Hz Square signal, Vp = " << amplitude << "V" << endl;

	cout << "Sampling rate of AI0 is " << samplingRate << "S/s. In AI0 channel "
			"square signal must be read. Every ~" << samplingRate/freqDesired
         << "samples 1 square cycle must be checked" << endl;

	cout << "AI1 is connected to AO1 physically. In AI1, the value read should be close "
			"to the 5th parameter introduced when this program has been executed." << endl;

	// Each block: 4096 words * 64 bits/word = 4096 words * 8 bytes/word
//	uint64_t* data2 = new uint64_t(DMATtoHOSTBlockNWords*nBlocksPerDMAAcc*8);
	uint64_t* data2 = new uint64_t(DMATtoHOSTBlockNWords*nBlocksPerDMAAcc);
	int sampleCounter = 0;
	int elementsRead = -1;
	int32_t *dmaaivalues = NULL;
	int errorFounds = 0;
	int timeout = 0;
	do {
		//attempt to read nBlocksPerDMAAcc block
		myStatus = irio_getDMATtoHostData(&p_DrvPvt,nBlocksPerDMAAcc,0,data2,&elementsRead,&status);
		//elementsRead should be a block if available otherwise 0
		dmaaivalues=(int32_t*)data2;
		if (myStatus > IRIO_success) {
			errorFounds++;
			// TODO: Revisar si esto sirve de algo
//			status.code = IRIO_success;
//			myStatus = IRIO_success;
		}
		if (elementsRead == nBlocksPerDMAAcc){
			timeout = 0;
			sampleCounter += (nBlocksPerDMAAcc*DMATtoHOSTBlockNWords);
			//We count blocks to acquire only in finite acquisition
			if (continueAcq == 0) {
				if (sampleCounter >= TotalsamplesToAcq)
					flagExit=1;
				// In finite mode printf the samples acquired
				for (int x = 0; x < DMATtoHOSTBlockNWords*nBlocksPerDMAAcc; x++){
					cout << "Nº sample [" << std::setw(3) << std::setfill('0') << x
						 << "] AI0 = " << std::fixed <<	dmaaivalues[(x*2)]*CVADC << ", AI1 = "
						 << std::fixed << (double)dmaaivalues[(x*2)+1]*CVADC << endl;
				}
			}
			if ((sampleCounter%(nBlocksPerDMAAcc*DMATtoHOSTBlockNWords))==0)
				cout << "----Total Samples per channel acquired: " << sampleCounter
				     << "; Size of a block: " << DMATtoHOSTBlockNWords << "; Number of "
				        "blocks per read: " << nBlocksPerDMAAcc << "; Number of channels "
					 << DMATtoHOSTNCh << "; Error Founds: " << errorFounds << endl;
		}
		else{
			usleep(500000*((float)(1/Fref))*nBlocksPerDMAAcc*DMATtoHOSTBlockNWords); //half time to get an entire block, then 60ms  multiplied by the number of blocks required
			timeout ++;
			if (timeout==10){
				flagExit=1;
				irio_mergeStatus(&status,DAQtimeout_Warning,verbosity,"[%s,%d]-(%s) "
					 "WARNING timeout error reading from DAQ buffer. Error Code: %d\n",__func__,__LINE__,appCallID.c_str(),status.detailCode);
			}
		}
	} while(flagExit == 0);

	cout << "\t Total Samples acquired: " << sampleCounter << endl;

	delete [] data2;

	/**
	 * TEST 17
	 * CLEANING DMAS
	 */
	cout << endl << "TEST 17: Cleaning DMAs channels" << endl << endl;
	cout << "[irio_cleanDMAsTtoHost function] Cleaning DMAs" << endl;
	myStatus = irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	ASSERT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 18
	 * CLOSING DMAS
	 */
	// Once data acquisition has finished DMAs have to be closed
	cout << endl << "TEST 18: Closing DMAS" << endl << endl;
	cout << "[irio_closeDMAsTtoHost function] Closing DMAs" << endl;
	myStatus = irio_closeDMAsTtoHost(&p_DrvPvt,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);

	/**
	 * TEST 19
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 19: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}

// Tests for UPM hardware: all cRIO modules not available
TEST(TP_cRIO_DAQ, noModules) {
	string testName = "TP_cRIO_DAQ: Functional test of bitfile cRIO_DAQ";
	string testDescription = "Test verifies driver’s ability to check cRIO DAQ profile. "
	                         "This test is focused on the resources implemented in the FPGA "
		                 	 "to improve IRIO code coverage. Several parameters are going to be "
		                     "instantiated with no functionality because cRIO modules are not connected "
		                     "in NI9159 chassis.";

	TestUtilsIRIO::displayTitle("\t\tExecuting test: "+testName, FCYN);
	TestUtilsIRIO::displayTitle(testDescription);

	// Environment variables
	string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
	string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

	string appCallID = "functionalcRIODAQTest";
	string NIRIOmodel = "NI 9159";
	string bitfileName = "cRIO_DAQ";
	string FPGAversion = "V1.0";// User doesn't have to know what FPGA Version is used
	string filePath = "../resources/"+RIODevice+"/";

	int myStatus = 0;
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);

	/**
	 * TEST 0
	 * DRIVER INITIALIZATION
	 */
	cout << "TEST 0: Testing driver initialization and resources" << endl << endl;
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
	}
	ASSERT_EQ(myStatus, IRIO_success);

	// Storage getter methods values
	int valueGetter = -1;

	/**
	 * TEST 1
	 * FPGA START
	 */
	cout << endl << "TEST 1: Testing FPGA start mode" << endl << endl;
	cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value 1" << endl;
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	// IRIO can manage success or warning after starting the FPGA, not error
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
	// This function does not modify status neither myStatus, it is not necessary to check that variables

	irio_getFPGAStart(&p_DrvPvt,&valueGetter,&status);
	cout << "[irio_getFPGAStart function] Getting FPGA state. FPGA State is: "
		 << valueGetter << " (0-stopped, 1-running)" << endl;

	/**
	 * TEST 2
	 * IRIO DRIVER CLOSING
	 */
	cout << endl << "TEST 2: Closing IRIO DRIVER" << endl << endl;
	cout << "[irio_closeDriver function] Closing driver..." << endl;
	myStatus = irio_closeDriver(&p_DrvPvt,0,&status);
	if (myStatus > IRIO_success) {
		TestUtilsIRIO::getErrors(status);
	}
	EXPECT_EQ(myStatus, IRIO_success);
}
