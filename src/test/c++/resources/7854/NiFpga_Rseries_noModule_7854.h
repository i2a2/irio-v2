/*
 * Generated with the FPGA Interface C API Generator 19.0
 * for NI-RIO 19.0 or later.
 */
#ifndef __NiFpga_Rseries_noModule_7854_h__
#define __NiFpga_Rseries_noModule_7854_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 190
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Rseries_noModule_7854_Bitfile;
 */
#define NiFpga_Rseries_noModule_7854_Bitfile "NiFpga_Rseries_noModule_7854.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Rseries_noModule_7854_Signature = "4C2C7679B0802C4296CE92FDE0DB9E91";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorBool_InitDone = 0x81FA,
   NiFpga_Rseries_noModule_7854_IndicatorBool_RIOAdapterCorrect = 0x81FE,
   NiFpga_Rseries_noModule_7854_IndicatorBool_auxDI0 = 0x8146,
   NiFpga_Rseries_noModule_7854_IndicatorBool_auxDI1 = 0x8142,
   NiFpga_Rseries_noModule_7854_IndicatorBool_auxDI2 = 0x813E,
   NiFpga_Rseries_noModule_7854_IndicatorBool_auxDI3 = 0x813A,
   NiFpga_Rseries_noModule_7854_IndicatorBool_auxDI4 = 0x8136,
   NiFpga_Rseries_noModule_7854_IndicatorBool_auxDI5 = 0x8132,
} NiFpga_Rseries_noModule_7854_IndicatorBool;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorU8_DevProfile = 0x81EE,
   NiFpga_Rseries_noModule_7854_IndicatorU8_DevQualityStatus = 0x8212,
   NiFpga_Rseries_noModule_7854_IndicatorU8_Platform = 0x8216,
   NiFpga_Rseries_noModule_7854_IndicatorU8_SGNo = 0x8112,
} NiFpga_Rseries_noModule_7854_IndicatorU8;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorI16_DevTemp2 = 0x821A,
} NiFpga_Rseries_noModule_7854_IndicatorI16;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorU16_DMATtoHOSTOverflows = 0x81DE,
} NiFpga_Rseries_noModule_7854_IndicatorU16;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI0 = 0x8174,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI1 = 0x8170,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI2 = 0x816C,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI3 = 0x8168,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI4 = 0x8164,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI5 = 0x8160,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI6 = 0x8190,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI7 = 0x8194,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI8 = 0x8198,
   NiFpga_Rseries_noModule_7854_IndicatorI32_auxAI9 = 0x819C,
} NiFpga_Rseries_noModule_7854_IndicatorI32;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorU32_ExpectedIOModuleID = 0x8204,
   NiFpga_Rseries_noModule_7854_IndicatorU32_Fref = 0x81F4,
   NiFpga_Rseries_noModule_7854_IndicatorU32_InsertedIOModuleID = 0x8200,
   NiFpga_Rseries_noModule_7854_IndicatorU32_SGFref0 = 0x810C,
} NiFpga_Rseries_noModule_7854_IndicatorU32;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorI64_aux64AI0 = 0x81B8,
   NiFpga_Rseries_noModule_7854_IndicatorI64_aux64AI1 = 0x81BC,
   NiFpga_Rseries_noModule_7854_IndicatorI64_aux64AI2 = 0x81C0,
   NiFpga_Rseries_noModule_7854_IndicatorI64_aux64AI3 = 0x81C4,
   NiFpga_Rseries_noModule_7854_IndicatorI64_aux64AI4 = 0x81C8,
   NiFpga_Rseries_noModule_7854_IndicatorI64_aux64AI5 = 0x81CC,
} NiFpga_Rseries_noModule_7854_IndicatorI64;

typedef enum
{
   NiFpga_Rseries_noModule_7854_ControlBool_AOEnable0 = 0x8116,
   NiFpga_Rseries_noModule_7854_ControlBool_DAQStartStop = 0x820A,
   NiFpga_Rseries_noModule_7854_ControlBool_DMATtoHOSTEnable0 = 0x81D6,
   NiFpga_Rseries_noModule_7854_ControlBool_DebugMode = 0x820E,
   NiFpga_Rseries_noModule_7854_ControlBool_auxDO0 = 0x815E,
   NiFpga_Rseries_noModule_7854_ControlBool_auxDO1 = 0x815A,
   NiFpga_Rseries_noModule_7854_ControlBool_auxDO2 = 0x8156,
   NiFpga_Rseries_noModule_7854_ControlBool_auxDO3 = 0x814A,
   NiFpga_Rseries_noModule_7854_ControlBool_auxDO4 = 0x814E,
   NiFpga_Rseries_noModule_7854_ControlBool_auxDO5 = 0x8152,
} NiFpga_Rseries_noModule_7854_ControlBool;

typedef enum
{
   NiFpga_Rseries_noModule_7854_ControlU8_SGSignalType0 = 0x811E,
} NiFpga_Rseries_noModule_7854_ControlU8;

typedef enum
{
   NiFpga_Rseries_noModule_7854_ControlU16_DMATtoHOSTSamplingRate0 = 0x81DA,
   NiFpga_Rseries_noModule_7854_ControlU16_SGAmp0 = 0x812E,
} NiFpga_Rseries_noModule_7854_ControlU16;

typedef enum
{
   NiFpga_Rseries_noModule_7854_ControlI32_AO0 = 0x8120,
   NiFpga_Rseries_noModule_7854_ControlI32_auxAO0 = 0x818C,
   NiFpga_Rseries_noModule_7854_ControlI32_auxAO1 = 0x8188,
   NiFpga_Rseries_noModule_7854_ControlI32_auxAO2 = 0x8184,
   NiFpga_Rseries_noModule_7854_ControlI32_auxAO3 = 0x8180,
   NiFpga_Rseries_noModule_7854_ControlI32_auxAO4 = 0x817C,
   NiFpga_Rseries_noModule_7854_ControlI32_auxAO5 = 0x8178,
} NiFpga_Rseries_noModule_7854_ControlI32;

typedef enum
{
   NiFpga_Rseries_noModule_7854_ControlU32_SGFreq0 = 0x8128,
   NiFpga_Rseries_noModule_7854_ControlU32_SGPhase0 = 0x8124,
   NiFpga_Rseries_noModule_7854_ControlU32_SGUpdateRate0 = 0x8118,
   NiFpga_Rseries_noModule_7854_ControlU32_TabControl = 0x8220,
} NiFpga_Rseries_noModule_7854_ControlU32;

typedef enum
{
   NiFpga_Rseries_noModule_7854_ControlI64_aux64AO0 = 0x81A0,
   NiFpga_Rseries_noModule_7854_ControlI64_aux64AO1 = 0x81A4,
   NiFpga_Rseries_noModule_7854_ControlI64_aux64AO2 = 0x81A8,
   NiFpga_Rseries_noModule_7854_ControlI64_aux64AO3 = 0x81AC,
   NiFpga_Rseries_noModule_7854_ControlI64_aux64AO4 = 0x81B0,
   NiFpga_Rseries_noModule_7854_ControlI64_aux64AO5 = 0x81B4,
} NiFpga_Rseries_noModule_7854_ControlI64;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorArrayU8_DMATtoHOSTFrameType = 0x81E2,
   NiFpga_Rseries_noModule_7854_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x81E6,
   NiFpga_Rseries_noModule_7854_IndicatorArrayU8_FPGAVIversion = 0x81F2,
} NiFpga_Rseries_noModule_7854_IndicatorArrayU8;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_Rseries_noModule_7854_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_Rseries_noModule_7854_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_Rseries_noModule_7854_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x81EA,
   NiFpga_Rseries_noModule_7854_IndicatorArrayU16_DMATtoHOSTNCh = 0x81D2,
} NiFpga_Rseries_noModule_7854_IndicatorArrayU16;

typedef enum
{
   NiFpga_Rseries_noModule_7854_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_Rseries_noModule_7854_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_Rseries_noModule_7854_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_Rseries_noModule_7854_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_Rseries_noModule_7854_TargetToHostFifoU64;

#if !NiFpga_VxWorks

/* Control: Clusterin */
/* Use NiFpga_WriteArrayU8() to access Clusterin */
const uint32_t NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_Resource = 0x821C;
const uint32_t NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_PackedSizeInBytes = 51;

typedef struct NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_Type{
   uint16_t Numberofsampleinablock04095;
   uint16_t SamplingRateNDefault5000Decimationfactorfromrefclock;
   uint64_t Channelsshifregisterupto4A;
   uint64_t Channelsshifregisterupto4B;
   uint64_t Channelsshifregisterupto4C;
   uint64_t Infoshiftregister;
   int16_t rampcounter;
   NiFpga_Bool GroupEnableN;
   NiFpga_Bool Debugactivation;
   uint64_t Samplecounter;
   uint16_t Decimationcounter;
   uint16_t Blockscounter;
}NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_Type;


void NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_Type* const destination);

void NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Rseries_noModule_7854_ControlCluster_Clusterin_Type* const source);

#endif /* !NiFpga_VxWorks */


#if NiFpga_Cpp
}
#endif

#endif
