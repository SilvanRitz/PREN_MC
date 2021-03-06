/*
 * InfraredSensor.h
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_AUTONOM_ENTLADEN_H_
#define SOURCES_AUTONOM_ENTLADEN_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "PWM3.h"
#include "CLS1.h"
#include "UTIL1.h"

void debugPrintfAEntladen(const char *fmt, ...);
void autoEntladen(void);
uint8_t A_Entladen_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);


#endif /* SOURCES_AUTONOM_ENTLADEN_H_ */
