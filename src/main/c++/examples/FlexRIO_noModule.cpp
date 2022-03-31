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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>

#define msgtest(n,function)  \
					printf("\n####################################################\n"); \
					printf("IRIO test " #n " " #function " function\\s\n"); \
					printf("####################################################\n")

void msgerr(TIRIOStatusCode code, int nTest, const char* testName, TStatus* status,int verbosity, int exitOnFailure){
	if(code==IRIO_success){
		printf("IRIO test %d %s function\\s successful execution [OK] \n\n",nTest,testName);
		fflush(stdout);
	}else{
		char* detailStr=NULL;
		irio_getErrorString(status->detailCode,&detailStr);
		printf("IRIO test %d %s function\\s unsuccessful execution. Error Code: %d (%s) [ERROR]\n",nTest,testName,status->detailCode,detailStr);
		if(verbosity==0){
			printf("\n\tIRIO ERRORS:\n %s",status->msg);
		}else{
			printf("\n\tCheck previous messages for more detailed information of the error\n");
		}
		free(detailStr); detailStr = NULL;
		fflush(stdout);
		irio_resetStatus(status);

		if(exitOnFailure){
			printf("\n CRITICAL FALIURE Aborting execution\n");
			fflush(stdout);
			exit(-1);
		}
	}
}

void usage(char *name) {
        printf("This example checks the terminals in the FPGA design\n"
                "This example requires a FlexRIO device\n"
                "Use lsrio.py to identify the RIO devices included in the fast controller\n"
        		"\n"
                "Usage: %s <SERIAL_NUMBER> <RIOMODEL> \n"
                "Example: %s 0x01666C59 7965 \n", name, name);
}

int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus=0;

	int aivalue=0;
	int verbosity=1;

	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;

	if (argc != 3) {
			usage(argv[0]);
			return 1;
	}

	asprintf(&filePath,"%s/../../c/examples/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&bitfileName,"FlexRIOnoModule_%s",argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);

	///iRIOCore testing initNIRIODriver
	msgtest(0,irio_initDriver);
	myStatus=irio_initDriver("RIOX",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	free(filePath); filePath = NULL;
	free(bitfileName); bitfileName = NULL;
	free(NIriomodel); NIriomodel = NULL;
//	msgerr(myStatus,0,"irioinitDriver",&status,verbosity,1);


	/// iRIOCore testing irio_closeDriver function
	msgtest(5,irio_closeDriver);
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
//	msgerr(myStatus,5,"irio_closeDriver",&status,verbosity,0);

	return 0;
}

