/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for FlexRIO PXIe-7966R+NI5734 analog adapter module
 *
 * Author		 : Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
 *
 * Copyright (c) : 2010-2015 Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 ******************************************************************************/

#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <thread>
#include <chrono>
#include <climits>

#include "irioCoreCPP.h"

using std::cout;
using std::this_thread::sleep_for;

int main (std::int32_t argc, char **argv)
{

	std::string argv1;
	std::string argv2;
	std::string argv3;

	if (argc != 4) {
		irio::usage(argv[0]);
			return 1;

	}
	else {
		argv1 = argv[1];
		argv2 = argv[2];
		argv3 = argv[3];


		irio FlexRIO (argv1, argv2, "FlexRIOMod5734_","V1.1");

		std::uint64_t *dataBuffer=NULL;
		std::int_least16_t *auxDataBuffer;
		std::int32_t DMA_SR =0;
		std::int32_t coupling = 0;
		std::int32_t samplingrate = 0;
		std::int32_t DMATtoHOSTBlockNWords = 0;
		std::int32_t DMATtoHOSTNCh = 0;

		std::int32_t sampleCounter = 0;
		std::int32_t elementsRead = 0;

		std::int32_t numOfSamplesToShow = 0;

		// initialize the RIO device calling the irio_initDriver
		FlexRIO.nextTest("irio_initDriver");
		FlexRIO.init("RIO0");
		FlexRIO.clearAttributes();
		FlexRIO.checkError(1);

		// start the execution of the FPGA
		FlexRIO.nextTest("irio_setFPGAStart" );
		FlexRIO.startSetFPGA(1);
		FlexRIO.checkError(1);

		FlexRIO.irioSleep(100);

		/**
		 * DEBUG MODE CONFIGURATION: OFF MODE
		 */

		FlexRIO.nextTest("irio_setDebugMode & irio_getDebugMode");
		FlexRIO.setDebugMode(0);// Debug mode set to OFF
		FlexRIO.getDebugMode(); // DebugMode FPGA register is read
		FlexRIO.checkError(0);

		/**
		 * SIGNALTYPE FOR WG 0 CONFIGURATION: DC TYPE
		 */

		FlexRIO.irioSleep(100);

		cout << ("\n##############################################################\n")
			 <<	("The purpose of tests: 3, 4, 5 and 6 consist of configuring internal waveform-generator 0 \n")
			 <<	("with a DC (digital value), and read the output of the waveform-generator from and auxiliary register. \n")
			 <<	("################################################################\n");


		FlexRIO.nextTest("irio_setSGSignalType & irio_getSGSignalType");
		FlexRIO.setSGSignalType(0,0);// DC signal configured, value 0
		FlexRIO.getSGSignalType(0);
		FlexRIO.checkError(0);

		/**
		* AO0 (FROM WG0) IS CONFIGURED WITH 2048 DIGITAL DC VALUE
		*/

		FlexRIO.irioSleep(100);

		FlexRIO.nextTest("irio_setAO & irio_getAO");
		FlexRIO.setA0(0, 0, 2048);// Set AO terminal to 2048 digital value
		FlexRIO.getA0(0, 0);
		FlexRIO.checkError(0);

		/**
		 * AO0_ENABLE IS ACTIVATED
		 */

		FlexRIO.irioSleep(100);

		FlexRIO.nextTest("irio_setAOEnable & irio_getAOEnable");
		FlexRIO.setA0Enable(0, 1);// AO is enabled
		FlexRIO.getA0Enable(0);
		FlexRIO.checkError(0);

		FlexRIO.irioSleep(100);

		//In this design the auxAI9 terminal has the last value written by the waveform generator implemented in the FPGA.
		//Above, we have configured the DC output and we have written a 2048, Therefore if we read auxAI9 the expected value is 2048

	   /**
		* auxAI9 WILL BE READ, AND 2048 DIGITAL VALUE IS EXPECTED
		*/

		FlexRIO.nextTest("irio_getAuxAI");
		cout << ("The auxAI9 register must have the same value as the output of the signal generator: in this case 2048\n");
		FlexRIO.getAI(1, 9);
		FlexRIO.checkError(0);

		/**
		 * DMA FUNCTION TESTS
		 */

		/**
		 * DMA CLEANING
		 */

		FlexRIO.nextTest("irio_cleanDMAsTtoHost");

		FlexRIO.cleanDMAsTtoHost();// DMA FIFOs are cleaned
		FlexRIO.checkError(0);

		FlexRIO.irioSleep(100);

		FlexRIO.nextTest("irio_setUpDMAsTtoHost");
		// All DMAs are configured. In this case there is only one DMA implemented in the FPGA with four channels (every channel has a digital value of 16 bits)
		FlexRIO.setUpDMAsTtoHost();
		FlexRIO.checkError(1);

		/**
		 * DMA SAMPLING RATE CONFIGURATION
		 */

		FlexRIO.nextTest("irio_setDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate");
		cout << "FPGA Clock reference (Fref value) is: " <<FlexRIO.p_DrvPvt.Fref << " Hz\n";
		DMA_SR = 120000000;

		//sampling rate desired 1.2MS/s
		FlexRIO.setDMATtoHostSamplingRate(0, DMA_SR);
		// equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
		// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
		//		 - SamplingRate is the sampling rate desired to be configured
		//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
		// E.g., If you want 10000S/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate

		FlexRIO.getDMATtoHostSamplingRate(0);
		FlexRIO.checkError(0);

		//Coupling configuration for NI5761
		//msgtest(10,irio_setAICoupling & irio_getAICoupling);
		//FlexRIO.msgtest("irio_setAICoupling & irio_getAICoupling");
		//FlexRIO.setExitOnFailure(0);

		// setting DC mode directly.
		coupling = std::atoi(argv3.c_str());

		// This part is setting the DC coupling for channel 0 only
		FlexRIO.setA0(1, 6, 3);	//3 is the command to change the DC coupling
		FlexRIO.setA0(1, 7, 0);	//1 is the channel to setup; 0 in this example
		FlexRIO.setA0(1, 8, coupling); //1 is DC mode
		FlexRIO.setA0(1, 6, 0); //Rising edge generation
		FlexRIO.setA0(1, 6, 1); //Rising edge generation

		/**
		 * DMA ENABLE
		 */
		samplingrate=FlexRIO.p_DrvPvt.Fref/DMA_SR;

		FlexRIO.nextTest("irio_setDMATtoHostEnable & irio_getDMATtoHostEnable");
		FlexRIO.setDMATtoHostEnable(0,1);//DMA data transfer to Host is activated
		FlexRIO.getDMATtoHostEnable(0);
		FlexRIO.checkError(1);

		/**
		 * DAQ START
		 */
		//usleep(100000);
		FlexRIO.irioSleep(100000);
		FlexRIO.irioSleep(100000);
		FlexRIO.irioSleep(100000);

		FlexRIO.nextTest("irio_setDAQStartStop irio_getDAQStartStop");
		FlexRIO.setDAQStartStop(1);// Data acquisition is started
		FlexRIO.getDAQStartStop();
		FlexRIO.checkError(1);

		/**
		 * LOOP FOR ACQUIRING ONLY 1 BLOCK AND ONLY SHOW 51 SAMPLES
		 */
		DMATtoHOSTBlockNWords=FlexRIO.p_DrvPvt.DMATtoHOSTBlockNWords[0];
		DMATtoHOSTNCh=FlexRIO.p_DrvPvt.DMATtoHOSTNCh[0];
		sampleCounter = 0;
		elementsRead = 0;
		numOfSamplesToShow = 0;

		do{
			FlexRIO.getDMATtoHostData(1, 0, dataBuffer, &elementsRead);

			if (FlexRIO.getMyStatus()!=IRIO_success)
			{
				FlexRIO.checkError(1);
			}
			if(elementsRead==1)
			{
				numOfSamplesToShow=200; // 50 Samples must represent 1 sine cycle. 10e more are required to check that entire cycle is acquired
				auxDataBuffer=(std::int_least16_t *)dataBuffer;
				sampleCounter++;

				for (std::int32_t i=0;i<numOfSamplesToShow;i++) //only one part of the block is displayed to simplify the output
				{
					//one period has 100 samples
					cout << "Sample["<< i << "]=" << std::showpos << (std::int64_t)(auxDataBuffer[(i*DMATtoHOSTNCh)+0]*FlexRIO.p_DrvPvt.CVADC)<< std::noshowpos << "\n";
					//We are displaying only samples from CH0!!!!
				}

			}
			else
			{
				FlexRIO.irioSleep(((1/(std::int64_t)samplingrate)*DMATtoHOSTBlockNWords)*DMA_SR);// we wait at least half the duration of the block in microseconds

			}
		}	while (sampleCounter<1);

		FlexRIO.setDAQStartStop(0);  // Data acquisition is stopped
		FlexRIO.checkError(0);

		/**
		 * IRIO CLOSE DRIVER
		 */
		FlexRIO.nextTest("irio_closeDriver");
		FlexRIO.close();
		FlexRIO.checkError(0);

	}

	return 0;
}


