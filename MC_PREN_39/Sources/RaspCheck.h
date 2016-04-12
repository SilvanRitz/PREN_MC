/*
 * UltraSonic.h
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_RASP_CHECK
#define SOURCES_RASP_CHECK

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "CLS1.h"

void checkRasp(void);
void debugPrintfRaspCheck(const char *fmt, ...);
uint8_t Rasp_Check_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
#endif /* SOURCES_RASP_CHECK */
