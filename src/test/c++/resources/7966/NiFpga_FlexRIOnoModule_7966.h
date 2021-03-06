/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOnoModule_7966_h__
#define __NiFpga_FlexRIOnoModule_7966_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOnoModule_7966_Bitfile;
 */
#define NiFpga_FlexRIOnoModule_7966_Bitfile "NiFpga_FlexRIOnoModule_7966.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOnoModule_7966_Signature = "47D8FDC2426A7A22B9F5D2082C616F58";

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_InitDone = 0xFE,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_RIOAdapterCorrect = 0x102,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_auxDI0 = 0x66,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_auxDI1 = 0x62,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_auxDI2 = 0x5E,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_auxDI3 = 0x5A,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_auxDI4 = 0x56,
   NiFpga_FlexRIOnoModule_7966_IndicatorBool_auxDI5 = 0x52,
} NiFpga_FlexRIOnoModule_7966_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorU8_DevProfile = 0xF2,
   NiFpga_FlexRIOnoModule_7966_IndicatorU8_DevQualityStatus = 0x46,
   NiFpga_FlexRIOnoModule_7966_IndicatorU8_Platform = 0x10E,
   NiFpga_FlexRIOnoModule_7966_IndicatorU8_SGNo = 0x4E,
} NiFpga_FlexRIOnoModule_7966_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorI16_DevTemp = 0x42,
} NiFpga_FlexRIOnoModule_7966_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorU16_DMATtoHOSTOverflows = 0x36,
} NiFpga_FlexRIOnoModule_7966_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI0 = 0x94,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI1 = 0x90,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI2 = 0x8C,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI3 = 0x88,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI4 = 0x84,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI5 = 0x80,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI6 = 0x1C,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI7 = 0x20,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI8 = 0x24,
   NiFpga_FlexRIOnoModule_7966_IndicatorI32_auxAI9 = 0x28,
} NiFpga_FlexRIOnoModule_7966_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorU32_ExpectedIOModuleID = 0x108,
   NiFpga_FlexRIOnoModule_7966_IndicatorU32_Fref = 0xF8,
   NiFpga_FlexRIOnoModule_7966_IndicatorU32_InsertedIOModuleID = 0x104,
   NiFpga_FlexRIOnoModule_7966_IndicatorU32_SGFref0 = 0x48,
} NiFpga_FlexRIOnoModule_7966_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorI64_aux64AI0 = 0xC8,
   NiFpga_FlexRIOnoModule_7966_IndicatorI64_aux64AI1 = 0xCC,
   NiFpga_FlexRIOnoModule_7966_IndicatorI64_aux64AI2 = 0xD0,
   NiFpga_FlexRIOnoModule_7966_IndicatorI64_aux64AI3 = 0xD4,
   NiFpga_FlexRIOnoModule_7966_IndicatorI64_aux64AI4 = 0xD8,
   NiFpga_FlexRIOnoModule_7966_IndicatorI64_aux64AI5 = 0xDC,
} NiFpga_FlexRIOnoModule_7966_IndicatorI64;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_ControlBool_AOEnable0 = 0x2,
   NiFpga_FlexRIOnoModule_7966_ControlBool_DAQStartStop = 0x3A,
   NiFpga_FlexRIOnoModule_7966_ControlBool_DMATtoHOSTEnable0 = 0x2E,
   NiFpga_FlexRIOnoModule_7966_ControlBool_DebugMode = 0x3E,
   NiFpga_FlexRIOnoModule_7966_ControlBool_auxDO0 = 0x7E,
   NiFpga_FlexRIOnoModule_7966_ControlBool_auxDO1 = 0x7A,
   NiFpga_FlexRIOnoModule_7966_ControlBool_auxDO2 = 0x76,
   NiFpga_FlexRIOnoModule_7966_ControlBool_auxDO3 = 0x6A,
   NiFpga_FlexRIOnoModule_7966_ControlBool_auxDO4 = 0x6E,
   NiFpga_FlexRIOnoModule_7966_ControlBool_auxDO5 = 0x72,
} NiFpga_FlexRIOnoModule_7966_ControlBool;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_ControlU8_SGSignalType0 = 0xA,
} NiFpga_FlexRIOnoModule_7966_ControlU8;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_ControlU16_DMATtoHOSTSamplingRate0 = 0x32,
   NiFpga_FlexRIOnoModule_7966_ControlU16_SGAmp0 = 0x1A,
} NiFpga_FlexRIOnoModule_7966_ControlU16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_ControlI32_AO0 = 0xC,
   NiFpga_FlexRIOnoModule_7966_ControlI32_auxAO0 = 0xAC,
   NiFpga_FlexRIOnoModule_7966_ControlI32_auxAO1 = 0xA8,
   NiFpga_FlexRIOnoModule_7966_ControlI32_auxAO2 = 0xA4,
   NiFpga_FlexRIOnoModule_7966_ControlI32_auxAO3 = 0xA0,
   NiFpga_FlexRIOnoModule_7966_ControlI32_auxAO4 = 0x9C,
   NiFpga_FlexRIOnoModule_7966_ControlI32_auxAO5 = 0x98,
} NiFpga_FlexRIOnoModule_7966_ControlI32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_ControlU32_SGFreq0 = 0x14,
   NiFpga_FlexRIOnoModule_7966_ControlU32_SGPhase0 = 0x10,
   NiFpga_FlexRIOnoModule_7966_ControlU32_SGUpdateRate0 = 0x4,
   NiFpga_FlexRIOnoModule_7966_ControlU32_TabControl = 0x114,
} NiFpga_FlexRIOnoModule_7966_ControlU32;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_ControlI64_aux64AO0 = 0xB0,
   NiFpga_FlexRIOnoModule_7966_ControlI64_aux64AO1 = 0xB4,
   NiFpga_FlexRIOnoModule_7966_ControlI64_aux64AO2 = 0xB8,
   NiFpga_FlexRIOnoModule_7966_ControlI64_aux64AO3 = 0xBC,
   NiFpga_FlexRIOnoModule_7966_ControlI64_aux64AO4 = 0xC0,
   NiFpga_FlexRIOnoModule_7966_ControlI64_aux64AO5 = 0xC4,
} NiFpga_FlexRIOnoModule_7966_ControlI64;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8_DMATtoHOSTFrameType = 0xE6,
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8_DMATtoHOSTSampleSize = 0xEA,
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8_FPGAVIversion = 0xF6,
} NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOnoModule_7966_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0xEE,
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU16_DMATtoHOSTNCh = 0xE2,
} NiFpga_FlexRIOnoModule_7966_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIOnoModule_7966_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOnoModule_7966_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOnoModule_7966_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOnoModule_7966_TargetToHostFifoU64;

#endif
