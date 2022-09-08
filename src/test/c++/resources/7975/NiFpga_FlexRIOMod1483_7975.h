/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOMod1483_7975_h__
#define __NiFpga_FlexRIOMod1483_7975_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOMod1483_7975_Bitfile;
 */
#define NiFpga_FlexRIOMod1483_7975_Bitfile "NiFpga_FlexRIOMod1483_7975.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOMod1483_7975_Signature = "DAAD18F2206A91994B4F25BD76186A2C";

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_AcqStatusAcqInProgress = 0x1000E,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_DI0 = 0x10026,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_DI1 = 0x1002A,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_InitDone = 0x10046,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_RIOAdapterCorrect = 0x100BE,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_uartBreakIndicator = 0x10092,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_uartFramingError = 0x10096,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_uartOverrunError = 0x1009A,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_uartRxReady = 0x1008E,
   NiFpga_FlexRIOMod1483_7975_IndicatorBool_uartTxReady = 0x1008A,
} NiFpga_FlexRIOMod1483_7975_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorU8_DevProfile = 0x100C2,
   NiFpga_FlexRIOMod1483_7975_IndicatorU8_DevQualityStatus = 0x100C6,
   NiFpga_FlexRIOMod1483_7975_IndicatorU8_Platform = 0x100CA,
   NiFpga_FlexRIOMod1483_7975_IndicatorU8_SGNo = 0x1005A,
   NiFpga_FlexRIOMod1483_7975_IndicatorU8_uartRxByte = 0x10086,
} NiFpga_FlexRIOMod1483_7975_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorI16_DevTemp = 0x1004E,
   NiFpga_FlexRIOMod1483_7975_IndicatorI16_auxAI0 = 0x1002E,
   NiFpga_FlexRIOMod1483_7975_IndicatorI16_auxAI1 = 0x10036,
} NiFpga_FlexRIOMod1483_7975_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorU16_DMATtoHOSTOverflows = 0x10042,
} NiFpga_FlexRIOMod1483_7975_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorU32_AcqStatusClocksPerLine = 0x10010,
   NiFpga_FlexRIOMod1483_7975_IndicatorU32_AcqStatusFramesAcqd = 0x10018,
   NiFpga_FlexRIOMod1483_7975_IndicatorU32_AcqStatusLinesPerFrame = 0x10014,
   NiFpga_FlexRIOMod1483_7975_IndicatorU32_Fref = 0x100B0,
   NiFpga_FlexRIOMod1483_7975_IndicatorU32_InsertedIOModuleID = 0x100B8,
} NiFpga_FlexRIOMod1483_7975_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_ControlBool_AcqControlIgnoreDVAL = 0x10002,
   NiFpga_FlexRIOMod1483_7975_ControlBool_ControlEnable = 0x1009E,
   NiFpga_FlexRIOMod1483_7975_ControlBool_DAQStartStop = 0x1004A,
   NiFpga_FlexRIOMod1483_7975_ControlBool_DMATtoHOSTEnable0 = 0x1003E,
   NiFpga_FlexRIOMod1483_7975_ControlBool_DO0 = 0x1001E,
   NiFpga_FlexRIOMod1483_7975_ControlBool_DO1 = 0x10022,
   NiFpga_FlexRIOMod1483_7975_ControlBool_DVALHigh = 0x10056,
   NiFpga_FlexRIOMod1483_7975_ControlBool_DebugMode = 0x10052,
   NiFpga_FlexRIOMod1483_7975_ControlBool_FVALHigh = 0x1005E,
   NiFpga_FlexRIOMod1483_7975_ControlBool_LVALHigh = 0x10062,
   NiFpga_FlexRIOMod1483_7975_ControlBool_LineScan = 0x1003A,
   NiFpga_FlexRIOMod1483_7975_ControlBool_SpareHigh = 0x10066,
   NiFpga_FlexRIOMod1483_7975_ControlBool_uartByteMode = 0x1007A,
   NiFpga_FlexRIOMod1483_7975_ControlBool_uartReceive = 0x10076,
   NiFpga_FlexRIOMod1483_7975_ControlBool_uartSetBaudRate = 0x1007E,
   NiFpga_FlexRIOMod1483_7975_ControlBool_uartTransmit = 0x10072,
} NiFpga_FlexRIOMod1483_7975_ControlBool;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_ControlU8_Configuration = 0x1006E,
   NiFpga_FlexRIOMod1483_7975_ControlU8_SignalMapping = 0x1006A,
   NiFpga_FlexRIOMod1483_7975_ControlU8_uartBaudRate = 0x10082,
   NiFpga_FlexRIOMod1483_7975_ControlU8_uartTxByte = 0x100A2,
} NiFpga_FlexRIOMod1483_7975_ControlU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_ControlI16_auxAO0 = 0x10032,
} NiFpga_FlexRIOMod1483_7975_ControlI16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_ControlU32_AcqControlFramesToAcq = 0x10004,
   NiFpga_FlexRIOMod1483_7975_ControlU32_AcqControlLineScanHeight = 0x10008,
   NiFpga_FlexRIOMod1483_7975_ControlU32_TabControl = 0x100CC,
} NiFpga_FlexRIOMod1483_7975_ControlU32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8_DMATtoHOSTFrameType = 0x100AA,
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x100AE,
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8_FPGAVIversion = 0x100B6,
} NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOMod1483_7975_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU16_DMATtoHOSTNCh = 0x100A6,
} NiFpga_FlexRIOMod1483_7975_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOMod1483_7975_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOMod1483_7975_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOMod1483_7975_TargetToHostFifoU64;

#endif
