/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for resource search in FlexRIO devices
 * 				using the different profiles.
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

#include "irioCoreCPP.h"

using std::cout;
using namespace std;



//TODO: This app is pending of rebuiding bitfiles with sufix 7965
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

		irio FlexRIO (argv1, argv2,"FlexRIO_CPUDAQ_","V1.0");
		//-----------------CPUDAQ Test
		FlexRIO.nextTest("CPUDAQ Test");
		cout << "Test 0: CPU DAQ.Should Found:\n";
		cout << "2 DMAs\n";
		cout << "2 AI\n";
		cout << "2 AO\n";
		cout << "2 auxAI\n";
		cout << "2 auxAO\n";
		cout << "2 DI\n";
		cout << "2 DO\n";
		cout << "2 auxDI\n";
		cout << "2 auxDO\n";
		cout << "2 SG\n";


		FlexRIO.init("testCPUDAQ");
		FlexRIO.close();
		FlexRIO.checkError(0);

		//-----------------CPUIMAQ Test
		FlexRIO.nextTest("CPUIMAQ Test");
		FlexRIO.clearBitFileName();
		FlexRIO.setBitFileName("FlexRIO_CPUIMAQ_");

		cout << "Test 1: CPUIMAQ.Should Found:\n";
		cout << "1 IMAQ DMAs\n";
		cout << "1 CLConfig\n";
		cout << "1 CLUART\n";
		cout << "2 auxAI\n";
		cout << "2 auxAO\n";
		cout << "2 DI\n";
		cout << "2 DO\n";
		cout << "2 auxDI\n";
		cout << "2 auxDO\n";

		FlexRIO.init("testCPUIMAQ");
		FlexRIO.close();
		FlexRIO.checkError(0);

		//-----------------all possible FlexRIO I/O resources Test
		FlexRIO.clearBitFileName();
		FlexRIO.setBitFileName("FlexRIOonlyResources_");

		FlexRIO.nextTest("MAXIO Test");
		cout << "Test 2: MAXIO. It should be Found:\n";
		cout << "1 DMA\n";
		cout << "2 AO\n";
		cout << "16 auxAI\n";
		cout << "16 auxAO\n";
		cout << "16 auxDI\n";
		cout << "16 auxDO\n";
		cout << "54 DI \n";
		cout << "54 DO \n";
		cout << "2 SG\n";

		FlexRIO.setFPGAversion("V1.1");
		FlexRIO.init("testMAXIO");
		FlexRIO.close();
		FlexRIO.checkError(0);

		//---------------Missing resources test

		FlexRIO.nextTest("Missing resources test");

		cout << "[Bug7516] This tests checks the correct order in error messages when verbosity=1 \n";
		cout << "Test 3: Missing resources. It should be Found:\n";
		cout << "1 AO + Error finding AO1Enable\n";
		cout << "16 auxAI\n";
		cout << "16 auxAO\n";
		cout << "16 auxDI\n";
		cout << "16 auxDO\n";
		cout << "54 DI \n";
		cout << "54 DO \n";
		cout << "0 SG + Error finding SGFref0 and SGSignalType1\n";
		cout << "0 DMAs + Error finding DMATtoHOSTSamplingRate0\n";

		FlexRIO.clearFilePath();
		FlexRIO.setFilePath("../../c/examples/resourceFail/");

		FlexRIO.init("testMAXIO");
		FlexRIO.close();
		FlexRIO.clearAttributes();
		//Invert status -> previous call should fail
		if(FlexRIO.getMyStatus()!=IRIO_success){
			FlexRIO.setMyStatus(IRIO_success);
		}else{
			FlexRIO.setMyStatus(IRIO_error);
		}

		FlexRIO.checkError(0);

	}


	//-----------------GPUDAQ Test
//	msgtest(4,GPUDAQ Test);
//	printf("Test 4: GPUDAQ.Should Found:\n");
//	asprintf(&bitfileName,"FlexRIO_GPUDAQ_%s",argv[2]);
//	printf("2 GPUDMAs\n");
//	printf("2 AI\n");
//	printf("2 AO\n");
//	printf("2 auxAI\n");
//	printf("2 auxAO\n");
//	printf("2 DI\n");
//	printf("2 DO\n");
//	printf("2 auxDI\n");
//	printf("2 auxDO\n");
//	printf("2 SG\n");
//	printf("This test must be skipped. GPUDAQ is not supported yet.\n");
//	myStatus=irio_initDriver("testGPUDAQ",argv[1],NIriomodel,bitfileName,"V1.0",verbosity,filePath,filePath,&p_DrvPvt,&status);
//	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
//	msgerr(myStatus,4,"GPUDAQ Test",&status,verbosity,0);
//

//	//-----------------GPUIMAQ Test
//	msgtest(5,GPUIMAQ Test);
//	asprintf(&bitfileName,"FlexRIO_GPUIMAQ_%s",riomodel_folder);
//	printf("Test 5: GPUIMAQ.Should Found:\n");
//	printf("1 GPUDMAs\n");
//	printf("1 CLConfig");
//	printf("1 CLUART");
//	printf("2 auxAI\n");
//	printf("2 auxAO\n");
//	printf("2 DI\n");
//	printf("2 DO\n");
//	printf("2 auxDI\n");
//	printf("2 auxDO\n");
//	printf("This test must be skipped. GPUIMAQ is not supported yet.\n");
//	myStatus=irio_initDriver("testGPUIMAQ",argv[1],NIriomodel,bitfileName,"V1.0",verbosity,filePath,filePath,&p_DrvPvt,&status);
//	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
//	msgerr(myStatus,5,"GPUIMAQ Test",&status,verbosity,0);



	return 0;
}
