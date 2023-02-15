/*
 * Generated with the FPGA Interface C API Generator 19.0
 * for NI-RIO 19.0 or later.
 */
#ifndef __NiFpga_Rseries_onlyResources_7854_h__
#define __NiFpga_Rseries_onlyResources_7854_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 190
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Rseries_onlyResources_7854_Bitfile;
 */
#define NiFpga_Rseries_onlyResources_7854_Bitfile "NiFpga_Rseries_onlyResources_7854.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Rseries_onlyResources_7854_Signature = "45C89DDB2212D5A794FBAD4F69E5494F";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI0 = 0x82B6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI1 = 0x82B2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI10 = 0x8256,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI11 = 0x8252,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI12 = 0x8276,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI13 = 0x8272,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI14 = 0x826E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI15 = 0x826A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI16 = 0x83B2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI17 = 0x83B6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI18 = 0x82EA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI19 = 0x82EE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI2 = 0x82AE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI20 = 0x82F2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI21 = 0x82F6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI22 = 0x82FA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI23 = 0x82FE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI24 = 0x833A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI25 = 0x833E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI26 = 0x8342,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI27 = 0x8346,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI28 = 0x834A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI29 = 0x834E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI3 = 0x82AA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI30 = 0x832A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI31 = 0x832E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI32 = 0x8332,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI33 = 0x8336,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI34 = 0x83AA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI35 = 0x83AE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI36 = 0x836A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI37 = 0x836E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI38 = 0x8372,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI39 = 0x8376,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI4 = 0x82A6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI40 = 0x837A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI41 = 0x837E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI42 = 0x83BA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI43 = 0x83BE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI44 = 0x83C2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI45 = 0x83C6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI46 = 0x83CA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI47 = 0x83CE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI48 = 0x83F6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI49 = 0x83FA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI5 = 0x82A2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI50 = 0x83FE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI51 = 0x83EA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI52 = 0x83EE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI53 = 0x83F2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI6 = 0x8266,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI7 = 0x8262,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI8 = 0x825E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_DI9 = 0x825A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_InitDone = 0x84AA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_RIOAdapterCorrect = 0x84AE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI0 = 0x8146,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI1 = 0x8142,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI10 = 0x81DE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI11 = 0x81E2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI12 = 0x81BE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI13 = 0x81C2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI14 = 0x81C6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI15 = 0x81CA,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI2 = 0x813E,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI3 = 0x813A,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI4 = 0x8136,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI5 = 0x8132,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI6 = 0x81CE,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI7 = 0x81D2,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI8 = 0x81D6,
   NiFpga_Rseries_onlyResources_7854_IndicatorBool_auxDI9 = 0x81DA,
} NiFpga_Rseries_onlyResources_7854_IndicatorBool;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorU8_DevProfile = 0x849E,
   NiFpga_Rseries_onlyResources_7854_IndicatorU8_DevQualityStatus = 0x84C6,
   NiFpga_Rseries_onlyResources_7854_IndicatorU8_Platform = 0x84CA,
   NiFpga_Rseries_onlyResources_7854_IndicatorU8_SGNo = 0x8112,
} NiFpga_Rseries_onlyResources_7854_IndicatorU8;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorI16_DevTemp = 0x84C2,
} NiFpga_Rseries_onlyResources_7854_IndicatorI16;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorU16_DMATtoHOSTOverflows = 0x848E,
} NiFpga_Rseries_onlyResources_7854_IndicatorU16;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI0 = 0x8174,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI1 = 0x8170,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI10 = 0x821C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI11 = 0x8220,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI12 = 0x8224,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI13 = 0x8228,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI14 = 0x822C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI15 = 0x8230,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI2 = 0x816C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI3 = 0x8168,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI4 = 0x8164,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI5 = 0x8160,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI6 = 0x820C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI7 = 0x8210,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI8 = 0x8214,
   NiFpga_Rseries_onlyResources_7854_IndicatorI32_auxAI9 = 0x8218,
} NiFpga_Rseries_onlyResources_7854_IndicatorI32;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorU32_ExpectedIOModuleID = 0x84B4,
   NiFpga_Rseries_onlyResources_7854_IndicatorU32_Fref = 0x84A4,
   NiFpga_Rseries_onlyResources_7854_IndicatorU32_InsertedIOModuleID = 0x84B0,
   NiFpga_Rseries_onlyResources_7854_IndicatorU32_SGFref0 = 0x8190,
   NiFpga_Rseries_onlyResources_7854_IndicatorU32_SGFref1 = 0x810C,
} NiFpga_Rseries_onlyResources_7854_IndicatorU32;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI0 = 0x8428,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI1 = 0x842C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI10 = 0x8414,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI11 = 0x8410,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI12 = 0x840C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI13 = 0x8408,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI14 = 0x8404,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI15 = 0x8400,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI2 = 0x8430,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI3 = 0x8434,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI4 = 0x8438,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI5 = 0x843C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI6 = 0x8424,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI7 = 0x8420,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI8 = 0x841C,
   NiFpga_Rseries_onlyResources_7854_IndicatorI64_aux64AI9 = 0x8418,
} NiFpga_Rseries_onlyResources_7854_IndicatorI64;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_ControlBool_AOEnable0 = 0x8116,
   NiFpga_Rseries_onlyResources_7854_ControlBool_AOEnable1 = 0x8236,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DAQStartStop = 0x84BA,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DMATtoHOSTEnable0 = 0x8486,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO0 = 0x82CE,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO1 = 0x82CA,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO10 = 0x828E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO11 = 0x8292,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO12 = 0x8286,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO13 = 0x8282,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO14 = 0x827E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO15 = 0x827A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO16 = 0x82E2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO17 = 0x82DE,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO18 = 0x82D2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO19 = 0x82D6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO2 = 0x82C6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO20 = 0x82DA,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO21 = 0x82E6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO22 = 0x8312,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO23 = 0x830E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO24 = 0x8302,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO25 = 0x8306,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO26 = 0x830A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO27 = 0x8316,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO28 = 0x8362,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO29 = 0x835E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO3 = 0x82BA,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO30 = 0x831A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO31 = 0x831E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO32 = 0x8322,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO33 = 0x8326,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO34 = 0x8392,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO35 = 0x838E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO36 = 0x8352,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO37 = 0x8356,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO38 = 0x835A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO39 = 0x8366,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO4 = 0x82BE,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO40 = 0x83D6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO41 = 0x83DA,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO42 = 0x8382,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO43 = 0x8386,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO44 = 0x838A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO45 = 0x8396,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO46 = 0x83E2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO47 = 0x83E6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO48 = 0x839A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO49 = 0x839E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO5 = 0x82C2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO50 = 0x83A2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO51 = 0x83A6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO52 = 0x83D2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO53 = 0x83DE,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO6 = 0x829E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO7 = 0x829A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO8 = 0x8296,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DO9 = 0x828A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_DebugMode = 0x84BE,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO0 = 0x815E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO1 = 0x815A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO10 = 0x81A6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO11 = 0x81A2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO12 = 0x81AE,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO13 = 0x81B2,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO14 = 0x81B6,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO15 = 0x81BA,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO2 = 0x8156,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO3 = 0x814A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO4 = 0x814E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO5 = 0x8152,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO6 = 0x8196,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO7 = 0x819A,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO8 = 0x819E,
   NiFpga_Rseries_onlyResources_7854_ControlBool_auxDO9 = 0x81AA,
} NiFpga_Rseries_onlyResources_7854_ControlBool;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_ControlU8_SGSignalType0 = 0x811E,
   NiFpga_Rseries_onlyResources_7854_ControlU8_SGSignalType1 = 0x823E,
} NiFpga_Rseries_onlyResources_7854_ControlU8;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_ControlU16_DMATtoHOSTSamplingRate0 = 0x848A,
   NiFpga_Rseries_onlyResources_7854_ControlU16_SGAmp0 = 0x812E,
   NiFpga_Rseries_onlyResources_7854_ControlU16_SGAmp1 = 0x824E,
} NiFpga_Rseries_onlyResources_7854_ControlU16;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_ControlI32_AO0 = 0x8120,
   NiFpga_Rseries_onlyResources_7854_ControlI32_AO1 = 0x8240,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO0 = 0x818C,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO1 = 0x8188,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO10 = 0x81F4,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO11 = 0x81F8,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO12 = 0x81FC,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO13 = 0x8200,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO14 = 0x8204,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO15 = 0x8208,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO2 = 0x8184,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO3 = 0x8180,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO4 = 0x817C,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO5 = 0x8178,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO6 = 0x81E4,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO7 = 0x81E8,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO8 = 0x81EC,
   NiFpga_Rseries_onlyResources_7854_ControlI32_auxAO9 = 0x81F0,
} NiFpga_Rseries_onlyResources_7854_ControlI32;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_ControlU32_SGFreq0 = 0x8128,
   NiFpga_Rseries_onlyResources_7854_ControlU32_SGFreq1 = 0x8248,
   NiFpga_Rseries_onlyResources_7854_ControlU32_SGPhase0 = 0x8124,
   NiFpga_Rseries_onlyResources_7854_ControlU32_SGPhase1 = 0x8244,
   NiFpga_Rseries_onlyResources_7854_ControlU32_SGUpdateRate0 = 0x8118,
   NiFpga_Rseries_onlyResources_7854_ControlU32_SGUpdateRate1 = 0x8238,
   NiFpga_Rseries_onlyResources_7854_ControlU32_TabControl = 0x84D0,
} NiFpga_Rseries_onlyResources_7854_ControlU32;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO0 = 0x8468,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO1 = 0x846C,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO11 = 0x8450,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO12 = 0x844C,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO13 = 0x8448,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO14 = 0x8444,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO15 = 0x8440,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO2 = 0x8470,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO3 = 0x8474,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO4 = 0x8478,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO5 = 0x847C,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO6 = 0x8464,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO7 = 0x8460,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO8 = 0x845C,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux64AO9 = 0x8458,
   NiFpga_Rseries_onlyResources_7854_ControlI64_aux65AO10 = 0x8454,
} NiFpga_Rseries_onlyResources_7854_ControlI64;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8_DMATtoHOSTFrameType = 0x8492,
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x8496,
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8_FPGAVIversion = 0x84A2,
} NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_Rseries_onlyResources_7854_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x849A,
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU16_DMATtoHOSTNCh = 0x8482,
} NiFpga_Rseries_onlyResources_7854_IndicatorArrayU16;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_Rseries_onlyResources_7854_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_Rseries_onlyResources_7854_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_Rseries_onlyResources_7854_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_Rseries_onlyResources_7854_TargetToHostFifoU64;

#if !NiFpga_VxWorks

/* Control: Clusterin */
/* Use NiFpga_WriteArrayU8() to access Clusterin */
const uint32_t NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_Resource = 0x84CC;
const uint32_t NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_PackedSizeInBytes = 51;

typedef struct NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_Type{
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
}NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_Type;


void NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_Type* const destination);

void NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Rseries_onlyResources_7854_ControlCluster_Clusterin_Type* const source);

#endif /* !NiFpga_VxWorks */


#if NiFpga_Cpp
}
#endif

#endif
