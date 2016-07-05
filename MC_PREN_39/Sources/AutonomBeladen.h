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

enum aBeladenStates_t{
	INIT,
	ENABLE_IR,
	CHECK_IR,
	CONTAINER_FOUND,
	HALT,
	GREIF_ZU_1,
	HEBEL_RUNTER_2,
	GREIF_AUF_3,
	HEBEL_RUNTER_4,
	GET_CONTAINER_HALB,
	GET_CONTAINER,
	EMPTY_CONTAINER,
	HEBEL_RUNTER_7,
	LOCKER_CONTAINER,
	CONTAINER_ABDRUECKEN,
	PLACE_CONTAINER,
	HEBEL_HOCH_9,
	GREIF_ZU_10,
	HEBEL_HOCH_11,
	GREIF_OFFEN_12,
	SEND_REPORT,
	EXIT
}volatile aBeladenStates;


void debugPrintfABeladen(const char *fmt, ...);
void autoBeladen(void);
uint8_t A_Beladen_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
void beladen_Aufgerufen(uint16);
void setDistance(uint16 dist);

#endif /* SOURCES_AUTONOM_BELADEN_H_ */
