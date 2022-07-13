/*
 * irioCoreCPP.h
 *
 *  Created on: Apr 27, 2022
 *      Author: ANDRES CUZCO
 */

/**
 * @file irioCoreCPP
 * @date 25/05/2022
 * @authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * @authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * @authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * @authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * @authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * @authors Andres Cuzco (Universidad Politécnica de Madrid, UPM)
 * @title Irio Class
 * @brief Class used to initialize a driver used in examples
 */
#ifndef SRC_MAIN_C___EXAMPLES_STARTUPIRIODRIVER_H_
#define SRC_MAIN_C___EXAMPLES_STARTUPIRIODRIVER_H_

#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>


#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>


#ifdef __cplusplus
//extern "C" {
//#endif

class irio {
public:
	//Public Attributes/Structures
	irioDrv_t p_DrvPvt;
	TStatus status;


	/**
	 * Constructor of an object of class irio.
	 *
	 * An object of the irio class allows to initialize a driver.
	 *
	 * When this object is created, this contains next variables with default values:
	 * 		myStatus
	 * 		functionStatus
	 * 		numberTest
	 * 		aivalue
	 * 		verbosity
	 * 		FPGATemp
	 *
	 * Other variables are contained in this object but are configured with the parameters:
	 * 		filePAth
	 * 		bitfileName
	 * 		NIriomodel
	 * 		model
	 * 		NIrioserial
	 * 		exitOnFailure
	 *
	 * In addition, the constructor initializes the status structure.
	 *
	 * @param[in] argv1				First parameter, refers to the serial number of RIO target device
	 * @param[in] argv2 			Model number of the RIO target device
	 * @param[in] auxBitFile		Part of the name of the bitfile to be downloaded into the FPGA. It can be modified with setBitFileName function
	 *
	 * @param[in] FPGAversion 		Version of the bitfile (e.g. [1,0] for v1.0). Must match with the value of the FPGA register
	 */
	irio(std::string argv1, std::string argv2,std::string auxBitFile,  std::string FPGAversion);


	virtual ~irio();


	int init(const std::string appCallID);

	int close();


	/**
	 * @brief Prints a message of the current test/function
	 * @param function		Function/Test name
	 */
	void nextTest(std::string function);

	/**
	 * @brief Checks for errors. If a error is detected, this method shows the description of the error code
	 * The information represented depends on the  'verbosity' and 'exitOnFailure' variables
	 * @param exitOnFailure		Exit in error case
	 */
	void checkError(std::int32_t exitOnFailure);

	int mergeStatus(std::int32_t code, const char *format, ...);

	void irioSleep (std::int32_t useconds);

	void increaseNumTest();


	// To clear some attributes
	void clearFilePath();
	void clearBitFileName();
	void clearNIriomodel();
	void clearAttributes();

	/**
	 * @brief Indicates the necessary parameters and shows an example of use
	 * @param name		Executing file name
	 */
	static void usage (std::string name);

	//FPGA METHODS
	int startSetFPGA(std::int32_t);
	int startGetFPGA();

	//DEBUG MODE CONFIGURATION
	int setDebugMode(std::int32_t);
	int getDebugMode();

	//SIGNALTYPE METHODS
	int setSGSignalType(std::int32_t numGen, std::int32_t signalType);
	int getSGSignalType(std::int32_t numGen);

	int setSGUpdateRate (std::int32_t numGen, std::int32_t value);
	int getSGUpdateRate (std::int32_t numGen);

	int setSGFreq (std::int32_t numGen, std::int32_t valueFreq);
	int getSGFreq (std::int32_t numGen);

	int setSGAmp (std::int32_t numGen, std::int32_t valueAmp);
	int getSGAmp (std::int32_t numGen);

	//******** DMA METHODS*******************
	int cleanDMAsTtoHost();
	int setUpDMAsTtoHost();

	int setDMATtoHostSamplingRate (std::int32_t numDMA, std::int32_t valueSR);
	int getDMATtoHostSamplingRate (std::int32_t numDMA);

	int getDMATtoHostData (std::int32_t NBlocks, std::int32_t numDMA, std::uint64_t *data, std::int32_t *elementsRead);

	int getDMATtoHostImage(std::int32_t imageSize, std::int32_t numDMA, std::uint64_t *data, std::int32_t *elementsRead);

	int setDMATtoHostEnable(std::int32_t numDMA, std::int32_t valueEna);
	int getDMATtoHostEnable(std::int32_t numDMA);

	//******** COUPLING METHODS*******************
	int setAICoupling(std::int32_t coupling);
	int getAICoupling();

	// ***********DAQ METHODS ***************
	int setDAQStartStop(std::int32_t value);
	int getDAQStartStop();


	//********ANALOG I/O METHODS*************
	//INPUT
	int getAI(std::int32_t typeAux, std::int32_t numPort);
	//OUTPUT
	int setA0(std::int32_t typeAux, std::int32_t numPort, std::int32_t valuePort);
	int getA0(std::int32_t typeAux, std::int32_t);

	//ENABLES
	int setA0Enable(std::int32_t numAO, std::int32_t valueEna);
	int getA0Enable(std::int32_t numAO);
	//********DIGITAL I/O METHODS*************
	//INPUT
	int getDI(std::int32_t typeAux, std::int32_t);
	//OUTPUT
	int setD0(std::int32_t typeAux, std::int32_t, std::int32_t);
	int getD0(std::int32_t typeAux, std::int32_t);

	//******** OTHERS *******************
	int getDevTemp();

	int configCL(std::int32_t fvalHigh, std::int32_t lvalHigh, std::int32_t dvalHigh, std::int32_t spareHigh, std::int32_t controlEnable, std::int32_t linescan,std::int32_t signalMapping, std::int32_t configuration);

	// ************Getters and Setters***********
	std::int32_t getMyStatus();
	void setMyStatus(std::int32_t);

	std::int32_t getNumberTest();
	void setNumberTest(std::int32_t value);

	std::int32_t getAivalue();
	void setAivalue(std::int32_t value);

	std::int32_t getVerbosity();
	void setVerbosity(std::int32_t);

	std::string getFilePath();
	void setFilePath(std::string filePath);

	std::string getBitFileName();
	void setBitFileName(std::string bitfileName);

	std::string getFPGAversion() ;
	void setFPGAversion(std::string version);

	std::string getNameTest();
	void setNameTest(std::string nameTest);

private:

	std::int32_t myStatus;
	std::int32_t numberTest;
	std::int32_t aivalue;
	std::int32_t verbosity;
	std::string filePath;
	std::string bitfileName;
	std::string NIriomodel;
	std::string model;
	std::string NIrioserial;
	std::int32_t FPGATemp;
	std::string FPGAversion;
	std::string nameTest;
};

#endif
#endif /* SRC_MAIN_C___EXAMPLES_STARTUPIRIODRIVER_H_ */
