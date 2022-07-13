/*
 * StartUpIRIODriver.cpp
 *
 *  Created on: Apr 27, 2022
 *      Author: ANDRES CUZCO
 */

#include "irioCoreCPP.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <irioDataTypes.h>
#include <climits>
#include <cstdarg>
#include <thread>
#include <chrono>

using std::cout;
using std::this_thread::sleep_for;

irio::irio(std::string argv1, std::string argv2, std::string auxBitFile,std::string FPGAversion) {
	// TODO Auto-generated constructor stub
	//Default values
	this->myStatus = 0;
	this->numberTest = 0;
	this->aivalue = 0;
	this->verbosity = 1;
	this->FPGATemp=-1;
	this->FPGAversion= FPGAversion;

	//Setting values
	this->filePath = "../../c/examples/resourceTest/" + this->filePath.append(argv2);
	this->bitfileName = auxBitFile + this->bitfileName.append(argv2);
	this->NIriomodel = "PXIe-"+ this->NIriomodel.append(argv2)+"R";
	this->model.append(argv2);
	this->NIrioserial.append(argv1);

	// Crea una estructura que permitira almacenar el estado actual en caso de warning, error o success
	// Inicialmente la estructura sera success en todos los parametros
	irio_initStatus(&status);
}

irio::~irio() {

}

int irio::init(std::string appCallID ){

	// This allows the user to make some configurations prior to the start of the execution, specially
	// needed for some FlexRIO adapter modules.

	myStatus |=irio_initDriver(appCallID.c_str(),NIrioserial.c_str(),NIriomodel.c_str(), bitfileName.c_str(),FPGAversion.c_str(),verbosity,filePath.c_str(),filePath.c_str(),&p_DrvPvt,&status);

	return myStatus;
}

int irio::close(){

	myStatus |= irio_closeDriver(&p_DrvPvt,0, &status);

	return myStatus;
}


void irio::nextTest(std::string function){

	nameTest.assign(function);

	cout << "\n**************************************************\n";
	cout << "Executing: " + function +" function\\s\n";
	cout << "**************************************************\n";
}

void irio::checkError(std::int32_t exitOnFailure){

	if(myStatus==IRIO_success){
			cout << "IRIO test " << numberTest << " "<< nameTest << " function\\s successful execution [OK] \n\n";
			cout.flush();
	}else{

		std::string::pointer detailStr;

		irio_getErrorString(status.detailCode,&detailStr);
		cout << "IRIO test "<< numberTest << nameTest << "function\\s unsuccessful execution. Error Code: " << status.detailCode << "(" << detailStr <<") [ERROR]\n";
		if(verbosity==0){
			cout << "\n\tIRIO ERRORS:\n " << status.msg;
		}else{
			cout << "\n\tCheck previous messages for more detailed information of the error\n";
		}

		delete detailStr;

		cout.flush();
		irio_resetStatus(&status);

		if(exitOnFailure){
			cout << "\n CRITICAL FALIURE Aborting execution\n";
			fflush(stdout);
			exit(-1);
		}else {
			myStatus=IRIO_success;
		}


	}
	increaseNumTest();
}

int irio::mergeStatus(std::int32_t code, const char *format, ...){
	std::va_list args;
	va_start(args,format);
	irio_mergeStatus(&status,(TErrorDetailCode)code,verbosity,format,args);

	va_end(args);

	return 0;
}

void irio::irioSleep (std::int32_t useconds){
	sleep_for(std::chrono::microseconds(useconds));
}

void irio::increaseNumTest(){
	numberTest++;
}

void irio::clearFilePath(){
	filePath.clear();
	filePath.shrink_to_fit();
}

void irio::clearBitFileName(){
	bitfileName.clear();
	bitfileName.shrink_to_fit();

}

void irio::clearNIriomodel(){
	NIriomodel.clear();
	NIriomodel.shrink_to_fit();
}

void irio::clearAttributes(){
	clearFilePath();
	clearBitFileName();
	clearNIriomodel();
}

// Esta funcion se implementa sin static ya que se ha indicado en el header que es estatica
void irio::usage(std::string name){

	cout.flush();

	if (name.compare("./FlexRIO_mod5761-7966") == 0)
	{
		cout << "\nThis example checks the data acquisition profile in the FlexRIO device. \nNo external "
				"signal generator is needed because the example uses the internal signal generator implemented in the FPGA\n"
				"This example requires a 796x FlexRIO device with a NI5761 adapter module\n"
				"Use lsrio.py to identify the RIO devices included in the fast controller\n\n"

				"Usage: " + name + " <SERIAL_NUMBER> <RIOMODEL> <NI5761 Coupling AC=0,DC=1>\n"
				"Example: " + name + " 0x0177A2AD 7966 0\n";
	}
	else if (name.compare("./FlexRIO_mod5734-7966") == 0){
		cout << "\nThis example checks the data acquisition profile in the FlexRIO device.\n"
                "This example requires a 7966 FlexRIO device with a NI5734 adapter module\n"
        		"Connect the analog signal to CH0\n"
        		"Sampling rate is 120MS/s\n"
                "Use lsrio.py to identify the RIO devices included in the fast controller\n"
        		"\n"
				"Usage: " + name + " <SERIAL_NUMBER> <RIOMODEL> <NI5734 Coupling AC=0,DC=1>\n"
				"Example: " + name + " 0x0177A2AD 7966 0\n";

	}
	else if (name.compare("./FlexRIO_mod1483-7966-lmaq") ==0 ){
		cout << "\nThis example checks the image acquisition from a cameralink compatible device\n"
				"This example requires a FlexRIO device 7966 with a NI1483\n"
				"Use lsrio.py to identify the RIO devices included in the fast controller\n"
				"\n"
				"Usage: " + name + " <SERIAL_NUMBER> <RIOMODEL> \n"
				"Example: " + name + " 0x01A34CC7 7966 \n";

	}
	else if (name.compare("./FlexRIO_mod6581-7965") ==0 ){
		cout << "This example checks the use of digital lines in the NI6581\n"
                "This example requires a FlexRIO device with a NI6581 adapter module\n"
                "Use lsrio.py to identify the RIO devices included in the fast controller\n"
        		"\n"
				"Usage: " + name + " <SERIAL_NUMBER> <RIOMODEL> \n"
				"Example: " + name + " 0x01666C59 7965 \n";

	}
	else {
		cout << "\nThis example checks the terminals in the FPGA design\n"
				"This example requires a FlexRIO device\n"
				"Use lsrio.py to identify the RIO devices included in the fast controller\n"
				"\n"
				"Usage: " + name + " <SERIAL_NUMBER> <RIOMODEL> \n"
				"Example: " + name + " 0x01666C59 7965 \n";
	}
	cout.flush();
}

int irio::startSetFPGA(std::int32_t value){

	cout << "[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value " << value << " \n";
	myStatus|=irio_setFPGAStart(&p_DrvPvt,value,&status);

	return myStatus;
}

int irio::startGetFPGA(){

	myStatus|=irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	cout << "[irio_getFPGAStart function] FPGA State is: " << aivalue << " . 1-->\"running\" 0-->\"stopped\"\n";

	return myStatus;
}

int irio::setDebugMode(std::int32_t value){

	std::string enableValue;

	if(value==1){
		enableValue="1 (ON)\n";
	}else
		enableValue = "0 (OFF)\n";

	cout << "[irio_setDebugMode function] DebugMode set to " << enableValue;
	myStatus|=irio_setDebugMode(&p_DrvPvt,value,&status);

	return myStatus;
}

int irio::getDebugMode(){

	myStatus|=irio_getDebugMode(&p_DrvPvt, &aivalue,&status);
	cout << "[irio_getDebugMode function] DebugMode read: " << aivalue<<" \n" ;

	return myStatus;
}

int irio::setSGSignalType(std::int32_t numGen, std::int32_t signalType){

	std::string signal;

	if(signalType==0){
		signal="0 (DC)\n";
	}else
		signal = "1 (SINE)\n";

	cout << "[irio_setSGSignalType function] SGSignalType " << numGen << " set to " << signal;

	myStatus|=irio_setSGSignalType(&p_DrvPvt,numGen,signalType,&status);

	return myStatus;
}

int irio::getSGSignalType(std::int32_t numGen){


	myStatus|=irio_getSGSignalType(&p_DrvPvt,numGen,&aivalue,&status);

	cout << "[irio_getSGSignalType function] SGSignalType: "  << numGen << " read " << aivalue << " \n";

	return myStatus;
}

int irio::setSGUpdateRate(std::int32_t numGen, std::int32_t value){

	cout << "[irio_setSGUpdateRate function] SGUpdateRate " << numGen << " set to " << (p_DrvPvt.SGfref[0]/value)<< " MS/s\n";

	myStatus|=irio_setSGUpdateRate(&p_DrvPvt, numGen, (p_DrvPvt.SGfref[0]/value), &status);

	return myStatus;
}

int irio::getSGUpdateRate(std::int32_t numGen){


	myStatus|=irio_getSGUpdateRate(&p_DrvPvt, numGen, &aivalue, &status);
	cout << "[irio_getSGUpdateRate function] SGUpdateRate "  << numGen << " read " << aivalue << " MS/s\n";

	return myStatus;
}

int irio::setSGFreq (std::int32_t numGen, std::int32_t valueFreq){

	cout << "[irio_setSGFreq function] SGFreq " << numGen << " set to " << valueFreq << ", meaning " << valueFreq/(UINT_MAX/(10000000)) <<"Hz\n";

	myStatus|=irio_setSGFreq(&p_DrvPvt, numGen, valueFreq, &status);

	return myStatus;
}
int irio::getSGFreq (std::int32_t numGen){

	myStatus|=irio_getSGFreq(&p_DrvPvt, numGen, &aivalue, &status);
	cout << "[irio_getSGFreq function] SGFreq "  << numGen << " read " << aivalue << ", meaning " << aivalue/(UINT_MAX/(10000000)) <<"Hz\n";

	return myStatus;
}

int irio::setSGAmp (std::int32_t numGen, std::int32_t valueAmp){

	// Amplitude configured (digital value) for WG0 is 4096
	cout << "[irio_setSGAmp function] SGAmp " << numGen << " set to " << valueAmp << ", meaning " << valueAmp/p_DrvPvt.CVDAC <<"V \n";

	myStatus|=irio_setSGAmp(&p_DrvPvt, numGen, valueAmp, &status);// y(t)=4096sin(2*pi*10000*t) signal configured

	return myStatus;
}
int irio::getSGAmp (std::int32_t numGen){

	myStatus|=irio_getSGAmp(&p_DrvPvt, numGen, &aivalue, &status);
	cout << "[irio_getSGAmp function] SGAmp "  << numGen << " read " << aivalue << ", meaning " << aivalue/p_DrvPvt.CVDAC <<"V \n";// Amplitude configured (digital value) for WG0 is 4096

	return myStatus;

}

int irio::cleanDMAsTtoHost(){

	myStatus|=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned

	return myStatus;
}

int irio::setUpDMAsTtoHost(){

	myStatus|=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);

	return myStatus;
}

int irio::setDMATtoHostSamplingRate (std::int32_t numDMA, std::int32_t valueSR) {

	cout << "[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA " << numDMA << " set to " << valueSR << "S/s \n";
	myStatus|=irio_setDMATtoHostSamplingRate(&p_DrvPvt, numDMA, (p_DrvPvt.Fref/valueSR), &status);
	// equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - SamplingRate is the sampling rate desired to be configured
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000S/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate

	return myStatus;
}
int irio::getDMATtoHostSamplingRate (std::int32_t numDMA){


	myStatus|=irio_getDMATtoHostSamplingRate(&p_DrvPvt, numDMA, &aivalue, &status);
	cout << "[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA " << numDMA << " read: " << p_DrvPvt.Fref/aivalue << "S/s \n";

	return myStatus;
}

int irio::getDMATtoHostData (std::int32_t NBlocks, std::int32_t numDMA, std::uint64_t *data, std::int32_t *elementsRead){

	myStatus|=irio_getDMATtoHostData(&p_DrvPvt,NBlocks,numDMA,data, elementsRead,&status); //1 block of 4096 64 bit words are expected to be acquired

	return myStatus;
}

int irio::getDMATtoHostImage (std::int32_t imageSize, std::int32_t numDMA, std::uint64_t *data, std::int32_t *elementsRead){

	myStatus|=irio_getDMATtoHostImage(&p_DrvPvt,imageSize,numDMA,data, elementsRead,&status); //1 block of 4096 64 bit words are expected to be acquired

	return myStatus;
}

int irio::setDMATtoHostEnable(std::int32_t numDMA, std::int32_t valueEna){

	std::string enableValue;

	if(valueEna==1){
		enableValue="1 (ON)\n";
	}else
		enableValue = "0 (OFF)\n";

	cout << "[irio_setDMATtoHostEnable function] DMATtoHostEnable " << numDMA << " set to "<< enableValue;
	myStatus|=irio_setDMATtoHostEnable(&p_DrvPvt,numDMA,valueEna,&status);


	return myStatus;
}

int irio::getDMATtoHostEnable(std::int32_t numDMA){

	myStatus|=irio_getDMATtoHostEnable(&p_DrvPvt,numDMA,&aivalue,&status);
	cout << "[irio_getDMATtoHostEnable function] DMATtoHostEnable " << numDMA << " value read is: "<< aivalue << "\n";

	return myStatus;
}

int irio::setAICoupling(std::int32_t coupling){

	cout <<"[irio_setAICoupling function] AICoupling set to " << coupling << " (AC=0, DC=1)\n";
	myStatus|=irio_setAICoupling(&p_DrvPvt,(TIRIOCouplingMode)coupling, &status);

	return myStatus;
}

int irio::getAICoupling(){

	myStatus|=irio_getAICoupling(&p_DrvPvt, (TIRIOCouplingMode *)&aivalue, &status);
	cout <<"[irio_getAICoupling function] AICoupling read : " << aivalue << "\n";

	return myStatus;

}

int irio::setDAQStartStop(std::int32_t value){

	std::string enableValue;

	if(value==1){
		enableValue="1 (ON)\n";
	}else
		enableValue = "0 (OFF)\n";

	cout <<"[irio_setDAQStartStop function] DAQStartStop set to " << enableValue;
	myStatus|=irio_setDAQStartStop(&p_DrvPvt,value, &status);

	return myStatus;
}

int irio::getDAQStartStop(){

	myStatus|=irio_getDAQStartStop(&p_DrvPvt, &aivalue, &status);
	cout <<"[irio_getDAQStartStop function] DAQStartStop read: " << aivalue << "\n";

	return myStatus;

}

int irio::getAI(std::int32_t typeAux, std::int32_t numPort){

	// Only Analog Input Ports
	if (typeAux==0){ //NORMAL INPUT

		myStatus|=irio_getAI(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getAI function] value read from AI" << numPort << " is:" << aivalue << "\n\n";
	}else { //AUX INPUT
		myStatus|=irio_getAuxAI(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getAuxAI function] value read from auxAI" << numPort << " is:" << aivalue << "\n\n";
	}

	return myStatus;

}


int irio::setA0(std::int32_t typeAux, std::int32_t numPort, std::int32_t valuePort){

	// Only Analog Output Ports
	if (typeAux==0){ //NORMAL OUTPUT
		cout << "[irio_setAO function] value " << valuePort << " is set in AO"<< numPort << "\n";
		myStatus|=irio_setAO(&p_DrvPvt,numPort,valuePort,&status);

	}else { //AUX OUTPUT
		cout << "[irio_setAuxAO function] value " << valuePort << " is set in auxAO"<< numPort << "\n";
		myStatus|=irio_setAuxAO(&p_DrvPvt,numPort,valuePort,&status);
	}

	return myStatus;
}

int irio::getA0(std::int32_t typeAux, std::int32_t numPort){

	// Only Analog Output Ports
	if (typeAux==0){ //NORMAL OUTPUT
		myStatus|=irio_getAO(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getAO function] value read from AO"<< numPort << " is:" << aivalue << "\n";

	}else { //AUX OUTPUT
		myStatus|=irio_getAuxAO(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getAuxAO function] value read from auxAO"<< numPort << " is:" << aivalue << "\n";
	}

	return myStatus;
}


int irio::setA0Enable(std::int32_t numAO, std::int32_t valueEna){

	std::string enableValue;

	if(valueEna==1){
		enableValue="1 (ON)\n";
	}else
		enableValue = "0 (OFF)\n";

	cout << "[irio_setAOEnable function] AOEnable " << numAO << " set to "<< enableValue;
	myStatus|=irio_setAOEnable(&p_DrvPvt,numAO,valueEna,&status);

	return myStatus;
}

int irio::getA0Enable(std::int32_t numAO){

	myStatus|=irio_getAOEnable(&p_DrvPvt,numAO,&aivalue,&status);
	cout << "[irio_getAOEnable function] AOEnable " << numAO << " value read is: "<< aivalue << "\n";

	return myStatus;
}

int irio::getDI(std::int32_t typeAux, std::int32_t numPort){

	// Only DIGITAL Input Ports
	if (typeAux==0){ //NORMAL INPUT
		myStatus|=irio_getDI(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getDI function] value read from DI" << numPort << " is:" << aivalue << "\n\n";

	}else { //AUX INPUT
		myStatus|=irio_getAuxDI(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getAuxDI function] value read from auxDI" << numPort << " is:" << aivalue << "\n\n";
	}

	return myStatus;
}

int irio::setD0(std::int32_t typeAux, std::int32_t numPort, std::int32_t valuePort){

	// Only DIGITAL Output Ports
	if (typeAux==0){ //NORMAL OUTPUT
		cout << "[irio_setDO function] value " << valuePort << " is set in DO"<< numPort << "\n";
		myStatus|=irio_setDO(&p_DrvPvt,numPort,valuePort,&status);

	}else { //AUX OUTPUT
		cout << "[irio_setAuxDO function] value " << valuePort << " is set in auxDO"<< numPort << "\n";
		myStatus|=irio_setAuxDO(&p_DrvPvt,numPort,valuePort,&status);
	}

	return myStatus;
}

int irio::getD0(std::int32_t typeAux, std::int32_t numPort){

	// Only DIGITAL Output Ports
	if (typeAux==0){ //NORMAL OUTPUT
		myStatus|=irio_getDO(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getDO function] value read from DO"<< numPort << " is:" << aivalue << "\n";

	}else { //AUX OUTPUT
		myStatus|=irio_getAuxDO(&p_DrvPvt,numPort,&aivalue,&status);
		cout << "[irio_getAuxDO function] value read from auxDO"<< numPort << " is:" << aivalue << "\n";
	}

	return myStatus;
}

int irio::getDevTemp(){

	myStatus|=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	cout << "[irio_getDevTemp function] temperature read from FPGA: " <<std::fixed<< std::setprecision(2)<< FPGATemp*0.25 <<"ºC\n";

	return myStatus;
}

int irio::configCL(std::int32_t fvalHigh, std::int32_t lvalHigh, std::int32_t dvalHigh, std::int32_t spareHigh, std::int32_t controlEnable, std::int32_t linescan,std::int32_t signalMapping, std::int32_t configuration){

	myStatus|=irio_configCL(&p_DrvPvt,fvalHigh,lvalHigh,dvalHigh,spareHigh,controlEnable,linescan,(CL_SignalMapping_T)signalMapping,(CL_Config_T) configuration,&status);

	return myStatus;
}

std::int32_t irio::getMyStatus(){
	return myStatus;
}
void irio::setMyStatus(std::int32_t value){
	this->myStatus =value;
}

std::int32_t irio::getNumberTest(){
	return numberTest;
}
void irio::setNumberTest(std::int32_t value){
	this->numberTest =value;
}

std::int32_t irio::getAivalue(){
	return aivalue;
}
void irio::setAivalue(std::int32_t value){
	this->aivalue =value;
}

std::int32_t irio::getVerbosity(){
	return verbosity;
}
void irio::setVerbosity(std::int32_t value){
	this->verbosity =value;
}

std::string irio::getFilePath(){

	return filePath;
}
void irio::setFilePath(std::string filePath){

	this->filePath= filePath.c_str() + model + "/";
}

std::string irio::getBitFileName() {
	return bitfileName;
}
void irio::setBitFileName(std::string bitfileName) {
	this->bitfileName = bitfileName + model;
}

std::string irio::getFPGAversion(){
	return FPGAversion;
}
void irio::setFPGAversion(std::string version) {
	this->FPGAversion = version;
}

std::string irio::getNameTest(){

	return nameTest;
}
void irio::setNameTest(std::string nameTest){

	this->nameTest=nameTest;
}


