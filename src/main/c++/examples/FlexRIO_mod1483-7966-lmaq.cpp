/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for image acquisition using
 * 					a 7966 FlexRIO Device with a 1483 Adapter module
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
#include <thread>
#include <chrono>
#include <cstdarg>

#include "irioCoreCPP.h"

using std::cout;
using std::this_thread::sleep_for;

int main (std::int32_t argc, char **argv)
{
	std::string argv1;
	std::string argv2;


	if (argc != 3) {
		irio::usage(argv[0]);
			return 1;

	}
	else {
		argv1 = argv[1];
		argv2 = argv[2];

		irio FlexRIO (argv1, argv2, "FlexRIOMod1483_","V1.1");

		std::int32_t i=0;
		std::uint16_t fc=0;
		std::uint16_t *fc2;
		std::int32_t firstImage=0;
		std::int32_t count=0;

		std::uint64_t* dataBuffer= NULL;

		cout << "*****************IRIO IMAQ Profile Example*****************\n";
		cout << "*****************Image Acquisition Example*****************\n";

		//******** Download bitfile

		FlexRIO.nextTest("irio_initDriver");
		FlexRIO.init("testCPUDAQ");
		FlexRIO.clearAttributes();
		FlexRIO.checkError(1);

		FlexRIO.nextTest("irio_configCL");
		//Config CameraLink Adapter Module
		FlexRIO.configCL(1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL);
		//Config DMA
		FlexRIO.checkError(1);


		//********* FPGA Start
		FlexRIO.nextTest("irio_setFPGAStart" );

		FlexRIO.irioSleep(1000);

		FlexRIO.startSetFPGA(1);
		FlexRIO.checkError(1);

		//*********** Acquire Images
		//Acquire 1000 Images and check FrameCounter
		FlexRIO.nextTest("Acquiring images" );
		i=0;
		fc=0;
		firstImage=1;
		count=0;

		//Allocate Memory for image
		std::uint64_t* dataBuffer= NULL;
		dataBuffer= (std::uint64_t*)(malloc((256*256/8)*sizeof(uint64_t)));
		//Start Acquisition
		FlexRIO.setDMATtoHostEnable(0, 1);
		FlexRIO.setDAQStartStop(1);
		//The example is going to acquire 1000 images. This loop needs a variable time depending on the framerate programmed in the camera.
		cout << (" 1000 frames are going to be acquired. The counter embedded in the frame will be recovered and printed. The counter value can start at any number but all the values must be consecutive.\n");
		while(FlexRIO.getMyStatus()==IRIO_success && i<1000){

			FlexRIO.getDMATtoHostImage((256*256), 0, dataBuffer, &count);

			if(FlexRIO.getMyStatus()==IRIO_success){
				if(count==256*256){
					fc2=(uint16_t*)dataBuffer; //In this example the cameralink simulator is injecting a counter in the first two pixels (bytes)
					if(firstImage){
						firstImage=0;
					}else if((fc+1)%65536!=fc2[0]){

						FlexRIO.mergeStatus(Generic_Error,"\nFrameCounter Error at Image fc[i]=%d, fc[i-1]=%d, img: %d\n",fc2[0],fc, i);
						FlexRIO.setMyStatus(IRIO_error);
						break;
					}

					fc=fc2[0];
					cout << "Frame acquired ["<<i <<"], counter value obtained from image:"<<fc<<"\n";
					i++;
				}else{
					FlexRIO.irioSleep(1000);
				}
			}
		}

		FlexRIO.checkError(0);

		/// iRIOCore testing irio_closeDriver function
		FlexRIO.nextTest("irio_closeDriver");
		FlexRIO.close();
		FlexRIO.checkError(0);
	}

	return 0;
}


