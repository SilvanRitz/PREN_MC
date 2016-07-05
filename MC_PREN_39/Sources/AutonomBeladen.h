/*
 * AutonomBeladen.h
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_AUTONOM_BELADEN_H_
#define SOURCES_AUTONOM_BELADEN_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PWM3.h"
#include "CLS1.h"
#include "UTIL1.h"

//-----DC SPD---------

#define BELADEN_SPD					100



void debugPrintfABeladen(const char *fmt, ...);
void autoBeladen(void);
uint8_t A_Beladen_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
void beladen_Aufgerufen(uint16);
void setDistance(uint16 dist);

#endif /* SOURCES_AUTONOM_BELADEN_H_ */
