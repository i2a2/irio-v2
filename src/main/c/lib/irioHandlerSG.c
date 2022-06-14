/**************************************************************************//**
 * \file irioHandlerSG.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Waveform generator handler methods for IRIO driver
 * \date Sept., 2010 (Last Review July 2015)
 * \copyright (C) 2010-2015 Universidad Politécnica de Madrid (UPM)
 * \par License:
 * 	\n This project is released under the GNU Public License version 2.
 * \cond
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * \endcond
 *****************************************************************************/

#include "irioHandlerSG.h"
#include "irioDataTypes.h"
#include "irioResourceFinder.h"
#include "irioError.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @name Signal Generators Resource Strings
 * Strings for SG FPGA Resources
 */
///@{
#define STRINGNAME_SGNO "_IndicatorU8_SGNo"                //!< Identifies SGNo register
#define STRINGNAME_SGFREQ "_ControlU32_SGFreq"             //!< Identifies SGFreq register
#define STRINGNAME_SGAMP "_ControlU16_SGAmp"               //!< Identifies SGAmp register
#define STRINGNAME_SGPHASE "_ControlU32_SGPhase"           //!< Identifies SGPhase register
#define STRINGNAME_SGSIGNALTYPE "_ControlU8_SGSignalType"  //!< Identifies SGSignalType register
#define STRINGNAME_SGUPDATERATE "_ControlU32_SGUpdateRate" //!< Identifies SGPUpdateRate register
#define STRINGNAME_SGFREF "_IndicatorU32_SGFref"           //!< Identifies SGFref register
#define STRINGNAME_FREQREF "_IndicatorU32_Fref"            //!< Identifies Fref register
///@}

int irio_findSGs(irioDrv_t* p_DrvPvt, TStatus* status){
	TIRIOStatusCode local_code = IRIO_success;
	NiFpga_Status fpgaStatus;
	//First search, map and read NoOfSG. This is a mandatory port.
	local_code |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGNO,-1,&p_DrvPvt->enumSGNo,status,p_DrvPvt->verbosity);


	if(p_DrvPvt->enumSGNo.found && local_code==IRIO_success){
		fpgaStatus=NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumSGNo.value,&p_DrvPvt->NoOfSG);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGNO,fpgaStatus);
			return IRIO_error;
		}
	}else{
		return local_code;
	}

	//Search Signal Generators

	//Search up to NoOfSG signal generators.
	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Signal Generators found (Searching %d):",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->NoOfSG);
	}
	int i=0;
	int count=0;
	TStatus local_status,accum_status;
	irio_initStatus(&local_status);
	irio_initStatus(&accum_status);
	while(i<p_DrvPvt->NoOfSG && i<(p_DrvPvt->max_numberofSG)){
		irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGFREQ,i,&p_DrvPvt->enumSGFreq[i],&local_status,0);
		irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGAMP,i,&p_DrvPvt->enumSGAmp[i],&local_status,0);
		irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGPHASE,i,&p_DrvPvt->enumSGPhase[i],&local_status,0);
		irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGSIGNALTYPE,i,&p_DrvPvt->enumSGSignalType[i],&local_status,0);
		irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGUPDATERATE,i,&p_DrvPvt->enumSGUpdateRate[i],&local_status,0);
		irio_findResourceEnum(p_DrvPvt,STRINGNAME_SGFREF,i,&p_DrvPvt->enumSGFref[i],&local_status,0);

		if(local_status.code==IRIO_success){
			//Initialize phase shift to 0
			NiFpga_MergeStatus(&fpgaStatus,
					NiFpga_WriteU32(p_DrvPvt->session,p_DrvPvt->enumSGPhase[i].value,(uint32_t)0));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error initializing %s%d to zero. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGPHASE,i,fpgaStatus);
				local_code |= IRIO_warning;
			}
			//Initialize Sgfref0
			NiFpga_MergeStatus(&fpgaStatus,
					NiFpga_ReadU32(p_DrvPvt->session,p_DrvPvt->enumSGFref[i].value,&(p_DrvPvt->SGfref[i])));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error Reading %s%d from SGFref. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGFREF,i,fpgaStatus);
				local_code |= IRIO_warning;
			}

			if(p_DrvPvt->verbosity){
				printf("SG%d, ",i);
			}
			count++;
		}else{
			irio_mergeStatus(&accum_status,ResourceNotFound_Warning,0,"[%s,%d]-(%s) WARNING Searching SignalGenerator%d resources:\n%s",__func__,__LINE__,p_DrvPvt->appCallID,i,local_status.msg);
			irio_resetStatus(&local_status);

			local_code |= IRIO_warning;

			p_DrvPvt->enumSGFreq[i].found=0;
			p_DrvPvt->enumSGAmp[i].found=0;
			p_DrvPvt->enumSGPhase[i].found=0;
			p_DrvPvt->enumSGSignalType[i].found=0;
			p_DrvPvt->enumSGUpdateRate[i].found=0;
			p_DrvPvt->enumSGFref[i].found=0;
		}
		i++;
	}

	if(accum_status.code!=IRIO_success){
		irio_mergeStatus(status,accum_status.detailCode,p_DrvPvt->verbosity,"%s",accum_status.msg);
		irio_resetStatus(&accum_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Signal Generators found: %d/%d\n",__func__,__LINE__,p_DrvPvt->appCallID,count,p_DrvPvt->NoOfSG);
	}

	if(local_code<IRIO_error){
		return local_code;
	}else{
		return IRIO_error;
	}
}

int irio_getSGFreq(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGFreq[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint32_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU32(p_DrvPvt->session,p_DrvPvt->enumSGFreq[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGFREQ,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGFREQ,n);
		local_status |= IRIO_warning;
	}
	
	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setSGFreq(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGFreq[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		uint32_t aux = (uint32_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU32(p_DrvPvt->session,p_DrvPvt->enumSGFreq[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGFREQ,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGFREQ,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGAmp(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGAmp[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint16_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumSGAmp[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGAMP,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGAMP,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
}
}

int irio_setSGAmp(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGAmp[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		uint16_t aux=(uint16_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU16(p_DrvPvt->session,p_DrvPvt->enumSGAmp[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGAMP,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGAMP,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGPhase(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGPhase[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint32_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU32(p_DrvPvt->session,p_DrvPvt->enumSGPhase[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGPHASE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGPHASE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setSGPhase(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGPhase[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		uint32_t aux=(uint32_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU32(p_DrvPvt->session,p_DrvPvt->enumSGPhase[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGPHASE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGPHASE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGSignalType(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGSignalType[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint8_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumSGSignalType[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGSIGNALTYPE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGSIGNALTYPE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setSGSignalType(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGSignalType[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		uint8_t aux = (uint8_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU8(p_DrvPvt->session,p_DrvPvt->enumSGSignalType[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGSIGNALTYPE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGSIGNALTYPE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGUpdateRate(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGUpdateRate[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint32_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU32(p_DrvPvt->session,p_DrvPvt->enumSGUpdateRate[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGUPDATERATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGUPDATERATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setSGUpdateRate(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumSGUpdateRate[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint32_t aux = (uint32_t)value;

		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU32(p_DrvPvt->session,p_DrvPvt->enumSGUpdateRate[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGUPDATERATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGUPDATERATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getFref(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumFref.found){
		*value = (int32_t) p_DrvPvt->Fref;
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_FREQREF);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGFref(irioDrv_t* p_DrvPvt, int n, uint32_t* value, TStatus* status) {
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumSGFref!= NULL && p_DrvPvt->enumSGFref[n].found) {
		*value = (int32_t) p_DrvPvt->SGfref[n];
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SGFREF);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGCVDAC(irioDrv_t* p_DrvPvt, double* SGCVDAC, TStatus* status) {
	TIRIOStatusCode local_status = IRIO_success;
	if (p_DrvPvt->CVDAC != 0.0) {
		*SGCVDAC=p_DrvPvt->CVDAC;
	}
	else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can't obtain CVDAC (conversion from Volts for analog outputs) parameter \n",__func__,__LINE__,p_DrvPvt->appCallID);
		local_status |= IRIO_warning;
	}
	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSGCVADC(irioDrv_t* p_DrvPvt, double* SGCVADC, TStatus* status) {
	TIRIOStatusCode local_status = IRIO_success;
	if (p_DrvPvt->CVADC != 0.0) {
		*SGCVADC=p_DrvPvt->CVADC;
	}
	else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can't obtain CVADC (conversion to Volts of analog inputs) parameter \n",__func__,__LINE__,p_DrvPvt->appCallID);
		local_status |= IRIO_warning;
	}
	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
