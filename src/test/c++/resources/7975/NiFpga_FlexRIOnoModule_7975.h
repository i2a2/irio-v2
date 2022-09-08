/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOnoModule_7975_h__
#define __NiFpga_FlexRIOnoModule_7975_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOnoModule_7975_Bitfile;
 */
#define NiFpga_FlexRIOnoModule_7975_Bitfile "NiFpga_FlexRIOnoModule_7975.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOnoModule_7975_Signature = "C9BA136F2358CF65E584E92593A3DC3A";

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_InitDone = 0x100FE,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_RIOAdapterCorrect = 0x10102,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_auxDI0 = 0x10066,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_auxDI1 = 0x10062,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_auxDI2 = 0x1005E,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_auxDI3 = 0x1005A,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_auxDI4 = 0x10056,
   NiFpga_FlexRIOnoModule_7975_IndicatorBool_auxDI5 = 0x10052,
} NiFpga_FlexRIOnoModule_7975_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorU8_DevProfile = 0x100F2,
   NiFpga_FlexRIOnoModule_7975_IndicatorU8_DevQualityStatus = 0x10046,
   NiFpga_FlexRIOnoModule_7975_IndicatorU8_Platform = 0x1010E,
   NiFpga_FlexRIOnoModule_7975_IndicatorU8_SGNo = 0x1004E,
} NiFpga_FlexRIOnoModule_7975_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorI16_DevTemp = 0x10042,
} NiFpga_FlexRIOnoModule_7975_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorU16_DMATtoHOSTOverflows = 0x10036,
} NiFpga_FlexRIOnoModule_7975_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI0 = 0x10094,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI1 = 0x10090,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI2 = 0x1008C,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI3 = 0x10088,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI4 = 0x10084,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI5 = 0x10080,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI6 = 0x1001C,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI7 = 0x10020,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI8 = 0x10024,
   NiFpga_FlexRIOnoModule_7975_IndicatorI32_auxAI9 = 0x10028,
} NiFpga_FlexRIOnoModule_7975_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorU32_ExpectedIOModuleID = 0x10108,
   NiFpga_FlexRIOnoModule_7975_IndicatorU32_Fref = 0x100F8,
   NiFpga_FlexRIOnoModule_7975_IndicatorU32_InsertedIOModuleID = 0x10104,
   NiFpga_FlexRIOnoModule_7975_IndicatorU32_SGFref0 = 0x10048,
} NiFpga_FlexRIOnoModule_7975_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorI64_aux64AI0 = 0x100C8,
   NiFpga_FlexRIOnoModule_7975_IndicatorI64_aux64AI1 = 0x100CC,
   NiFpga_FlexRIOnoModule_7975_IndicatorI64_aux64AI2 = 0x100D0,
   NiFpga_FlexRIOnoModule_7975_IndicatorI64_aux64AI3 = 0x100D4,
   NiFpga_FlexRIOnoModule_7975_IndicatorI64_aux64AI4 = 0x100D8,
   NiFpga_FlexRIOnoModule_7975_IndicatorI64_aux64AI5 = 0x100DC,
} NiFpga_FlexRIOnoModule_7975_IndicatorI64;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_ControlBool_AOEnable0 = 0x10002,
   NiFpga_FlexRIOnoModule_7975_ControlBool_DAQStartStop = 0x1003A,
   NiFpga_FlexRIOnoModule_7975_ControlBool_DMATtoHOSTEnable0 = 0x1002E,
   NiFpga_FlexRIOnoModule_7975_ControlBool_DebugMode = 0x1003E,
   NiFpga_FlexRIOnoModule_7975_ControlBool_auxDO0 = 0x1007E,
   NiFpga_FlexRIOnoModule_7975_ControlBool_auxDO1 = 0x1007A,
   NiFpga_FlexRIOnoModule_7975_ControlBool_auxDO2 = 0x10076,
   NiFpga_FlexRIOnoModule_7975_ControlBool_auxDO3 = 0x1006A,
   NiFpga_FlexRIOnoModule_7975_ControlBool_auxDO4 = 0x1006E,
   NiFpga_FlexRIOnoModule_7975_ControlBool_auxDO5 = 0x10072,
} NiFpga_FlexRIOnoModule_7975_ControlBool;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_ControlU8_SGSignalType0 = 0x1000A,
} NiFpga_FlexRIOnoModule_7975_ControlU8;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_ControlU16_DMATtoHOSTSamplingRate0 = 0x10032,
   NiFpga_FlexRIOnoModule_7975_ControlU16_SGAmp0 = 0x1001A,
} NiFpga_FlexRIOnoModule_7975_ControlU16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_ControlI32_AO0 = 0x1000C,
   NiFpga_FlexRIOnoModule_7975_ControlI32_auxAO0 = 0x100AC,
   NiFpga_FlexRIOnoModule_7975_ControlI32_auxAO1 = 0x100A8,
   NiFpga_FlexRIOnoModule_7975_ControlI32_auxAO2 = 0x100A4,
   NiFpga_FlexRIOnoModule_7975_ControlI32_auxAO3 = 0x100A0,
   NiFpga_FlexRIOnoModule_7975_ControlI32_auxAO4 = 0x1009C,
   NiFpga_FlexRIOnoModule_7975_ControlI32_auxAO5 = 0x10098,
} NiFpga_FlexRIOnoModule_7975_ControlI32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_ControlU32_SGFreq0 = 0x10014,
   NiFpga_FlexRIOnoModule_7975_ControlU32_SGPhase0 = 0x10010,
   NiFpga_FlexRIOnoModule_7975_ControlU32_SGUpdateRate0 = 0x10004,
   NiFpga_FlexRIOnoModule_7975_ControlU32_TabControl = 0x10114,
} NiFpga_FlexRIOnoModule_7975_ControlU32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_ControlI64_aux64AO0 = 0x100B0,
   NiFpga_FlexRIOnoModule_7975_ControlI64_aux64AO1 = 0x100B4,
   NiFpga_FlexRIOnoModule_7975_ControlI64_aux64AO2 = 0x100B8,
   NiFpga_FlexRIOnoModule_7975_ControlI64_aux64AO3 = 0x100BC,
   NiFpga_FlexRIOnoModule_7975_ControlI64_aux64AO4 = 0x100C0,
   NiFpga_FlexRIOnoModule_7975_ControlI64_aux64AO5 = 0x100C4,
} NiFpga_FlexRIOnoModule_7975_ControlI64;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8_DMATtoHOSTFrameType = 0x100E6,
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x100EA,
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8_FPGAVIversion = 0x100F6,
} NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOnoModule_7975_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x100EE,
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU16_DMATtoHOSTNCh = 0x100E2,
} NiFpga_FlexRIOnoModule_7975_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIOnoModule_7975_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOnoModule_7975_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOnoModule_7975_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOnoModule_7975_TargetToHostFifoU64;

#endif
