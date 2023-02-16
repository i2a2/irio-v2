/*
 * Generated with the FPGA Interface C API Generator 19.0
 * for NI-RIO 19.0 or later.
 */
#ifndef __NiFpga_Rseries_CPUDAQ_7854_h__
#define __NiFpga_Rseries_CPUDAQ_7854_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 190
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Rseries_CPUDAQ_7854_Bitfile;
 */
#define NiFpga_Rseries_CPUDAQ_7854_Bitfile "NiFpga_Rseries_CPUDAQ_7854.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Rseries_CPUDAQ_7854_Signature = "7E0C3DE1C4F35F1306B49AEFEBF6547F";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_DI0 = 0x8162,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_DI1 = 0x815E,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_InitDone = 0x81E2,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_PXIIN0 = 0x814E,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_PXIIN1 = 0x8146,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_RIOAdapterCorrect = 0x81DA,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_auxDI0 = 0x815A,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorBool_auxDI1 = 0x8156,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorBool;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU8_DevProfile = 0x81CA,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU8_DevQualityStatus = 0x81D2,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU8_Platform = 0x81BE,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU8_SGNo = 0x8142,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorU8;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorI16_DevTemp = 0x81CE,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorI16;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU16_DMATtoHOSTOverflows = 0x81A6,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorU16;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorI32_AI0 = 0x8198,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorI32_AI1 = 0x8194,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorI32_auxAI0 = 0x8190,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorI32_auxAI1 = 0x818C,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorI32;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU32_Fref = 0x81D4,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU32_InsertedIOModuleID = 0x81DC,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU32_SGFref0 = 0x8128,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorU32_SGFref1 = 0x8110,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorU32;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_AOEnable0 = 0x817A,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_AOEnable1 = 0x8176,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_DAQStartStop = 0x81C6,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_DMATtoHOSTEnable0 = 0x81AE,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_DMATtoHOSTEnable1 = 0x819E,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_DO0 = 0x8172,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_DO1 = 0x816E,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_DebugMode = 0x81C2,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_PXIOUT0 = 0x8152,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_PXIOUT1 = 0x814A,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_auxDO0 = 0x816A,
   NiFpga_Rseries_CPUDAQ_7854_ControlBool_auxDO1 = 0x8166,
} NiFpga_Rseries_CPUDAQ_7854_ControlBool;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_ControlU8_SGSignalType0 = 0x812E,
   NiFpga_Rseries_CPUDAQ_7854_ControlU8_SGSignalType1 = 0x8116,
} NiFpga_Rseries_CPUDAQ_7854_ControlU8;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_ControlU16_DMATtoHOSTSamplingRate0 = 0x81AA,
   NiFpga_Rseries_CPUDAQ_7854_ControlU16_DMATtoHOSTSamplingRate1 = 0x81A2,
   NiFpga_Rseries_CPUDAQ_7854_ControlU16_SGAmp0 = 0x813E,
   NiFpga_Rseries_CPUDAQ_7854_ControlU16_SGAmp1 = 0x8126,
} NiFpga_Rseries_CPUDAQ_7854_ControlU16;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_ControlI32_AO0 = 0x8188,
   NiFpga_Rseries_CPUDAQ_7854_ControlI32_AO1 = 0x8184,
   NiFpga_Rseries_CPUDAQ_7854_ControlI32_auxAO0 = 0x8180,
   NiFpga_Rseries_CPUDAQ_7854_ControlI32_auxAO1 = 0x817C,
} NiFpga_Rseries_CPUDAQ_7854_ControlI32;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_ControlU32_SGFreq0 = 0x8138,
   NiFpga_Rseries_CPUDAQ_7854_ControlU32_SGFreq1 = 0x8120,
   NiFpga_Rseries_CPUDAQ_7854_ControlU32_SGPhase0 = 0x8134,
   NiFpga_Rseries_CPUDAQ_7854_ControlU32_SGPhase1 = 0x811C,
   NiFpga_Rseries_CPUDAQ_7854_ControlU32_SGUpdateRate0 = 0x8130,
   NiFpga_Rseries_CPUDAQ_7854_ControlU32_SGUpdateRate1 = 0x8118,
} NiFpga_Rseries_CPUDAQ_7854_ControlU32;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8_DMATtoHOSTFrameType = 0x81B6,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x81B2,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8_FPGAVIversion = 0x81E6,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8Size_DMATtoHOSTFrameType = 2,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 2,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x810C,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU16_DMATtoHOSTNCh = 0x81B8,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU16;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 2,
   NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU16Size_DMATtoHOSTNCh = 2,
} NiFpga_Rseries_CPUDAQ_7854_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_Rseries_CPUDAQ_7854_TargetToHostFifoU64_DMATtoHOST0 = 1,
   NiFpga_Rseries_CPUDAQ_7854_TargetToHostFifoU64_DMATtoHOST1 = 0,
} NiFpga_Rseries_CPUDAQ_7854_TargetToHostFifoU64;


#if NiFpga_Cpp
}
#endif

#endif
