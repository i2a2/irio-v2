#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <fstream>
#include <stdio.h>
#include <gtest/gtest.h>
#include <type_traits>
#include <functional>
#include <algorithm>

#define RST    "\x1B[0m"	//Foreground color, Reset
#define FBLA   "\x1b[30m"	//Foreground color, Black
#define FRED   "\x1b[31m"	//Foreground color, Red
#define FGRN   "\x1b[32m"	//Foreground color, Green
#define FYEL   "\x1b[33m"	//Foreground color, Yellow
#define FBLU   "\x1b[34m"	//Foreground color, Blue
#define FMAG   "\x1b[35m"	//Foreground color, Magenta
#define FCYN   "\x1b[36m"	//Foreground color, Cyan
#define FWHT   "\x1b[37m"	//Foreground color, White
#define FDEF   "\x1b[39m"	//Foreground color, Default(foreground color at startup)
#define FLGRA  "\x1b[90m"	//Foreground color, Light Gray
#define FLRED  "\x1b[91m"	//Foreground color, Light Red
#define FLGRN  "\x1b[92m"	//Foreground color, Light Green
#define FLYEL  "\x1b[93m"	//Foreground color, Light Yellow
#define FLBLU  "\x1b[94m"	//Foreground color, Light Blue
#define FLMAG  "\x1b[95m"	//Foreground color, Light Magenta
#define FLCYN  "\x1b[96m"	//Foreground color, Light Cyan
#define FLWHT  "\x1b[97m"	//Foreground color, Light White

#define BBLA   "\x1b[40m"	//Background color, Black
#define BRED   "\x1b[41m"	//Background color, Red
#define BGRN   "\x1b[42m"	//Background color, Green
#define BYEL   "\x1b[43m"	//Background color, Yellow
#define BBLU   "\x1b[44m"	//Background color, Blue
#define BMAG   "\x1b[45m"	//Background color, Magenta
#define BCYN   "\x1b[46m"	//Background color, Cyan
#define BWHT   "\x1b[47m"	//Background color, White
#define BDEF   "\x1b[49m"	//Background color, Default(background color at startup)
#define BLGRA  "\x1b[100m"	//Background color, Light Gray
#define BLRED  "\x1b[101m"	//Background color, Light Red
#define BLGRN  "\x1b[102m"	//Background color, Light Green
#define BLYEL  "\x1b[103m"	//Background color, Light Yellow
#define BLBLU  "\x1b[104m"	//Background color, Light Blue
#define BLMAG  "\x1b[105m"	//Background color, Light Magenta
#define BLCYN  "\x1b[106m"	//Background color, Light Cyan
#define BLWHT  "\x1b[107m"	//Background color, Light White

class TestUtilsIRIO
{
public:

	TestUtilsIRIO();
	TestUtilsIRIO(const std::string& device_name);

	void setInferface(const std::string& device_name);
	const std::string& getDeviceName() const { return m_deviceName; }
private:
//	nds::tests::TestControlSystemInterfaceImpl* m_pInterface = NULL;
	std::string m_deviceName;
	const timespec* m_pvTime = NULL;

	void * m_pvTypePtr = NULL;
	const std::int32_t* m_pvInt32 = NULL;
	const float* m_pvFloat = NULL;
	const double* m_pvDouble = NULL;
	const std::string* m_pvString = NULL;
	const timespec* m_pvTimespec = NULL;

	const std::vector<bool>* m_pvVBool = NULL;
	const std::vector<std::uint8_t>* m_pvVUInt8 = NULL;
	const std::vector<std::uint16_t>* m_pvVUInt16 = NULL;
	const std::vector<std::uint32_t>* m_pvVUInt32 = NULL;
	const std::vector<std::int8_t>* m_pvVInt8 = NULL;
	const std::vector<std::int16_t>* m_pvVInt16 = NULL;
	const std::vector<std::int32_t>* m_pvVInt32 = NULL;
	const std::vector<float>* m_pvVFloat = NULL;
	const std::vector<double>* m_pvVDouble = NULL;
	const std::vector<timespec>* m_pvVTimespec = NULL;

	const void *m_pvValue = NULL;

	timespec m_timestamp = {0, 0};
	std::vector<std::string> STM_StatesStr = {"UNKNOWN", "OFF", "SWITCHINGOFF", "INITIALIZING", "ON", "STOPPING", "STARTING", "RUNNING", "FAULT", "MAX_STATE_NUM"};


public:
//	nds::tests::TestControlSystemInterfaceImpl* getPInterface();

	void STM_SwitchON(std::string nodeStr, const std::int32_t transitionTimeout=10000);
	void STM_SwitchRUN(std::string nodeStr, const std::int32_t transitionTimeout=10000);
	void STM_SwitchSTOP(std::string nodeStr, const std::int32_t transitionTimeout=10000);
	void STM_SwitchOFF(std::string nodeStr, const std::int32_t transitionTimeout=10000);
	void STM_SetState(std::string nodeStr, std::int32_t transitionSt, std::int32_t finalSt, const std::int32_t transitionTimeout=10000);
	void STM_CheckState(std::string nodeStr, std::int32_t currentState, const std::int32_t timeout=10000);

	std::string getErrorPVs(const std::string& MainNode);

	/*
	 * This function gets 'samples' samples for every channel and displays the result.
	 */
	std::vector<std::vector<float>> readDMAPVs(
			const std::string& DMANode,
			std::uint32_t channels,
			float samplingRate,
			std::uint32_t samples = 10,
			std::uint32_t initial_channel=0,
			bool verbose = true,
			std::uint32_t pvPushedVectorSize = 4096);

public:
	static std::string toUpper (const std::string & in_string);

	/**
	 * @brief Get a string by identifying the type of data (std::int32_t) and its value
	 */
	static std::string getString(const std::int32_t & data);

	/**
	 * @brief Get a string by identifying the type of data (double) and its value
	 */
	static std::string getString(const double & data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<bool> data) and its value
	 */
	static std::string getString(std::vector<bool> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint8_t>) and its value
	 */
	static std::string getString(std::vector<std::uint8_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint16_t>) and its value
	 */
	static std::string getString(std::vector<std::uint16_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint32_t>) and its value
	 */
	static std::string getString(std::vector<std::uint32_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int8_t>) and its value
	 */
	static std::string getString(std::vector<std::int8_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int16_t>) and its value
	 */
	static std::string getString(std::vector<std::int16_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int32_t>) and its value
	 */
	static std::string getString(std::vector<std::int32_t> data);

	/**
	 * @brief Get a string by identifying the type of data (double) and its value
	 */
	static std::string getString(std::vector<double> data);

	/**
	 * @brief Get a string by identifying the type of data (std::string) and its value
	 */
	static std::string getString(std:: string data);

	/**
	 * @brief Get a string by identifying the type of data (timespec data) and its value
	 */
	static std::string getString(timespec data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<timespec> data) and its value
	 */
	static std::string getString(std::vector<timespec> data);

	/**
	 * @brief Get a string by identifying the type of data (nds::timestamp_t) and its value
	 */
//	static std::string getString(nds::timestamp_t data);

	static void displayTitle(const std::string& msg, const std::string& forecolor=FWHT, const std::string& backcolor=BBLA) {
		std::cout<<forecolor<<backcolor<<"+-----------------------------------------------------------------------------"<<std::endl;
		std::cout<<msg<<std::endl;
		std::cout<<"+-----------------------------------------------------------------------------"<<std::endl<<RST<<std::endl;
	}

	static void waitEnterKey(const std::string& msg, const std::string& forecolor=FWHT, const std::string& backcolor=BBLA) {
		std::cout<<forecolor<<backcolor<<"+-----------------------------------------------------------------------------"<<std::endl;
		std::cout<<msg<<std::endl;
		std::cout<<"+-----------------------------------------------------------------------------"<<std::endl<<RST;

		while(std::cin.get()!='\n');
	}

	static std::string AlignStr(std::string text, unsigned int length, unsigned int aligment_type=1) {
		if (text.size() > length) {
			throw std::logic_error("String length must be, at least, the same of the input text");
		}
		else if (text.size() < length) {
			unsigned int total_spaces = length - text.size();

			switch (aligment_type) {
				case 0: // Centre
					return std::string(total_spaces-(total_spaces/2), ' ')+text+std::string(total_spaces/2, ' ');
				case 1: // Right
					return std::string(total_spaces, ' ')+text;
				default:// Left
					return text+std::string(total_spaces, ' ');
			}
		}
		else {
			return text;
		}
	}

	static std::string getEnvVar(const std::string& shellVarName) {
		std::string shellVarValue;
		try {
			shellVarValue=std::string(std::getenv(shellVarName.c_str()));
		}
		catch(...) {
			throw std::logic_error("Shell variables not defined. Please define "+shellVarName+" variable before run this test.");
		}

		return shellVarValue;
	}

};

#endif /* TESTUTILS_H_ */
