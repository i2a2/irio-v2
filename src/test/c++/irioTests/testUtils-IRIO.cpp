#include <irioDataTypes.h>
#include <irioError.h>
#include <testUtils-IRIO.h>

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

// void TestUtilsIRIO::logErrors(const int ret_status, const TStatus&
// out_status) {

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