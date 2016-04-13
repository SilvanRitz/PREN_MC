/*
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_ADC_HANDLER_H_
#define SOURCES_ADC_HANDLER_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

void debugPrintfInfraRedSensor(const char *fmt, ...);
void debugPrintfFlexSensor(const char *fmt, ...);
void cmdPrintfFlexSensor(const char *fmt, ...);
void handleADC(void);
void Lenk_pidDoWork(void);
uint16 lenkUpdateNomValue(void);

#endif /* SOURCES_ADC_HANDLER_H_ */

