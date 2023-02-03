/*
 * Generated with the FPGA Interface C API Generator 21.3
 * for NI-RIO 21.3 or later.
 */
#ifndef __NiFpga_Rseries_7854_h__
#define __NiFpga_Rseries_7854_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 213
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Rseries_7854_Bitfile;
 */
#define NiFpga_Rseries_7854_Bitfile "NiFpga_Rseries_7854.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Rseries_7854_Signature = "99F91E12F6134F3E5B9DFD241A490DAA";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_Rseries_7854_IndicatorBool_InitDone = 0x812E,
   NiFpga_Rseries_7854_IndicatorBool_RIOAdapterCorrect = 0x812A,
   NiFpga_Rseries_7854_IndicatorBool_auxDI0 = 0x81B6,
   NiFpga_Rseries_7854_IndicatorBool_auxDI1 = 0x81BA,
   NiFpga_Rseries_7854_IndicatorBool_auxDI2 = 0x81BE,
   NiFpga_Rseries_7854_IndicatorBool_auxDI3 = 0x81C2,
   NiFpga_Rseries_7854_IndicatorBool_auxDI4 = 0x81C6,
   NiFpga_Rseries_7854_IndicatorBool_auxDI5 = 0x81CA
} NiFpga_Rseries_7854_IndicatorBool;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorU8_DevProfile = 0x813A,
   NiFpga_Rseries_7854_IndicatorU8_DevQualityStatus = 0x811A,
   NiFpga_Rseries_7854_IndicatorU8_Platform = 0x8116,
   NiFpga_Rseries_7854_IndicatorU8_SGNo = 0x81EA
} NiFpga_Rseries_7854_IndicatorU8;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorI16_DevTemp = 0x811E
} NiFpga_Rseries_7854_IndicatorI16;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorU16_DMATtoHOSTOverflows = 0x814A
} NiFpga_Rseries_7854_IndicatorU16;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorI32_auxAI0 = 0x8184,
   NiFpga_Rseries_7854_IndicatorI32_auxAI1 = 0x8188,
   NiFpga_Rseries_7854_IndicatorI32_auxAI2 = 0x818C,
   NiFpga_Rseries_7854_IndicatorI32_auxAI3 = 0x8190,
   NiFpga_Rseries_7854_IndicatorI32_auxAI4 = 0x8194,
   NiFpga_Rseries_7854_IndicatorI32_auxAI5 = 0x8198,
   NiFpga_Rseries_7854_IndicatorI32_auxAI6 = 0x8168,
   NiFpga_Rseries_7854_IndicatorI32_auxAI7 = 0x8164,
   NiFpga_Rseries_7854_IndicatorI32_auxAI8 = 0x8160,
   NiFpga_Rseries_7854_IndicatorI32_auxAI9 = 0x815C
} NiFpga_Rseries_7854_IndicatorI32;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorU32_Fref = 0x8130,
   NiFpga_Rseries_7854_IndicatorU32_SGFref0 = 0x8158
} NiFpga_Rseries_7854_IndicatorU32;

typedef enum
{
   NiFpga_Rseries_7854_ControlBool_AOEnable0 = 0x81E6,
   NiFpga_Rseries_7854_ControlBool_DAQStartStop = 0x8126,
   NiFpga_Rseries_7854_ControlBool_DMATtoHOSTEnable0 = 0x8152,
   NiFpga_Rseries_7854_ControlBool_DebugMode = 0x8122,
   NiFpga_Rseries_7854_ControlBool_auxDO0 = 0x819E,
   NiFpga_Rseries_7854_ControlBool_auxDO1 = 0x81A2,
   NiFpga_Rseries_7854_ControlBool_auxDO2 = 0x81A6,
   NiFpga_Rseries_7854_ControlBool_auxDO3 = 0x81B2,
   NiFpga_Rseries_7854_ControlBool_auxDO4 = 0x81AE,
   NiFpga_Rseries_7854_ControlBool_auxDO5 = 0x81AA
} NiFpga_Rseries_7854_ControlBool;

typedef enum
{
   NiFpga_Rseries_7854_ControlU8_SGSignalType0 = 0x81DE
} NiFpga_Rseries_7854_ControlU8;

typedef enum
{
   NiFpga_Rseries_7854_ControlU16_DMATtoHOSTSamplingRate0 = 0x814E,
   NiFpga_Rseries_7854_ControlU16_SGAmp0 = 0x81CE
} NiFpga_Rseries_7854_ControlU16;

typedef enum
{
   NiFpga_Rseries_7854_ControlI32_AO0 = 0x81D8,
   NiFpga_Rseries_7854_ControlI32_auxAO0 = 0x816C,
   NiFpga_Rseries_7854_ControlI32_auxAO1 = 0x8170,
   NiFpga_Rseries_7854_ControlI32_auxAO2 = 0x8174,
   NiFpga_Rseries_7854_ControlI32_auxAO3 = 0x8178,
   NiFpga_Rseries_7854_ControlI32_auxAO4 = 0x817C,
   NiFpga_Rseries_7854_ControlI32_auxAO5 = 0x8180
} NiFpga_Rseries_7854_ControlI32;

typedef enum
{
   NiFpga_Rseries_7854_ControlU32_SGFreq0 = 0x81D0,
   NiFpga_Rseries_7854_ControlU32_SGPhase0 = 0x81D4,
   NiFpga_Rseries_7854_ControlU32_SGUpdateRate0 = 0x81E0,
   NiFpga_Rseries_7854_ControlU32_TabControl = 0x810C
} NiFpga_Rseries_7854_ControlU32;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorArrayU8_DMATtoHOSTFrameType = 0x8146,
   NiFpga_Rseries_7854_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x8142,
   NiFpga_Rseries_7854_IndicatorArrayU8_FPGAVIversion = 0x8136
} NiFpga_Rseries_7854_IndicatorArrayU8;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_Rseries_7854_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_Rseries_7854_IndicatorArrayU8Size_FPGAVIversion = 2
} NiFpga_Rseries_7854_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x813E,
   NiFpga_Rseries_7854_IndicatorArrayU16_DMATtoHOSTNCh = 0x8156
} NiFpga_Rseries_7854_IndicatorArrayU16;

typedef enum
{
   NiFpga_Rseries_7854_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_Rseries_7854_IndicatorArrayU16Size_DMATtoHOSTNCh = 1
} NiFpga_Rseries_7854_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_Rseries_7854_TargetToHostFifoU64_DMATtoHOST0 = 0
} NiFpga_Rseries_7854_TargetToHostFifoU64;

/* Control: Clusterin */
/* Use NiFpga_WriteArrayU8() to access Clusterin */
static const uint32_t NiFpga_Rseries_7854_ControlCluster_Clusterin_Resource = 0x8110;
static const uint32_t NiFpga_Rseries_7854_ControlCluster_Clusterin_PackedSizeInBytes = 51;

typedef struct NiFpga_Rseries_7854_ControlCluster_Clusterin_Type{
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
}NiFpga_Rseries_7854_ControlCluster_Clusterin_Type;


void NiFpga_Rseries_7854_ControlCluster_Clusterin_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Rseries_7854_ControlCluster_Clusterin_Type* const destination);

void NiFpga_Rseries_7854_ControlCluster_Clusterin_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Rseries_7854_ControlCluster_Clusterin_Type* const source);


#if NiFpga_Cpp
}
#endif

#endif
