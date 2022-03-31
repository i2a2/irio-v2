/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOonlyResources_7965_h__
#define __NiFpga_FlexRIOonlyResources_7965_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOonlyResources_7965_Bitfile;
 */
#define NiFpga_FlexRIOonlyResources_7965_Bitfile "NiFpga_FlexRIOonlyResources_7965.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOonlyResources_7965_Signature = "F48A49C7BA9ECCA73AA60533B666979B";

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI0 = 0x1C6,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI1 = 0x1C2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI10 = 0x166,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI11 = 0x162,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI12 = 0x186,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI13 = 0x182,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI14 = 0x17E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI15 = 0x17A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI16 = 0x2C2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI17 = 0x2C6,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI18 = 0x1FA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI19 = 0x1FE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI2 = 0x1BE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI20 = 0x202,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI21 = 0x206,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI22 = 0x20A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI23 = 0x20E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI24 = 0x24A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI25 = 0x24E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI26 = 0x252,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI27 = 0x256,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI28 = 0x25A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI29 = 0x25E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI3 = 0x1BA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI30 = 0x23A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI31 = 0x23E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI32 = 0x242,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI33 = 0x246,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI34 = 0x2BA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI35 = 0x2BE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI36 = 0x27A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI37 = 0x27E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI38 = 0x282,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI39 = 0x286,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI4 = 0x1B6,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI40 = 0x28A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI41 = 0x28E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI42 = 0x2CA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI43 = 0x2CE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI44 = 0x2D2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI45 = 0x2D6,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI46 = 0x2DA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI47 = 0x2DE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI48 = 0x306,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI49 = 0x30A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI5 = 0x1B2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI50 = 0x30E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI51 = 0x2FA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI52 = 0x2FE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI53 = 0x302,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI6 = 0x176,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI7 = 0x172,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI8 = 0x16E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_DI9 = 0x16A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_InitDone = 0x3AE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_RIOAdapterCorrect = 0x3B2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI0 = 0x72,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI1 = 0x6E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI10 = 0x10A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI11 = 0x10E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI12 = 0xEA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI13 = 0xEE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI14 = 0xF2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI15 = 0xF6,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI2 = 0x6A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI3 = 0x66,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI4 = 0x62,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI5 = 0x5E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI6 = 0xFA,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI7 = 0xFE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI8 = 0x102,
   NiFpga_FlexRIOonlyResources_7965_IndicatorBool_auxDI9 = 0x106,
} NiFpga_FlexRIOonlyResources_7965_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorU8_DevProfile = 0x3A2,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU8_DevQualityStatus = 0x52,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU8_Platform = 0x3BE,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU8_SGNo = 0x5A,
} NiFpga_FlexRIOonlyResources_7965_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorI16_DevTemp = 0x4E,
} NiFpga_FlexRIOonlyResources_7965_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorU16_DMATtoHOSTOverflows = 0x42,
} NiFpga_FlexRIOonlyResources_7965_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI0 = 0xA0,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI1 = 0x9C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI10 = 0x148,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI11 = 0x14C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI12 = 0x150,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI13 = 0x154,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI14 = 0x158,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI15 = 0x15C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI2 = 0x98,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI3 = 0x94,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI4 = 0x90,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI5 = 0x8C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI6 = 0x138,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI7 = 0x13C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI8 = 0x140,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI32_auxAI9 = 0x144,
} NiFpga_FlexRIOonlyResources_7965_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorU32_ExpectedIOModuleID = 0x3B8,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU32_Fref = 0x3A8,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU32_InsertedIOModuleID = 0x3B4,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU32_SGFref0 = 0xBC,
   NiFpga_FlexRIOonlyResources_7965_IndicatorU32_SGFref1 = 0x54,
} NiFpga_FlexRIOonlyResources_7965_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI0 = 0x338,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI1 = 0x33C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI10 = 0x324,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI11 = 0x320,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI12 = 0x31C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI13 = 0x318,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI14 = 0x314,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI15 = 0x310,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI2 = 0x340,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI3 = 0x344,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI4 = 0x348,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI5 = 0x34C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI6 = 0x334,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI7 = 0x330,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI8 = 0x32C,
   NiFpga_FlexRIOonlyResources_7965_IndicatorI64_aux64AI9 = 0x328,
} NiFpga_FlexRIOonlyResources_7965_IndicatorI64;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_ControlBool_AOEnable0 = 0x2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_AOEnable1 = 0x1E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DAQStartStop = 0x46,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DMATtoHOSTEnable0 = 0x3A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO0 = 0x1DE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO1 = 0x1DA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO10 = 0x19E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO11 = 0x1A2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO12 = 0x196,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO13 = 0x192,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO14 = 0x18E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO15 = 0x18A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO16 = 0x1F2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO17 = 0x1EE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO18 = 0x1E2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO19 = 0x1E6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO2 = 0x1D6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO20 = 0x1EA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO21 = 0x1F6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO22 = 0x222,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO23 = 0x21E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO24 = 0x212,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO25 = 0x216,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO26 = 0x21A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO27 = 0x226,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO28 = 0x272,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO29 = 0x26E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO3 = 0x1CA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO30 = 0x22A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO31 = 0x22E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO32 = 0x232,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO33 = 0x236,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO34 = 0x2A2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO35 = 0x29E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO36 = 0x262,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO37 = 0x266,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO38 = 0x26A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO39 = 0x276,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO4 = 0x1CE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO40 = 0x2E6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO41 = 0x2EA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO42 = 0x292,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO43 = 0x296,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO44 = 0x29A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO45 = 0x2A6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO46 = 0x2F2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO47 = 0x2F6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO48 = 0x2AA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO49 = 0x2AE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO5 = 0x1D2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO50 = 0x2B2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO51 = 0x2B6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO52 = 0x2E2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO53 = 0x2EE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO6 = 0x1AE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO7 = 0x1AA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO8 = 0x1A6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DO9 = 0x19A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_DebugMode = 0x4A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO0 = 0x8A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO1 = 0x86,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO10 = 0xD2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO11 = 0xCE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO12 = 0xDA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO13 = 0xDE,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO14 = 0xE2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO15 = 0xE6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO2 = 0x82,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO3 = 0x76,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO4 = 0x7A,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO5 = 0x7E,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO6 = 0xC2,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO7 = 0xC6,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO8 = 0xCA,
   NiFpga_FlexRIOonlyResources_7965_ControlBool_auxDO9 = 0xD6,
} NiFpga_FlexRIOonlyResources_7965_ControlBool;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_ControlU8_SGSignalType0 = 0xA,
   NiFpga_FlexRIOonlyResources_7965_ControlU8_SGSignalType1 = 0x26,
} NiFpga_FlexRIOonlyResources_7965_ControlU8;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_ControlU16_DMATtoHOSTSamplingRate0 = 0x3E,
   NiFpga_FlexRIOonlyResources_7965_ControlU16_SGAmp0 = 0x1A,
   NiFpga_FlexRIOonlyResources_7965_ControlU16_SGAmp1 = 0x36,
} NiFpga_FlexRIOonlyResources_7965_ControlU16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_ControlI32_AO0 = 0xC,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_AO1 = 0x28,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO0 = 0xB8,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO1 = 0xB4,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO10 = 0x120,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO11 = 0x124,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO12 = 0x128,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO13 = 0x12C,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO14 = 0x130,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO15 = 0x134,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO2 = 0xB0,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO3 = 0xAC,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO4 = 0xA8,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO5 = 0xA4,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO6 = 0x110,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO7 = 0x114,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO8 = 0x118,
   NiFpga_FlexRIOonlyResources_7965_ControlI32_auxAO9 = 0x11C,
} NiFpga_FlexRIOonlyResources_7965_ControlI32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_ControlU32_SGFreq0 = 0x14,
   NiFpga_FlexRIOonlyResources_7965_ControlU32_SGFreq1 = 0x30,
   NiFpga_FlexRIOonlyResources_7965_ControlU32_SGPhase0 = 0x10,
   NiFpga_FlexRIOonlyResources_7965_ControlU32_SGPhase1 = 0x2C,
   NiFpga_FlexRIOonlyResources_7965_ControlU32_SGUpdateRate0 = 0x4,
   NiFpga_FlexRIOonlyResources_7965_ControlU32_SGUpdateRate1 = 0x20,
   NiFpga_FlexRIOonlyResources_7965_ControlU32_TabControl = 0x3C4,
} NiFpga_FlexRIOonlyResources_7965_ControlU32;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO0 = 0x378,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO1 = 0x37C,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO10 = 0x364,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO11 = 0x360,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO12 = 0x35C,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO13 = 0x358,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO14 = 0x354,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO15 = 0x350,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO2 = 0x380,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO3 = 0x384,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO4 = 0x388,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO5 = 0x38C,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO6 = 0x374,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO7 = 0x370,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO8 = 0x36C,
   NiFpga_FlexRIOonlyResources_7965_ControlI64_aux64AO9 = 0x368,
} NiFpga_FlexRIOonlyResources_7965_ControlI64;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8_DMATtoHOSTFrameType = 0x396,
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x39A,
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8_FPGAVIversion = 0x3A6,
} NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x39E,
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU16_DMATtoHOSTNCh = 0x392,
} NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOonlyResources_7965_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOonlyResources_7965_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOonlyResources_7965_TargetToHostFifoU64;

#endif
