#include <iostream>

#include <cstdlib>
#include <string>
#include <csignal>
#include <thread>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// IRIO Library
extern "C" {
#include <irioDataTypes.h>
#include <irioDriver.h>
#include <irioError.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDMA.h>
#include <irioHandlerDigital.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioResourceFinder.h>
#include <limits.h>
#include <math.h>
}

using std::string;
using std::cout;
using std::cerr;
using std::endl;

const string RST  = "\x1B[0m";   // Foreground color, Reset
const string FCYN = "\x1b[36m";  // Foreground color, Cyan
const string FRED = "\x1b[31m";  // Foreground color, Red

typedef struct {
    int sps;
    float throughput;
    float result;
    bool overflow;
} test_result_t;

string getEnvVar(const string& name);
void initDriver(string bitfile_prefix, irioDrv_t* drv);
void closeDriver(irioDrv_t* drv);
void logErrors(const int ret_status, const TStatus& out_status);
float perfTest(int sps, bool* overflow, float* throughput, bool timeout);
void DMAPerformanceThread(irioDrv_t* drv, int dmaN, bool* finishFlag, long int* blocksRead, int blocksToRead, int bufSize, TStatus* status);
void DMAPerformanceThreadTimeout(irioDrv_t* drv, int dmaN, bool* finishFlag, long int* blocksRead, int blocksToRead,int bufSize, int timeout,  TStatus* status);

// Parameters
float threshold_allowed = 5.0; // %
int timeSleep = 5;
int dmaN = 0;
int blocksToRead = 5;

int main() {
	int verbose_test = std::stoi(getEnvVar("VerboseTest"));
	std::vector<int32_t> samplingRates = {100000,1000000,10000000,20000000,25000000,50000000,100000000};
    std::vector<test_result_t> results;
    std::vector<test_result_t> resultsTimeout;

    for (auto i: samplingRates) {
        test_result_t res;
        res.sps = i;
        res.result = perfTest(res.sps, &res.overflow, &res.throughput, false);
        results.push_back(res);
    }
    for (auto i: samplingRates) {
        test_result_t res;
        res.sps = i;
        res.result = perfTest(res.sps, &res.overflow, &res.throughput, true);
        resultsTimeout.push_back(res);
    }

	if (verbose_test) {
        cout << "[TEST] Tests without timeout: " << endl;
        for (auto res: results) {
            float deviation = 100 * (res.result - res.throughput)/res.throughput;
            cout << endl << "[TEST] Bandwidth expected: " << res.throughput << " MB/s for " << timeSleep << " seconds" << endl
                << "\t[TEST] Bandwidth tested: " << res.result << " MB/s" << endl
                << "\t[TEST] Deviation of " << (deviation > 0 ? "+" : "") << deviation << "%" << endl;
            if (res.overflow) {
                cout << FRED << "\t[TEST] Overflow detected" << RST << endl;
            }
        }
        cout << endl << "[TEST] Tests with timeout: " << endl;
        for (auto res: resultsTimeout) {
            float deviation = 100 * (res.result - res.throughput)/res.throughput;
            cout << endl << "[TEST] Bandwidth expected: " << res.throughput << " MB/s for " << timeSleep << " seconds" << endl
                << "\t[TEST] Bandwidth tested: " << res.result << " MB/s" << endl
                << "\t[TEST] Deviation of " << (deviation > 0 ? "+" : "") << deviation << "%" << endl;
            if (res.overflow) {
                cout << FRED << "\t[TEST] Overflow detected" << RST << endl;
            }
        }
    } else {
        cout << "[TEST] Tests without timeout: " << endl << endl
            << "+--------------+---------------+----------------+------------+----------------+" << endl 
            << "|  Target SPS  |  Target MB/s  |  Measured MB/s |  Overflow  |  Deviation (%) |" << endl
            << "+--------------+---------------+----------------+------------+----------------+" << endl;
        for (auto res: results) {
            float deviation = 100 * (res.result - res.throughput)/res.throughput;
            cout << "| " << std::setw(12) << res.sps << " | " 
                << std::setw(13) << std::fixed << std::setprecision(5) << res.throughput << " | " 
                << std::setw(14) << std::fixed << std::setprecision(5) << res.result     << " | " 
                << (res.overflow ? "   Yes    " : "    No    ")                          << " | " 
                << std::setw(14) << std::fixed << std::setprecision(3) << deviation << " |" << endl;
        }
        cout << "+--------------+---------------+----------------+------------+----------------+" << endl << endl;

        cout << "[TEST] Tests with timeout: " << endl << endl
            << "+--------------+---------------+----------------+------------+----------------+" << endl 
            << "|  Target SPS  |  Target MB/s  |  Measured MB/s |  Overflow  |  Deviation (%) |" << endl
            << "+--------------+---------------+----------------+------------+----------------+" << endl;
        for (auto res: resultsTimeout) {
            float deviation = 100 * (res.result - res.throughput)/res.throughput;
            cout << "| " << std::setw(12) << res.sps << " | " 
                << std::setw(13) << std::fixed << std::setprecision(5) << res.throughput << " | " 
                << std::setw(14) << std::fixed << std::setprecision(5) << res.result     << " | " 
                << (res.overflow ? "   Yes    " : "    No    ")                          << " | " 
                << std::setw(14) << std::fixed << std::setprecision(3) << deviation << " |" << endl;
        }
        cout << "+--------------+---------------+----------------+------------+----------------+" << endl << endl;
    }
}

// Test performance and return throughput and overflow
float perfTest(int sps, bool* overflow, float* throughput, bool timeout) {
	using clock = std::chrono::high_resolution_clock;
	irioDrv_t drv;
	TStatus status;
	int st = IRIO_success;
	irio_initStatus(&status);
	int verbose_test = std::stoi(getEnvVar("VerboseTest"));
	bool finishFlag = false;
	long int blocksRead = 0;

    // Initialize FPGA and DMA
	int32_t fref = -1;
	uint16_t NCh = 0, BlockNWords = 0;
	initDriver(std::string("FlexRIO_perf_"), &drv);
	st |= irio_setFPGAStart(&drv, 1, &status);
	st |= irio_setDebugMode(&drv, 1, &status);
    st |= irio_cleanDMAsTtoHost(&drv, &status);
    st |= irio_setUpDMAsTtoHost(&drv, &status);
    st |= irio_setDMATtoHostEnable(&drv, dmaN, 1, &status);
	st |= irio_getFref(&drv, &fref, &status);
	st |= irio_getDMATtoHOSTNCh(&drv, &NCh, &status);
	st |= irio_getDMATtoHOSTBlockNWords(&drv, &BlockNWords, &status);
    st |= irio_setDMATtoHostSamplingRate(&drv, dmaN, fref/sps, &status);
	if (verbose_test) cout 
        << "[TEST] FPGA Configured " << (st == 0 ? "successfully" : "unsuccessfully") << endl
		<< "[TEST] FPGA Clock Fref = " << fref << " Hz" << endl
		<< "[TEST] DMA has " << NCh << " channels" << endl
		<< "[TEST] Every block has " << BlockNWords << " words per block" << endl;
    logErrors(st, status);
    if (st != IRIO_success) return -1;
    irio_resetStatus(&status);

	// Calculate values
	uint32_t sampleSize = 8 / NCh;
	*throughput = (float)sps * sampleSize * NCh / 1000000;
	int bufSize = blocksToRead * BlockNWords; // * sampleSize * NCh;
    uint32_t tout = ceil((2000 * static_cast<float>(blocksToRead) * BlockNWords) / static_cast<float>(sps));

	auto start = clock::now();

    std::thread thr;
    if (timeout) {
        thr = std::thread(DMAPerformanceThreadTimeout, &drv, dmaN, &finishFlag, &blocksRead, blocksToRead, bufSize, tout, &status);
    } else {
        thr = std::thread(DMAPerformanceThread, &drv, dmaN, &finishFlag, &blocksRead, blocksToRead, bufSize, &status);
    }

    // Start DAQ
	st = irio_setDAQStartStop(&drv, 1, &status);
    logErrors(st, status);
    irio_resetStatus(&status);

	if (verbose_test) cout << "[TEST] Waiting for " << timeSleep << " s" << endl;
	sleep(timeSleep);

    // Stop DAQ
	finishFlag = true;
	thr.join();
	st = irio_setDAQStartStop(&drv, 0, &status);
    logErrors(st, status);
    irio_resetStatus(&status);
	auto end = clock::now();
	auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	float throughput_test = static_cast<float>(blocksRead*BlockNWords*sampleSize*NCh)/us.count();

	// Check for overflow
	int DMAOverflow = -1;
	st = irio_getDMATtoHostOverflow(&drv, &DMAOverflow, &status);
	logErrors(st, status);
    if (st != IRIO_success) return -1;

    *overflow = (DMAOverflow != 0);

    st |= irio_setDMATtoHostEnable(&drv, dmaN, 0, &status);
    st |= irio_closeDMAsTtoHost(&drv, &status);
	
	closeDriver(&drv);
    return throughput_test;
}

string getEnvVar(const string& shellVarName) {
    string shellVarValue;
    try {
        shellVarValue = string(std::getenv(shellVarName.c_str()));
    } catch (...) {
        throw std::logic_error("Shell variables not defined. Please define " +
                               shellVarName +
                               " variable before run this test.");
    }

    return shellVarValue;
}

void initDriver(string bitfile_prefix, irioDrv_t* drv) {
    int st = IRIO_success;

    int verbose_init = std::stoi(getEnvVar("VerboseInit"));
    int verbose_test = std::stoi(getEnvVar("VerboseTest"));
    string RIODevice = getEnvVar("RIODevice");
    string RIOSerial = getEnvVar("RIOSerial");

    string NIRIOmodel = "PXIe-" + RIODevice + "R";
    string bitfileName = bitfile_prefix + RIODevice;
    string filePath = "../resources/" + RIODevice + "/";
    string testName = ("Test_" + bitfileName);

    TStatus status;
    irio_initStatus(&status);

    if (verbose_test) cout << "[TEST] Initializing driver with bitfile \"" << bitfileName << "\"" << endl;
    st = irio_initDriver(testName.c_str(), RIOSerial.c_str(),
                         NIRIOmodel.c_str(), bitfileName.c_str(), "V1.2",
                         verbose_init, filePath.c_str(), filePath.c_str(), drv,
                         &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] Driver initialized " << ((st == IRIO_success) ? "successfully" : "unsuccessfully") << endl;
}

void closeDriver(irioDrv_t* drv) {
    int st = IRIO_success;
    TStatus status;
    irio_initStatus(&status);

    int verbose_test = std::stoi(getEnvVar("VerboseTest"));

    if (verbose_test) cout << "[TEST] Closing driver" << endl;
    st = irio_closeDriver(drv, 0, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] Driver closed " << ((st == IRIO_success) ? "successfully" : "unsuccessfully") << endl;
}

void logErrors(const int ret_status, const TStatus& out_status) {
    if (ret_status == IRIO_success) return;

    char* detailStr = nullptr;
    irio_getErrorString(out_status.detailCode, &detailStr);
    cerr << endl
         << "Runtime error/warning detail code: " << out_status.detailCode
         << ", " << detailStr << endl
         << endl;
    free(detailStr);
    detailStr = nullptr;
}

void DMAPerformanceThread(irioDrv_t* drv, int dmaN, bool* finishFlag, long int* blocksRead, int blocksToRead, int bufSize, TStatus* status) {
    int elementsRead = 0;
    std::vector<uint64_t> buffer(bufSize);

    while (!*finishFlag) {
        if (irio_getDMATtoHostData(drv, blocksToRead, dmaN, buffer.data(), &elementsRead, status) == IRIO_success) {
            if (elementsRead == blocksToRead) {
                *blocksRead += elementsRead;
            } else {
                usleep(5);
            }
        } else {
            logErrors(-1, *status); // -1 to force error logging
        }
    }
}
void DMAPerformanceThreadTimeout(irioDrv_t* drv, int dmaN, bool* finishFlag, long int* blocksRead, int blocksToRead, int bufSize, int timeout, TStatus* status) {
    int elementsRead = 0;
    std::vector<uint64_t> buffer(bufSize);

    while (!*finishFlag) {
        if (irio_getDMATtoHostData_timeout(drv, blocksToRead, dmaN, buffer.data(), &elementsRead, timeout, status) == IRIO_success) {
            *blocksRead += elementsRead;
        } else {
            logErrors(-1, *status); // -1 to force error logging
        }
    }
}