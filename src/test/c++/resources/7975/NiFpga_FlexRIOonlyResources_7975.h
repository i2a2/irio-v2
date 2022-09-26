/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOonlyResources_7975_h__
#define __NiFpga_FlexRIOonlyResources_7975_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOonlyResources_7975_Bitfile;
 */
#define NiFpga_FlexRIOonlyResources_7975_Bitfile "NiFpga_FlexRIOonlyResources_7975.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOonlyResources_7975_Signature = "E2C5791058E3D24FEDF1104354CD0AC0";

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI0 = 0x101C6,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI1 = 0x101C2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI10 = 0x10166,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI11 = 0x10162,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI12 = 0x10186,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI13 = 0x10182,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI14 = 0x1017E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI15 = 0x1017A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI16 = 0x102C2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI17 = 0x102C6,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI18 = 0x101FA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI19 = 0x101FE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI2 = 0x101BE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI20 = 0x10202,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI21 = 0x10206,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI22 = 0x1020A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI23 = 0x1020E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI24 = 0x1024A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI25 = 0x1024E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI26 = 0x10252,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI27 = 0x10256,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI28 = 0x1025A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI29 = 0x1025E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI3 = 0x101BA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI30 = 0x1023A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI31 = 0x1023E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI32 = 0x10242,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI33 = 0x10246,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI34 = 0x102BA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI35 = 0x102BE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI36 = 0x1027A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI37 = 0x1027E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI38 = 0x10282,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI39 = 0x10286,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI4 = 0x101B6,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI40 = 0x1028A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI41 = 0x1028E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI42 = 0x102CA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI43 = 0x102CE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI44 = 0x102D2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI45 = 0x102D6,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI46 = 0x102DA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI47 = 0x102DE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI48 = 0x10306,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI49 = 0x1030A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI5 = 0x101B2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI50 = 0x1030E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI51 = 0x102FA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI52 = 0x102FE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI53 = 0x10302,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI6 = 0x10176,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI7 = 0x10172,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI8 = 0x1016E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_DI9 = 0x1016A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_InitDone = 0x103AE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_RIOAdapterCorrect = 0x103B2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI0 = 0x10072,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI1 = 0x1006E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI10 = 0x1010A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI11 = 0x1010E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI12 = 0x100EA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI13 = 0x100EE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI14 = 0x100F2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI15 = 0x100F6,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI2 = 0x1006A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI3 = 0x10066,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI4 = 0x10062,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI5 = 0x1005E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI6 = 0x100FA,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI7 = 0x100FE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI8 = 0x10102,
   NiFpga_FlexRIOonlyResources_7975_IndicatorBool_auxDI9 = 0x10106,
} NiFpga_FlexRIOonlyResources_7975_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorU8_DevProfile = 0x103A2,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU8_DevQualityStatus = 0x10052,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU8_Platform = 0x103BE,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU8_SGNo = 0x1005A,
} NiFpga_FlexRIOonlyResources_7975_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorI16_DevTemp = 0x1004E,
} NiFpga_FlexRIOonlyResources_7975_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorU16_DMATtoHOSTOverflows = 0x10042,
} NiFpga_FlexRIOonlyResources_7975_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI0 = 0x100A0,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI1 = 0x1009C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI10 = 0x10148,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI11 = 0x1014C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI12 = 0x10150,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI13 = 0x10154,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI14 = 0x10158,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI15 = 0x1015C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI2 = 0x10098,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI3 = 0x10094,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI4 = 0x10090,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI5 = 0x1008C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI6 = 0x10138,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI7 = 0x1013C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI8 = 0x10140,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI32_auxAI9 = 0x10144,
} NiFpga_FlexRIOonlyResources_7975_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorU32_ExpectedIOModuleID = 0x103B8,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU32_Fref = 0x103A8,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU32_InsertedIOModuleID = 0x103B4,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU32_SGFref0 = 0x100BC,
   NiFpga_FlexRIOonlyResources_7975_IndicatorU32_SGFref1 = 0x10054,
} NiFpga_FlexRIOonlyResources_7975_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI0 = 0x10338,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI1 = 0x1033C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI10 = 0x10324,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI11 = 0x10320,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI12 = 0x1031C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI13 = 0x10318,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI14 = 0x10314,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI15 = 0x10310,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI2 = 0x10340,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI3 = 0x10344,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI4 = 0x10348,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI5 = 0x1034C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI6 = 0x10334,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI7 = 0x10330,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI8 = 0x1032C,
   NiFpga_FlexRIOonlyResources_7975_IndicatorI64_aux64AI9 = 0x10328,
} NiFpga_FlexRIOonlyResources_7975_IndicatorI64;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_ControlBool_AOEnable0 = 0x10002,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_AOEnable1 = 0x1001E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DAQStartStop = 0x10046,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DMATtoHOSTEnable0 = 0x1003A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO0 = 0x101DE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO1 = 0x101DA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO10 = 0x1019E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO11 = 0x101A2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO12 = 0x10196,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO13 = 0x10192,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO14 = 0x1018E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO15 = 0x1018A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO16 = 0x101F2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO17 = 0x101EE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO18 = 0x101E2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO19 = 0x101E6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO2 = 0x101D6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO20 = 0x101EA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO21 = 0x101F6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO22 = 0x10222,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO23 = 0x1021E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO24 = 0x10212,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO25 = 0x10216,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO26 = 0x1021A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO27 = 0x10226,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO28 = 0x10272,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO29 = 0x1026E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO3 = 0x101CA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO30 = 0x1022A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO31 = 0x1022E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO32 = 0x10232,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO33 = 0x10236,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO34 = 0x102A2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO35 = 0x1029E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO36 = 0x10262,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO37 = 0x10266,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO38 = 0x1026A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO39 = 0x10276,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO4 = 0x101CE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO40 = 0x102E6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO41 = 0x102EA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO42 = 0x10292,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO43 = 0x10296,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO44 = 0x1029A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO45 = 0x102A6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO46 = 0x102F2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO47 = 0x102F6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO48 = 0x102AA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO49 = 0x102AE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO5 = 0x101D2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO50 = 0x102B2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO51 = 0x102B6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO52 = 0x102E2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO53 = 0x102EE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO6 = 0x101AE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO7 = 0x101AA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO8 = 0x101A6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DO9 = 0x1019A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_DebugMode = 0x1004A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO0 = 0x1008A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO1 = 0x10086,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO10 = 0x100D2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO11 = 0x100CE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO12 = 0x100DA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO13 = 0x100DE,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO14 = 0x100E2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO15 = 0x100E6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO2 = 0x10082,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO3 = 0x10076,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO4 = 0x1007A,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO5 = 0x1007E,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO6 = 0x100C2,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO7 = 0x100C6,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO8 = 0x100CA,
   NiFpga_FlexRIOonlyResources_7975_ControlBool_auxDO9 = 0x100D6,
} NiFpga_FlexRIOonlyResources_7975_ControlBool;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_ControlU8_SGSignalType0 = 0x1000A,
   NiFpga_FlexRIOonlyResources_7975_ControlU8_SGSignalType1 = 0x10026,
} NiFpga_FlexRIOonlyResources_7975_ControlU8;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_ControlU16_DMATtoHOSTSamplingRate0 = 0x1003E,
   NiFpga_FlexRIOonlyResources_7975_ControlU16_SGAmp0 = 0x1001A,
   NiFpga_FlexRIOonlyResources_7975_ControlU16_SGAmp1 = 0x10036,
} NiFpga_FlexRIOonlyResources_7975_ControlU16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_ControlI32_AO0 = 0x1000C,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_AO1 = 0x10028,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO0 = 0x100B8,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO1 = 0x100B4,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO10 = 0x10120,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO11 = 0x10124,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO12 = 0x10128,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO13 = 0x1012C,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO14 = 0x10130,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO15 = 0x10134,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO2 = 0x100B0,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO3 = 0x100AC,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO4 = 0x100A8,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO5 = 0x100A4,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO6 = 0x10110,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO7 = 0x10114,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO8 = 0x10118,
   NiFpga_FlexRIOonlyResources_7975_ControlI32_auxAO9 = 0x1011C,
} NiFpga_FlexRIOonlyResources_7975_ControlI32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_ControlU32_SGFreq0 = 0x10014,
   NiFpga_FlexRIOonlyResources_7975_ControlU32_SGFreq1 = 0x10030,
   NiFpga_FlexRIOonlyResources_7975_ControlU32_SGPhase0 = 0x10010,
   NiFpga_FlexRIOonlyResources_7975_ControlU32_SGPhase1 = 0x1002C,
   NiFpga_FlexRIOonlyResources_7975_ControlU32_SGUpdateRate0 = 0x10004,
   NiFpga_FlexRIOonlyResources_7975_ControlU32_SGUpdateRate1 = 0x10020,
   NiFpga_FlexRIOonlyResources_7975_ControlU32_TabControl = 0x103C4,
} NiFpga_FlexRIOonlyResources_7975_ControlU32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO0 = 0x10378,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO1 = 0x1037C,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO11 = 0x10360,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO12 = 0x1035C,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO13 = 0x10358,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO14 = 0x10354,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO15 = 0x10350,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO2 = 0x10380,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO3 = 0x10384,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO4 = 0x10388,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO5 = 0x1038C,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO6 = 0x10374,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO7 = 0x10370,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO8 = 0x1036C,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux64AO9 = 0x10368,
   NiFpga_FlexRIOonlyResources_7975_ControlI64_aux65AO10 = 0x10364,
} NiFpga_FlexRIOonlyResources_7975_ControlI64;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8_DMATtoHOSTFrameType = 0x10396,
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x1039A,
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8_FPGAVIversion = 0x103A6,
} NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x1039E,
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU16_DMATtoHOSTNCh = 0x10392,
} NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOonlyResources_7975_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7975_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOonlyResources_7975_TargetToHostFifoU64;

#endif
