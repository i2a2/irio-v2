/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for FlexRIO PXIe-7966R+NI5761 analog adapter module
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

		irio FlexRIO (argv1, argv2, "FlexRIOMod5761_","V1.1");

		std::uint64_t *dataBuffer;
		std::int_least16_t *auxDataBuffer;

		dataBuffer=(std::uint64_t *)malloc(4096*8);//4096 data block size

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
		FlexRIO.setSGSignalType(0, 0); // DC signal configured, value 0
		FlexRIO.getSGSignalType(0);
		FlexRIO.checkError(0);

		/**
		* AO0 (FROM WG0) IS CONFIGURED WITH 2048 DIGITAL DC VALUE
		*/

		FlexRIO.irioSleep(100);

		FlexRIO.nextTest("irio_setAO & irio_getAO");
		FlexRIO.setA0(0, 0, 2048); // Set AO terminal to 2048 digital value
		FlexRIO.getA0(0, 0);
		FlexRIO.checkError(0);

		/**
		 * AO0_ENABLE IS ACTIVATED
		 */
		FlexRIO.irioSleep(100);

		FlexRIO.nextTest("irio_setAOEnable & irio_getAOEnable");
		FlexRIO.setA0Enable(0, 1); // AO is enabled
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
		std::int32_t DMA_SR = 500000;
		FlexRIO.setDMATtoHostSamplingRate(0, DMA_SR);
		// equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
		// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
		//		 - SamplingRate is the sampling rate desired to be configured
		//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
		// E.g., If you want 10000S/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate

		FlexRIO.getDMATtoHostSamplingRate(0);
		FlexRIO.checkError(0);

		//Coupling configuration for NI5761
		FlexRIO.nextTest("irio_setAICoupling & irio_getAICoupling");
		std::int32_t coupling = std::atoi(argv3.c_str());

		FlexRIO.setAICoupling(coupling);
		FlexRIO.getAICoupling();
		FlexRIO.checkError(0);

		/**
		 * DMA ENABLE
		 */
		std::int32_t samplingrate=FlexRIO.p_DrvPvt.Fref/DMA_SR;

		FlexRIO.nextTest("irio_setDMATtoHostEnable & irio_getDMATtoHostEnable");

		FlexRIO.setDMATtoHostEnable(0,1);//DMA data transfer to Host is activated
		FlexRIO.getDMATtoHostEnable(0);
		FlexRIO.checkError(1);

		/**
		 * DAQ START
		 */
		FlexRIO.irioSleep(100);

		FlexRIO.nextTest("irio_setDAQStartStop irio_getDAQStartStop");

		FlexRIO.setDAQStartStop(1);// Data acquisition is started
		FlexRIO.getDAQStartStop();
		FlexRIO.checkError(1);

		std::int32_t DMATtoHOSTBlockNWords=FlexRIO.p_DrvPvt.DMATtoHOSTBlockNWords[0];
		std::int32_t DMATtoHOSTNCh=FlexRIO.p_DrvPvt.DMATtoHOSTNCh[0];

		FlexRIO.irioSleep(1000);

		/**
		 * LOOP FOR ACQUIRING ONLY 1 BLOCK
		 */
		std::int32_t positiveTest = 0;
		std::int32_t negativeTest = 0;
		std::int32_t sampleCounter = 0;
		std::int32_t elementsRead = 0;
		FlexRIO.nextTest("setSGFreq setSGUpdateRate setDAQStartStop getDMATtoHostData setDMATtoHostEnable setUpDMAsTtoHost");

		do{

			FlexRIO.getDMATtoHostData(1, 0, dataBuffer, &elementsRead);//1 block of 4096 64 bit words are expected to be acquired

			if (FlexRIO.getMyStatus()!=IRIO_success)
			{
				FlexRIO.checkError(1);
			}

			if(elementsRead==1) //one block is available
			{
				auxDataBuffer=(std::int_least16_t *)dataBuffer;
				sampleCounter++;

				for (int i=0;i<DMATtoHOSTBlockNWords;i++)
				{
					//we are indexing channel 2. In this example CH0 and CH1 are physical channels 0 and 1.
					//CH2 is the digital data generated by the signal generator implemented in the FPGA
					//Signal generator is outputting always 2048. We check if this is correct or not.
					if((auxDataBuffer[(i*DMATtoHOSTNCh)+2]==2048)) //Reads FPGA channel 2
					{
						positiveTest++;
					}
					else
					{
						negativeTest++;
					}
				}
			}
			else
			{
				// we wait at least half the duration of the block in microseconds
				FlexRIO.irioSleep(((1/((std::int64_t)samplingrate))*DMATtoHOSTBlockNWords)*500000);
			}
		}while (sampleCounter<1);
		cout << ("Signal generator configured with DC signal and digital value 2048, is connected (internally in the FPGA) to the AI channel 2\n")
			 << ("4096 samples has been read from channel 2, and 2048 value is expected in all of them\n")
			 << "\t Samples correctly read: "<< positiveTest << "\n\t samples incorrectly read: " << negativeTest << "\n";


		FlexRIO.irioSleep(100000);// This sleep time, is to ensure that FIFO is full of data. FIFO will be cleaned below

		FlexRIO.setDAQStartStop(0);

		FlexRIO.irioSleep(100);

		FlexRIO.cleanDMAsTtoHost();
		FlexRIO.checkError(1);

		/**
		 * WAVEFORM GENERATOR RECONFIGURATION FOR TESTING SAMPLINGRATE AND SIGNAL GENERATION FREQUENCIES
		 */

		cout << ("\n##############################################################\n")
			 <<("The purpose of tests: 14, 15, 16, 17 and 18 consist of configuring internal waveform-generator 0 \n")
		 	 <<("with a SINE signal pattern with an amplitude of 4096 (digital value), and read the output from DMA0 \n")
		 	 <<("################################################################\n");

		FlexRIO.nextTest("irio_setSGUpdateRate & irio_getSGUpdateRate");
		cout << "FPGA SignalGenerator Fref has the value: "<< FlexRIO.p_DrvPvt.SGfref[0] <<" Hz\n";
		// (SGFref/(S/s))=SGUpdateRate. In this case SG0 will generate 10 MS/s
		// At this version, user has to apply this calculus

		FlexRIO.setSGUpdateRate(0, 10000000);
		FlexRIO.getSGUpdateRate(0);
		FlexRIO.checkError(0);

		// we want program signal generator with 10kHz periodic signal
		// equation to apply to obtain freq_desired is: SGFreq=freq_desired*( (2to32) / ( SGFref/(S/s)) )
		std::int32_t accIncr = 0;
		accIncr=10000*(UINT_MAX/(10000000)); // Frequency desired 10kHz

		FlexRIO.nextTest("irio_setSGFreq & irio_getSGFreq");

		FlexRIO.setSGFreq(0,accIncr); //REVISAR ESTO
		FlexRIO.getSGFreq(0);
		FlexRIO.checkError(0);


		FlexRIO.nextTest("irio_setSGAmp & irio_getSGAmp");

		// Amplitude configured (digital value) for WG0 is 4096
		// y(t)=4096sin(2*pi*10000*t) signal configured
		FlexRIO.setSGAmp(0,4096);
		FlexRIO.getSGAmp(0); // Amplitude configured (digital value) for WG0 is 4096
		FlexRIO.checkError(0);

		/**
		 * WG0 CONFIGURED WITH SINE SIGNAL PATTERN
		 */
		FlexRIO.nextTest("irio_setSGSignalType & irio_getSGSignalType (& irio_setAOEnable)");

		FlexRIO.setA0Enable(0, 0);// AO is disabled

		FlexRIO.irioSleep(100);

		FlexRIO.setSGSignalType(0,1);
		FlexRIO.getSGSignalType(0);

		FlexRIO.irioSleep(100000);

		FlexRIO.setA0Enable(0, 1);// AO is enabled

		FlexRIO.checkError(0);

		sampleCounter=0;
		elementsRead=0;

		FlexRIO.nextTest("irio_getDMATtoHostData & irio_setDAQStartStop");

		cout << "Inside the following 60 samples must be approximately 50 samples that represent one sine cycle with a maximum and minimum of "
			 << "+-" << 4096/FlexRIO.p_DrvPvt.CVDAC << "raw_value=+-4096.\n";

		FlexRIO.setDAQStartStop(1);

		if (FlexRIO.getMyStatus()!=IRIO_success)
		{
			FlexRIO.checkError(1);
		}

		/**
		* BUCLE FOR ACQUIRING 1 BLOCK AND ONLY SHOW 51 SAMPLES
		*/
		std::int32_t numOfSamplesToShow = 0;

		do{
			FlexRIO.getDMATtoHostData(1, 0, dataBuffer, &elementsRead);

			if (FlexRIO.getMyStatus()!=IRIO_success)
			{
				FlexRIO.checkError(1);
			}

			if(elementsRead==1)
			{
				numOfSamplesToShow=60; // 50 Samples must represent 1 sine cycle. 10e more are required to check that entire cycle is acquired
				auxDataBuffer=(std::int_least16_t *)dataBuffer;
				sampleCounter++;
				for (std::int32_t i=0;i<numOfSamplesToShow;i++) //only one part of the block is displayed to simplify the output
				{
					cout << "Sample["<< i << "]=" << std::showpos << (auxDataBuffer[(i*DMATtoHOSTNCh)+2]*FlexRIO.p_DrvPvt.CVADC)<< std::noshowpos << "\n";//one period has 100 samples
				}

			}
			else
			{
				// we wait at least half the duration of the block in microseconds
				FlexRIO.irioSleep(((1/(std::int64_t)samplingrate)*DMATtoHOSTBlockNWords)*500000);
			}
		}while (sampleCounter<1);

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


