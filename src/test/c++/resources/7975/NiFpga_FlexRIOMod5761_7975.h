/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOMod5761_7975_h__
#define __NiFpga_FlexRIOMod5761_7975_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOMod5761_7975_Bitfile;
 */
#define NiFpga_FlexRIOMod5761_7975_Bitfile "NiFpga_FlexRIOMod5761_7975.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOMod5761_7975_Signature = "937F16C77799A5118B8DBB01E5A961AA";

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_InitDone = 0x100EE,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_RIOAdapterCorrect = 0x100F2,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_auxDI0 = 0x1007A,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_auxDI1 = 0x10076,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_auxDI2 = 0x10072,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_auxDI3 = 0x1006E,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_auxDI4 = 0x1006A,
   NiFpga_FlexRIOMod5761_7975_IndicatorBool_auxDI5 = 0x10066,
} NiFpga_FlexRIOMod5761_7975_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorU8_DevProfile = 0x100E2,
   NiFpga_FlexRIOMod5761_7975_IndicatorU8_DevQualityStatus = 0x1005A,
   NiFpga_FlexRIOMod5761_7975_IndicatorU8_Platform = 0x100FE,
   NiFpga_FlexRIOMod5761_7975_IndicatorU8_SGNo = 0x10062,
} NiFpga_FlexRIOMod5761_7975_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorI16_DevTemp = 0x10056,
} NiFpga_FlexRIOMod5761_7975_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorU16_DMATtoHOSTOverflows = 0x1004A,
} NiFpga_FlexRIOMod5761_7975_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI0 = 0x100A8,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI1 = 0x100A4,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI10 = 0x10018,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI2 = 0x100A0,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI3 = 0x1009C,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI4 = 0x10098,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI5 = 0x10094,
   NiFpga_FlexRIOMod5761_7975_IndicatorI32_auxAI9 = 0x10014,
} NiFpga_FlexRIOMod5761_7975_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorU32_ExpectedIOModuleID = 0x100F8,
   NiFpga_FlexRIOMod5761_7975_IndicatorU32_Fref = 0x100E8,
   NiFpga_FlexRIOMod5761_7975_IndicatorU32_InsertedIOModuleID = 0x100F4,
   NiFpga_FlexRIOMod5761_7975_IndicatorU32_SGFref0 = 0x100C4,
   NiFpga_FlexRIOMod5761_7975_IndicatorU32_SGFref1 = 0x1005C,
} NiFpga_FlexRIOMod5761_7975_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorI64_aux64AI0 = 0x100C8,
} NiFpga_FlexRIOMod5761_7975_IndicatorI64;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_ControlBool_AOEnable0 = 0x1003A,
   NiFpga_FlexRIOMod5761_7975_ControlBool_AOEnable1 = 0x1001E,
   NiFpga_FlexRIOMod5761_7975_ControlBool_DAQStartStop = 0x1004E,
   NiFpga_FlexRIOMod5761_7975_ControlBool_DMATtoHOSTEnable0 = 0x10042,
   NiFpga_FlexRIOMod5761_7975_ControlBool_DebugMode = 0x10052,
   NiFpga_FlexRIOMod5761_7975_ControlBool_auxDO0 = 0x10092,
   NiFpga_FlexRIOMod5761_7975_ControlBool_auxDO1 = 0x1008E,
   NiFpga_FlexRIOMod5761_7975_ControlBool_auxDO2 = 0x1008A,
   NiFpga_FlexRIOMod5761_7975_ControlBool_auxDO3 = 0x1007E,
   NiFpga_FlexRIOMod5761_7975_ControlBool_auxDO4 = 0x10082,
   NiFpga_FlexRIOMod5761_7975_ControlBool_auxDO5 = 0x10086,
} NiFpga_FlexRIOMod5761_7975_ControlBool;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_ControlU8_SGSignalType0 = 0x1003E,
   NiFpga_FlexRIOMod5761_7975_ControlU8_SGSignalType1 = 0x10026,
} NiFpga_FlexRIOMod5761_7975_ControlU8;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_ControlU16_DMATtoHOSTSamplingRate0 = 0x10046,
   NiFpga_FlexRIOMod5761_7975_ControlU16_SGAmp0 = 0x10012,
   NiFpga_FlexRIOMod5761_7975_ControlU16_SGAmp1 = 0x10036,
} NiFpga_FlexRIOMod5761_7975_ControlU16;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_ControlI32_AO0 = 0x10004,
   NiFpga_FlexRIOMod5761_7975_ControlI32_AO1 = 0x10028,
   NiFpga_FlexRIOMod5761_7975_ControlI32_auxAO0 = 0x100C0,
   NiFpga_FlexRIOMod5761_7975_ControlI32_auxAO1 = 0x100BC,
   NiFpga_FlexRIOMod5761_7975_ControlI32_auxAO2 = 0x100B8,
   NiFpga_FlexRIOMod5761_7975_ControlI32_auxAO3 = 0x100B4,
   NiFpga_FlexRIOMod5761_7975_ControlI32_auxAO4 = 0x100B0,
   NiFpga_FlexRIOMod5761_7975_ControlI32_auxAO5 = 0x100AC,
} NiFpga_FlexRIOMod5761_7975_ControlI32;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_ControlU32_SGFreq0 = 0x1000C,
   NiFpga_FlexRIOMod5761_7975_ControlU32_SGFreq1 = 0x10030,
   NiFpga_FlexRIOMod5761_7975_ControlU32_SGPhase0 = 0x10008,
   NiFpga_FlexRIOMod5761_7975_ControlU32_SGPhase1 = 0x1002C,
   NiFpga_FlexRIOMod5761_7975_ControlU32_SGUpdateRate0 = 0x10000,
   NiFpga_FlexRIOMod5761_7975_ControlU32_SGUpdateRate1 = 0x10020,
   NiFpga_FlexRIOMod5761_7975_ControlU32_TabControl = 0x10104,
} NiFpga_FlexRIOMod5761_7975_ControlU32;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_ControlI64_aux64AO0 = 0x100CC,
} NiFpga_FlexRIOMod5761_7975_ControlI64;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8_DMATtoHOSTFrameType = 0x100D6,
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x100DA,
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8_FPGAVIversion = 0x100E6,
} NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOMod5761_7975_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x100DE,
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU16_DMATtoHOSTNCh = 0x100D2,
} NiFpga_FlexRIOMod5761_7975_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIOMod5761_7975_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOMod5761_7975_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOMod5761_7975_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOMod5761_7975_TargetToHostFifoU64;

#endif
