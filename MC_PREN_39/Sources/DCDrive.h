/*
 * InfraredSensor.h
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_DCDRIVE_H_
#define SOURCES_DCDRIVE_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "PWM3.h"
#include "CLS1.h"
#include "UTIL1.h"

void debugPrintfDCDrive(const char *fmt, ...);
void DCDhandleSpeed(void);
uint8_t PWM3_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
void setNomValue(uint16 actSpeed);

#endif /* SOURCES_DCDRIVE_H_ */
