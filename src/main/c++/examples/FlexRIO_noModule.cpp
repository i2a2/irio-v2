/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for resource access in  FlexRIO devices
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
//#include <thread>
//#include <chrono>

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

		argv1.append(argv[1]);
		argv2.append(argv[2]);

		irio FlexRIO (argv1, argv2, "FlexRIOnoModule_","V1.1");

		///iRIOCore testing initNIRIODriver
		FlexRIO.nextTest("irio_initDriver");
		FlexRIO.init("RIOX");
		FlexRIO.clearAttributes();
		FlexRIO.checkError(1);

		//iRIOCore testing setFPGAStart
		FlexRIO.nextTest("irio_setFPGAStart" );
		FlexRIO.setMyStatus(0); //PONER ESTO SIEMPRE QUE SE DE VALOR A LA VARIABLE myStatus
		FlexRIO.startSetFPGA(1);
		FlexRIO.startGetFPGA();
		FlexRIO.checkError(1);

		//usleep(100);
		FlexRIO.irioSleep(100);

		///iRIOCore testing: irio_setAuxAO irio_getAuxAI functions
		FlexRIO.nextTest("irio_setAuxAO irio_getAuxAO irio_getAuxAI");
		FlexRIO.setMyStatus(0);
		// It is known prior to the execution of the test that there are ten auxiliary analog input ports
		// but only six auxiliary analog output ports instantiated

		for(std::int32_t i=0;i< 6 ;i++)
		{
			FlexRIO.setA0(1,i,0);
			FlexRIO.getA0(1,i);
			FlexRIO.getAI(1,i);

			if (FlexRIO.getAivalue()!=0){
				FlexRIO.mergeStatus(Generic_Error, "AuxAI%d expected value 0, read value:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}

			FlexRIO.setA0(1,i,100);
			FlexRIO.getA0(1,i);
			FlexRIO.getAI(1,i);
			if (FlexRIO.getAivalue()!=100){
				FlexRIO.mergeStatus(Generic_Error, "AuxAI%d expected value 100, read value:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}
		}
		FlexRIO.checkError(0);

		///iRIOCore testing: irio_setAuxDO irio_getAuxDI functions
		FlexRIO.nextTest("irio_setAuxDO irio_getAuxDO irio_getAuxDI");
		FlexRIO.setMyStatus(0);
		for(std::int32_t i=0;i< 6 ;i++)
		{
			FlexRIO.setD0(1,i,0);
			FlexRIO.getD0(1,i);
			FlexRIO.getDI(1,i);

			if (FlexRIO.getAivalue()!=0){
				FlexRIO.mergeStatus(Generic_Error, "AuxDI%d expected value 0, value read:%d [ERROR]\n",i,FlexRIO.getAivalue());
			}

			FlexRIO.setD0(1,i,1);
			FlexRIO.getD0(1,i);
			FlexRIO.getDI(1,i);

			if (FlexRIO.getAivalue()!=1){
				FlexRIO.mergeStatus(Generic_Error, "AuxDI%d expected value 1,read value: %d [ERROR]\n",i,FlexRIO.getAivalue());
			}
		}
		FlexRIO.checkError(0);

		/// iRIOCore testing irio_getDevTemp function
		FlexRIO.nextTest("irio_getDevTemp");
		FlexRIO.setMyStatus(0);
		FlexRIO.getDevTemp();
		FlexRIO.checkError(0);


		/// iRIOCore testing irio_closeDriver function
		FlexRIO.nextTest("irio_closeDriver");
		FlexRIO.setMyStatus(0);
		FlexRIO.close();
		FlexRIO.checkError(0);

	}

	return 0;
}


