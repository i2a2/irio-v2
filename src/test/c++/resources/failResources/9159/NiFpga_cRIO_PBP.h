/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_cRIO_PBP_h__
#define __NiFpga_cRIO_PBP_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_cRIO_PBP_Bitfile;
 */
#define NiFpga_cRIO_PBP_Bitfile "NiFpga_cRIO_PBP.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_cRIO_PBP_Signature = "74080495E2F9BEE4BFA3362026D19049";

typedef enum
{
   NiFpga_cRIO_PBP_IndicatorBool_DI0 = 0x814E,
   NiFpga_cRIO_PBP_IndicatorBool_DI1 = 0x814A,
   NiFpga_cRIO_PBP_IndicatorBool_auxDI0 = 0x8146,
   NiFpga_cRIO_PBP_IndicatorBool_auxDI1 = 0x8142,
} NiFpga_cRIO_PBP_IndicatorBool;

typedef enum
{
   NiFpga_cRIO_PBP_IndicatorU8_SGNo = 0x813E,
} NiFpga_cRIO_PBP_IndicatorU8;

typedef enum
{
   NiFpga_cRIO_PBP_IndicatorI32_AI0 = 0x8184,
   NiFpga_cRIO_PBP_IndicatorI32_AI1 = 0x8180,
   NiFpga_cRIO_PBP_IndicatorI32_auxAI0 = 0x817C,
   NiFpga_cRIO_PBP_IndicatorI32_auxAI1 = 0x8178,
} NiFpga_cRIO_PBP_IndicatorI32;

typedef enum
{
   NiFpga_cRIO_PBP_IndicatorU32_SGFref0 = 0x8128,
   NiFpga_cRIO_PBP_IndicatorU32_SGFref1 = 0x8110,
} NiFpga_cRIO_PBP_IndicatorU32;

typedef enum
{
   NiFpga_cRIO_PBP_ControlBool_AOEnable0 = 0x8166,
   NiFpga_cRIO_PBP_ControlBool_AOEnable1 = 0x8162,
   NiFpga_cRIO_PBP_ControlBool_DO0 = 0x815E,
   NiFpga_cRIO_PBP_ControlBool_DO1 = 0x815A,
   NiFpga_cRIO_PBP_ControlBool_auxDO0 = 0x8156,
   NiFpga_cRIO_PBP_ControlBool_auxDO1 = 0x8152,
} NiFpga_cRIO_PBP_ControlBool;

typedef enum
{
   NiFpga_cRIO_PBP_ControlU8_SGSignalType0 = 0x812E,
   NiFpga_cRIO_PBP_ControlU8_SGSignalType1 = 0x8116,
} NiFpga_cRIO_PBP_ControlU8;

typedef enum
{
   NiFpga_cRIO_PBP_ControlU16_SGAmp0 = 0x813A,
   NiFpga_cRIO_PBP_ControlU16_SGAmp1 = 0x8126,
   NiFpga_cRIO_PBP_ControlU16_SamplingRate1 = 0x810E,
} NiFpga_cRIO_PBP_ControlU16;

typedef enum
{
   NiFpga_cRIO_PBP_ControlI32_AO0 = 0x8174,
   NiFpga_cRIO_PBP_ControlI32_AO1 = 0x8170,
   NiFpga_cRIO_PBP_ControlI32_auxAO0 = 0x816C,
   NiFpga_cRIO_PBP_ControlI32_auxAO1 = 0x8168,
} NiFpga_cRIO_PBP_ControlI32;

typedef enum
{
   NiFpga_cRIO_PBP_ControlU32_SGFreq0 = 0x8134,
   NiFpga_cRIO_PBP_ControlU32_SGFreq1 = 0x8120,
   NiFpga_cRIO_PBP_ControlU32_SGPhase1 = 0x811C,
   NiFpga_cRIO_PBP_ControlU32_SGUpdateRate0 = 0x8130,
   NiFpga_cRIO_PBP_ControlU32_SGUpdateRate1 = 0x8118,
} NiFpga_cRIO_PBP_ControlU32;

#endif
