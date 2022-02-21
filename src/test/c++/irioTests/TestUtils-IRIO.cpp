/*
 * TestUtilsIRIO.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: cmedina
 */

#include <gtest/gtest.h>
#include "../include/testUtils-IRIO.h"
#include <fstream>
#include <cstdlib>

TestUtilsIRIO::TestUtilsIRIO(const std::string& device_name):
//	m_pInterface(nds::tests::TestControlSystemInterfaceImpl::getInstance(device_name)),
	m_deviceName(device_name) {
}

TestUtilsIRIO::TestUtilsIRIO()
	{
}

void TestUtilsIRIO::setInferface(const std::string& device_name) {
//	m_pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance(device_name);
	m_deviceName = device_name;
}

//nds::tests::TestControlSystemInterfaceImpl* TestUtilsIRIO::getPInterface() {
//	return m_pInterface;
//}

//void TestUtilsIRIO::STM_CheckState(std::string nodeStr, std::int32_t currentState, const std::int32_t timeout) {
//	const timespec *pStateMachineSwitchTime;
//	const std::int32_t *pStateMachineState;
//
//	this->m_pInterface->getPushedInt32(nodeStr+"StateMachine.getState", pStateMachineSwitchTime, pStateMachineState, timeout);
//	EXPECT_EQ((std::int32_t)currentState, *pStateMachineState);
//}

//void TestUtilsIRIO::STM_SetState(std::string nodeStr, std::int32_t transitionSt, std::int32_t finalSt, const std::int32_t transitionTimeout) {
//
//	std::cout<<"STM SetState: Switching "<<this->STM_StatesStr[(int) finalSt]<<" in "<<nodeStr.substr(1, nodeStr.size()-2)<<std::endl;
//
//	//Change state:  OFF -> (initializing) -> ON
//	this->m_pInterface->writeCSValue(nodeStr+"StateMachine.setState", m_timestamp, finalSt);
//
//	this->STM_CheckState(nodeStr, transitionSt, transitionTimeout);
//	this->STM_CheckState(nodeStr, finalSt, transitionTimeout);
//
//	this->displayTitle(nodeStr.substr(1, nodeStr.size()-2)+" node "+this->STM_StatesStr[(int) finalSt]+"!", FCYN);
//	::usleep(10000);
//}
//
//void TestUtilsIRIO::STM_SwitchON(std::string nodeStr, const std::int32_t transitionTimeout) {
//	this->STM_SetState(nodeStr, (std::int32_t) nds::state_t::initializing, (std::int32_t) nds::state_t::on, transitionTimeout);
//}
//
//void TestUtilsIRIO::STM_SwitchRUN(std::string nodeStr, const std::int32_t transitionTimeout) {
//	this->STM_SetState(nodeStr, (std::int32_t) nds::state_t::starting, (std::int32_t) nds::state_t::running, transitionTimeout);
//}
//
//void TestUtilsIRIO::STM_SwitchSTOP(std::string nodeStr, const std::int32_t transitionTimeout) {
//	this->STM_SetState(nodeStr, (std::int32_t) nds::state_t::stopping, (std::int32_t) nds::state_t::on, transitionTimeout);
//}
//
//void TestUtilsIRIO::STM_SwitchOFF(std::string nodeStr, const std::int32_t transitionTimeout) {
//	this->STM_SetState(nodeStr, (std::int32_t) nds::state_t::switchingOff, (std::int32_t) nds::state_t::off, transitionTimeout);
//}
//
//
//std::string TestUtilsIRIO::getErrorPVs(const std::string& MainNode) {
//
//	std::string errorSource;
//	std::string errorSeverity;
//	std::string errorMessage;
//
//	timespec pvWrTime = {0,0};
//	std::int32_t errorSev;
//
//	::sleep(1);
//
//	getPInterface()->readCSValue(MainNode+"ErrorSrc", &pvWrTime, &errorSource);
//	getPInterface()->readCSValue(MainNode+"ErrorSev", &pvWrTime, &errorSev);
//	getPInterface()->readCSValue(MainNode+"ErrorMsg", &pvWrTime, &errorMessage);
//
//	errorSeverity = std::to_string(errorSev);
//
//	return " :: "+errorSource+" :: "+errorSeverity+" :: "+errorMessage;
//}
//
void writeTempFile(std::string filepath, std::vector<std::vector<float>> matrix, float samplingrate, std::uint32_t initial_channel){
	std::fstream temFile;
	temFile.open(filepath, std::fstream::trunc | std::ios_base::out  );
	if (!temFile) {
		std::cout<<"Error creating plotting temporal file"<<std::endl;
	}
	else {
//		std::cout<<"WRITING FILE WITH "<<matrix.size()<<" CHANNELS"<<std::endl;
		for (unsigned int col = 0; col < matrix.size(); col++) {
			for (unsigned int cell = 0; cell < matrix[col].size(); cell++) {
				temFile << std::to_string(matrix[col][cell])<<",";
			}
			temFile.seekp(-1,std::ios_base::seekdir::_S_end);
			temFile<<std::endl;
			//temFile.flush();
		}
		temFile.close();

//		std::cout<<"This is the filepath: "<<filepath<<std::endl;
		std::string command = "python ./ploter.py "+filepath+" "+std::to_string(samplingrate)+" "+std::to_string(initial_channel);
		std::system(command.c_str());
	}

	if (std::remove(filepath.c_str())){
		std::cout<<"Error deleting plotting temporal file"<<std::endl;
	}
}
//
//
//std::vector<std::vector<float>> TestUtilsIRIO::readDMAPVs(const std::string& DMANode,
//						   std::uint32_t channels,
//						   float samplingRate,
//						   std::uint32_t samples,
//						   std::uint32_t initial_channel,
//						   bool verbose,
//						   std::uint32_t pvPushedVectorSize) {
//	std::vector<float> initSummarizeVals = {-9999, 9999, 0, 0}; // MaxAmplitude = -9999, MinAmplitude =  9999, Negative Cycles, Positive Cycles, Negative Cycle Samples, Positive Cycle Samples
//	std::vector<std::string> SummaryStrings = {"  Vin_max (Vp)", \
//											   "  Vin_min (Vp)", \
//											   " Cycle Samples", \
//											   "  Ampli. (Vpp)"};
//	std::vector<std::vector<float>> summarizeInfo(4, initSummarizeVals);
//	std::vector<bool> currSign(4);
//	std::vector<bool> initSign(4);
//	std::vector<bool> prevSign(4);
//	std::vector<bool> initFlag(4, true);
//	std::vector<float> prevPosCySamples(4,-1.0);
//	std::vector<float> prevNegCySamples(4,-1.0);
//	std::vector<float> posCySamplesCounter(4, 0);
//	std::vector<float> negCySamplesCounter(4, 0);
//	std::vector<float> initPosCySamples(4);
//	std::vector<float> initNegCySamples(4);
//	std::vector<float> posCycles(4, 0);
//	std::vector<float> negCycles(4, 0);
//
//	std::vector<std::vector<float>> matrix(channels);
//
//	unsigned int rigthVectorCounter = 0;
//	for (unsigned j=0;j<channels;j++) {
//		bool dataAcquired = false;
//		while(!dataAcquired) {
//			try {
//				m_pInterface->getPushedInt32(DMANode+"CH"+std::to_string(j+initial_channel)+".numberOfPushedSamples", m_pvTime, m_pvInt32);
//				EXPECT_EQ(pvPushedVectorSize, *m_pvInt32);
//				if(*m_pvInt32==(std::int32_t)pvPushedVectorSize) {
//					dataAcquired = true;
//				}
//				else {
//					::sleep(1);/*give it one second to acquire*/
//				}
//			}
//			catch (const std::runtime_error& re) {
//				::sleep(1);/*give it one second to acquire*/
//			}
//		}
//
//		m_pInterface->getPushedVectorFloat(DMANode+"CH"+std::to_string(j+initial_channel)+".Data", m_pvTime, m_pvVFloat);
//		if (m_pvVFloat->size()==pvPushedVectorSize) {
//			if (verbose) {
//				matrix.at(j) = std::vector<float>(m_pvVFloat->begin(),m_pvVFloat->begin()+samples);
//			}
//			rigthVectorCounter++;
//		}
//	}
//
//	writeTempFile("temp.txt", matrix, samplingRate, initial_channel);
//
//	if (rigthVectorCounter==channels) {
//		matrix.clear();
//	}
//	else {
//		std::cout<<FLRED<<"Error getting vectors"<<std::endl;
//		std::cout<<RST;
//	}
//
//	return summarizeInfo;
//}
//
//
//std::string TestUtilsIRIO::toUpper (const std::string& in_string) {
//	std::string temp_string = in_string;
//	std::for_each(temp_string.begin(), temp_string.end(), [](char & c) {
//		c = ::toupper(c);
//	});
//
//	return temp_string;
//}
