/*
 * InfraredSensor.h
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_SERVO_PARSE_H_
#define SOURCES_SERVO_PARSE_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "CLS1.h"
#include "UTIL1.h"

#define CAM_SERVO_GERADE	120
//#define CAM_SERVO_RECHTS	70		//entspricht 30°
//#define CAM_SERVO_LINKS		170		//entspricht 30

void debugPrintfServoParse(const char *fmt, ...);
uint8_t SERVO_Lenkung_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
uint8_t SERVO_Kamera_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
#endif /* SOURCES_DCDRIVE_H_ */
