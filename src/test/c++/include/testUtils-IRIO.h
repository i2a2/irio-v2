#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <gtest/gtest.h>
#include <irioDataTypes.h>

using std::string;

namespace TestUtilsIRIO {

const string FWHT = "\x1b[37m";  // Foreground color, White
const string BBLA = "\x1b[40m";  // Background color, Black
const string RST  = "\x1B[0m";   // Foreground color, Reset
const string FCYN = "\x1b[36m";  // Foreground color, Cyan
const string FRED = "\x1b[31m";  // Foreground color, Red

void displayTitle(const string& msg, const string& forecolor = FWHT,
                  const string& backcolor = BBLA);

string getEnvVar(const string& shellVarName);

void logErrors(const int ret_status, const TStatus& out_status);

void initDriver(string bitfile_prefix, irioDrv_t* drv);
void closeDriver(irioDrv_t* drv);
int  loadHeaderFile(irioDrv_t* drv, string file_path, TStatus* status);
void freeHeaderFile(irioDrv_t* drv);
void startFPGA(irioDrv_t* drv);
void setDebugMode(irioDrv_t* drv, int debug_mode);
TIRIOCouplingMode setAICoupling(irioDrv_t* drv);

typedef struct {
    int AI, AO, auxAI, auxAO, DI, DO, auxDI, auxDO, SG, DMA, CLConfig, CLUART;
} irioResources_t;

void getResources(irioDrv_t* drv, irioResources_t* res);

namespace DMAHost {
    void cleanDMA(irioDrv_t* drv);
    void setupDMA(irioDrv_t* drv);
    int  setSamplingRate(irioDrv_t* drv, int32_t sampling_rate);
    void setEnable(irioDrv_t* drv, int channel, int enable);
    void setStartStop(irioDrv_t* drv, int startstop);
}  // namespace DMAHost
}  // namespace TestUtilsIRIO

#endif /* TESTUTILS_H_ */
