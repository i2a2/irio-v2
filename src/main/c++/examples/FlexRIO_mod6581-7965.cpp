/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for FlexRIO without adapter module
 *
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

		irio FlexRIO (argv1, argv2, "FlexRIOMod6581_","V1.1");

		///Initializing the RIO device
		FlexRIO.nextTest("irio_initDriver");
		FlexRIO.init("RIOX");
		FlexRIO.clearAttributes();
		FlexRIO.checkError(1);

		//Starting the FPGA in the FlexRIO
		FlexRIO.nextTest("irio_setFPGAStart" );
		FlexRIO.startSetFPGA(1);
		FlexRIO.checkError(1);

		FlexRIO.irioSleep(100);

		// In this example PORT2 has been configured as output and PORT0 and 1 as inputs.
		FlexRIO.nextTest("irio_setAuxDO irio_getAuxDO");

		// When auxDO6 is true, PORT 2 is updated with the value written in terminals DO0, DO1-- DO7. (see labview implementation)
		// This digital values are expected in the port 1.
		FlexRIO.setD0(1,6,1);
		FlexRIO.getD0(1,6);
		cout << "auxDO6 configured to 1, means that DO[7-0] are connected to port 2 in Connector A\n";
		FlexRIO.checkError(0);

		FlexRIO.irioSleep(10);

		/**
		 * WRITING AND READING DIGITAL I/O, BOTH VALUES: 0 AND 1.
		 */
		FlexRIO.nextTest("irio_setDO irio_getDO irio_getDI");
		cout << "\nIRIO test 3: This test write 0 and 1, into every digital output, and it is read from DO register.\n";
		cout << "Hardware digital I/O [7-0] are interconnected physically, then the value written in DO must be read in the DI too.\n";
		cout << "Although all values written and read are showed, in case of reading an unexpected value, Error message will be shown\n\n";
		fflush(stdout);

		for(std::int32_t i=0;i<8;i++)
		{
			FlexRIO.setD0(0,i,0);
			FlexRIO.getD0(0,i);

			if(FlexRIO.getAivalue()!=0)
			{
				FlexRIO.mergeStatus(Generic_Error, "Expected value in DO%d is 0, but value read is:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}

			FlexRIO.irioSleep(20);

			FlexRIO.getDI(0,i);

			if(FlexRIO.getAivalue()!=0)
			{
				FlexRIO.mergeStatus(Generic_Error, "Expected value in DI%d is 0, but value read is:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}

			FlexRIO.setD0(0,i,1);

			FlexRIO.getD0(0,i);

			if(FlexRIO.getAivalue()!=1)
			{
				FlexRIO.mergeStatus(Generic_Error, "Expected value in DO%d is 1, but value read is:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}

			FlexRIO.irioSleep(10);

			FlexRIO.getDI(0,i);

			if(FlexRIO.getAivalue()!=1)
			{
				FlexRIO.mergeStatus(Generic_Error, "Expected value in DI%d is 1, but value read is:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}
		}
		FlexRIO.checkError(0);

		/**
		* READING FPGA TEMPERATURE
		*/

		FlexRIO.nextTest("irio_getDevTemp");
		FlexRIO.getDevTemp();
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
