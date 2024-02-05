#include <irioDataTypes.h>
#include <irioError.h>
#include <testUtils-IRIO.h>
#include <memory>

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

using std::cerr;
using std::cout;
using std::endl;
using std::string;

static int getResourceCount(TResourcePort* arr, int max);

void TestUtilsIRIO::displayTitle(const string& msg, const string& forecolor,
                                 const string& backcolor) {
    cout << forecolor << backcolor
         << "+-------------------------------------------------------------"
            "----------------"
         << endl;
    cout << msg << endl;
    cout << "+-------------------------------------------------------------"
            "----------------"
         << endl
         << RST << endl;
}

string TestUtilsIRIO::getEnvVar(const string& shellVarName) {
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

void TestUtilsIRIO::logErrors(const int ret_status, const TStatus& out_status) {
    if (ret_status == IRIO_success) return;

    char* detailStr = nullptr;
    irio_getErrorString(out_status.detailCode, &detailStr);
    cerr << endl
         << TestUtilsIRIO::FRED
         << "Runtime error/warning detail code: " << out_status.detailCode
         << ", " << detailStr << TestUtilsIRIO::RST << endl
         << endl;
    free(detailStr);
    detailStr = nullptr;
}

void TestUtilsIRIO::initDriver(string bitfile_prefix, irioDrv_t* drv) {
    int st = IRIO_success;

    int verbose_init = std::stoi(TestUtilsIRIO::getEnvVar("VerboseInit"));
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    string RIODevice = TestUtilsIRIO::getEnvVar("RIODevice");
    string RIOSerial = TestUtilsIRIO::getEnvVar("RIOSerial");

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
    TestUtilsIRIO::logErrors(st, status);
    EXPECT_EQ(st, IRIO_success);
    if (verbose_test) cout << "[TEST] Driver initialized " << ((st == IRIO_success) ? "successfully" : "unsuccessfully") << endl;
}

void TestUtilsIRIO::closeDriver(irioDrv_t* drv) {
    int st = IRIO_success;
    TStatus status;
    irio_initStatus(&status);

    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    if (verbose_test) cout << "[TEST] Closing driver" << endl;
    st = irio_closeDriver(drv, 0, &status);
    TestUtilsIRIO::logErrors(st, status);
    ASSERT_EQ(st, IRIO_success);
    if (verbose_test) cout << "[TEST] Driver closed " << ((st == IRIO_success) ? "successfully" : "unsuccessfully") << endl;
}

static int getResourceCount(TResourcePort* arr, int max) {
    int cnt = 0;
    for (int i = 0; i < max; i++) 
        if (arr[i].found)
            cnt++;

    return cnt;
}

void TestUtilsIRIO::getResources(irioDrv_t* drv, irioResources_t* res) {
	res->AI = getResourceCount(drv->enumAnalogInput, drv->max_analoginputs);
	res->AO = getResourceCount(drv->enumAnalogOutput, drv->max_analogoutputs);
	res->auxAI = getResourceCount(drv->enumauxAI, drv->max_auxanaloginputs);
	res->auxAO = getResourceCount(drv->enumauxAO, drv->max_auxanalogoutputs);

	res->DI = getResourceCount(drv->enumDigitalInput, drv->max_digitalsinputs);
	res->DO = getResourceCount(drv->enumDigitalOutput, drv->max_digitalsoutputs);
	res->auxDI = getResourceCount(drv->enumauxDI, drv->max_auxdigitalsinputs);
	res->auxDO = getResourceCount(drv->enumauxDO, drv->max_auxdigitalsoutputs);

    res->DMA = getResourceCount(drv->enumDMATtoHOST, drv->max_dmas) + getResourceCount(drv->enumDMATtoGPU, drv->max_dmas_gpu);

	res->SG = drv->NoOfSG;

	res->CLConfig = drv->enumConfiguration.found;
	res->CLUART = drv->enumuartByteMode.found;
}

int TestUtilsIRIO::loadHeaderFile(irioDrv_t* drv, string file_path, TStatus* status) {
	char* headerPath=NULL;
    int local_status = IRIO_success;

	if (file_path[file_path.length() - 1] != '/')
		asprintf(&headerPath,"%s/%s%s%s",file_path.c_str(),STRINGNAME_PREFIX,drv->projectName,".h");
	else
		asprintf(&headerPath,"%s%s%s%s",file_path.c_str(),STRINGNAME_PREFIX,drv->projectName,".h");

	//Call for file init
	local_status |= irio_initFileSearch(drv,headerPath,(void**)&drv->headerFile,status);
	if(status->detailCode==FileNotFound_Error){
		status->detailCode=HeaderNotFound_Error;
	}
	free(headerPath);
    return local_status;
}

void TestUtilsIRIO::freeHeaderFile(irioDrv_t* drv) {
    irio_closeFileSearch(drv, (void**)&drv->headerFile, NULL);
}

void TestUtilsIRIO::startFPGA(irioDrv_t* drv) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    if (verbose_test) cout << "[TEST] Starting FPGA" << endl;
	TStatus status;
	irio_initStatus(&status);
	int startStatus = irio_setFPGAStart(drv,1,&status);
	TestUtilsIRIO::logErrors(startStatus, status);
    if (verbose_test) cout << "[TEST] FPGA started " << (startStatus ? "unsuccessfully" : "successfully") << endl;
	EXPECT_EQ(startStatus, IRIO_success);
}

void TestUtilsIRIO::setDebugMode(irioDrv_t* drv, int debug_mode) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
	TStatus status;
	irio_initStatus(&status);

	if (verbose_test) cout << "[TEST] Setting debug mode to 0" << endl;
	int st = irio_setDebugMode(drv, 0, &status);
	if (verbose_test) cout << "[TEST] Debug mode set " << (st ? "unsuccessfully" : "successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
}

void TestUtilsIRIO::DMAHost::cleanDMA(irioDrv_t* drv) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    TStatus status;
    irio_initStatus(&status);

	if (verbose_test) cout << "[TEST] Cleaning DMAs" << endl;
	int st = irio_cleanDMAsTtoHost(drv, &status);
	if (verbose_test) cout << "[TEST] DMAs cleaned " << (st ? "unsuccessfully" : "successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
}

void TestUtilsIRIO::DMAHost::setupDMA(irioDrv_t* drv) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    TStatus status;
    irio_initStatus(&status);

	if (verbose_test) cout << "[TEST] Setting up DMAs to host" << endl;
	int st = irio_setUpDMAsTtoHost(drv, &status);
	if (verbose_test) cout << "[TEST] DMAs set up " << (st ? "unsuccessfully" : "successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
}

int TestUtilsIRIO::DMAHost::setSamplingRate(irioDrv_t* drv, int32_t sampling_rate) {
	// Equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    int st;
    int32_t fref;
    TStatus status;
    irio_initStatus(&status);

	st = irio_getFref(drv, &fref, &status);
	if (verbose_test) cout << "[TEST] Fref = " << fref << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_NE(fref, 0);
	irio_resetStatus(&status);

	int32_t decimation_factor = fref/sampling_rate;
	if (verbose_test) cout << "[TEST] Setting decimation factor of DMA0 to Fref/SamplingRate = " << decimation_factor << endl;
	st = irio_setDMATtoHostSamplingRate(drv, 0, decimation_factor, &status);
	if (verbose_test) cout << "[TEST] Sampling rate set " << (st ? "unsuccessfully" : "successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

    return fref;
}

TIRIOCouplingMode TestUtilsIRIO::setAICoupling(irioDrv_t* drv) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    string couplingStr = TestUtilsIRIO::getEnvVar("VerboseTest");
    TStatus status;
    irio_initStatus(&status);

    TIRIOCouplingMode coupling = static_cast<TIRIOCouplingMode>(couplingStr == "1" || couplingStr == "DC");

    if (verbose_test) cout << "[TEST] Setting the AI Coupling to " << coupling << (coupling ? " (DC)" : " (AC)") << endl;
    int st = irio_setAICoupling(drv, coupling, &status);
	if (verbose_test) cout << "[TEST] AI coupling set " << (st ? "unsuccessfully" : "successfully") << endl;
    TestUtilsIRIO::logErrors(st, status);
    EXPECT_EQ(st, IRIO_success);

    return coupling;
}

void TestUtilsIRIO::DMAHost::setEnable(irioDrv_t* drv, int channel, int enable) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    TStatus status;
    irio_initStatus(&status);

	if (verbose_test) cout << "[TEST] Setting DMA" << channel << " enable to " << enable << endl;
	int st = irio_setDMATtoHostEnable(drv, channel, enable, &status);
	if (verbose_test) cout << "[TEST] DMA enable set up " << (st ? "unsuccessfully" : "successfully") << endl;
    TestUtilsIRIO::logErrors(st, status);
    EXPECT_EQ(st, IRIO_success);
}

void TestUtilsIRIO::DMAHost::setDAQStartStop(irioDrv_t* drv, int startstop) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    TStatus status;
    irio_initStatus(&status);

	if (verbose_test) cout << "[TEST] Setting DAQStartStop" << " to " << startstop << (startstop ? " (ON)" : " (OFF)")<< endl;
	int st = irio_setDAQStartStop(drv, startstop, &status);
	if (verbose_test) cout << "[TEST] DAQStartStop set " << (st ? "unsuccessfully" : "successfully") << endl;
    TestUtilsIRIO::logErrors(st, status);
    EXPECT_EQ(st, IRIO_success);
}

std::vector<uint64_t> TestUtilsIRIO::DMAHost::readDMAData(irioDrv_t* drv, int dmaN, int blocksToRead, int wordsPerBlock, int sampling_freq) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    if (verbose_test) cout << "[TEST] Reading " << blocksToRead << " blocks of " << wordsPerBlock << " words each from DMA" << dmaN << endl; 
    TStatus status;
    irio_initStatus(&status);

	int blocksRead = 0;
	int tries = 0, maxTries = 10;

    // Buffer = blocks * (words/block) * (bytes/word)
    std::vector<uint64_t> dataBuffer(blocksToRead * wordsPerBlock);

    while (tries < maxTries) {
        int st = irio_getDMATtoHostData(drv, blocksToRead, dmaN, dataBuffer.data(), &blocksRead, &status);
        TestUtilsIRIO::logErrors(st, status);
        if (verbose_test) cout << "[TEST] " << blocksRead << " blocks read" << (st ? " unsuccessfully" : " successfully") << endl; 
        EXPECT_EQ(st, IRIO_success);
        irio_resetStatus(&status);

        if (blocksRead == blocksToRead) {
            return dataBuffer;
        } else {
            int time = wordsPerBlock * 500000/sampling_freq;
            if (verbose_test) cout << "[TEST] Read failed. Waiting " << time << " us." << endl;
            usleep(time); // Wait Half block duration
        }
        ++tries;
        if (verbose_test) cout << "[TEST] DMA read try " << tries << " failed." << endl; 
    }
    ADD_FAILURE() << "[ERROR] No blocks read after " << maxTries << " tries"; 
    return dataBuffer;
}

std::vector<uint64_t> TestUtilsIRIO::DMAHost::readDMADataTimeout(irioDrv_t* drv, int dmaN, int blocksToRead, int wordsPerBlock, int sampling_freq) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    if (verbose_test) cout << "[TEST] Reading " << blocksToRead << " blocks of " << wordsPerBlock << " words each from DMA" << dmaN << endl; 
    TStatus status;
    irio_initStatus(&status);

	int blocksRead = 0;
	int tries = 0, maxTries = 10;

    float timePerWord = 1/((float)sampling_freq) * 1000;
    uint32_t timeout = ceil(timePerWord*blocksToRead*wordsPerBlock);

    // Buffer = blocks * (words/block) * (bytes/word)
    std::vector<uint64_t> dataBuffer(blocksToRead * wordsPerBlock);

    while (tries < maxTries) {
        int st = irio_getDMATtoHostData_timeout(drv, blocksToRead, dmaN, dataBuffer.data(), &blocksRead, timeout, &status);
        TestUtilsIRIO::logErrors(st, status);
        if (verbose_test) cout << "[TEST] " << blocksRead << " blocks read" << (st ? " unsuccessfully" : " successfully") << endl; 
        EXPECT_EQ(st, IRIO_success);
        irio_resetStatus(&status);

        if (blocksRead == blocksToRead) {
            return dataBuffer;
        }         ++tries;
        if (verbose_test) cout << "[TEST] DMA read try " << tries << " failed." << endl; 
    }
    ADD_FAILURE() << "[ERROR] No blocks read after " << maxTries << " tries"; 
    return dataBuffer;
}

void TestUtilsIRIO::SG::setUpdateRate(irioDrv_t* drv, int channel, int32_t update_rate) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
	uint32_t SGFref = 0;
    TStatus status;
    irio_initStatus(&status);

	int st = irio_getSGFref(drv, channel, &SGFref, &status);
    logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) cout << "[TEST] Read SG FRef = " << SGFref << " Hz" << endl;
	// Equation applied to set SGUpdateRate: SGUpdateRate=(SGFref/(Samples/s))
	if (verbose_test) cout << "[TEST] Setting SGUpdateRate" << channel << " to " << SGFref/update_rate
		 << ", meaning " << update_rate << " Samples/s" << endl;
	st = irio_setSGUpdateRate(drv, channel, SGFref/update_rate, &status);
    if (verbose_test) cout << "[TEST] SGUpdateRate" << channel << " set " << (st ? "unsuccessfully" : "successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
}

void TestUtilsIRIO::SG::setSignalType(irioDrv_t* drv, int channel, int signal_type) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    TStatus status;
    irio_initStatus(&status);

	if (verbose_test) cout << "[TEST] Settings SGSignalType" << channel << " to " << signal_type << endl;
	int st = irio_setSGSignalType(drv, channel, signal_type, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) cout << "[TEST] SGSignalType" << channel << " set " << (st ? "unsuccessfully" : "successfully") << endl;
	irio_resetStatus(&status);
}