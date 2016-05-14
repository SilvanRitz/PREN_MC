/*
 * InfraredSensor.h
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_HANDLE_ACTIONS_H_
#define SOURCES_HANDLE_ACTIONS_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "PWM3.h"
#include "CLS1.h"
#include "UTIL1.h"

typedef enum {
	INIT_ALL,
	INIT_DONE,
	DRIVE,
	BELADEN,
	ENTLADEN,
	FERTIG,
	AKKU_LEER
}handle_actions_t;

void debugPrintfHandleActions(const char *fmt, ...);
void changeToInitDone(void);
void changeToFertig(void);
void changeToDrive(void);
void changeToEntladen(void);
void changeToBeladen(void);
void changeToAkkuLeer(void);
handle_actions_t getHandleActionsState();
void handleActions(void);
uint8_t start_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
uint8_t stopp_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
void LED_Blink(void);


#endif /* SOURCES_HANDLE_ACTIONS_H_ */
