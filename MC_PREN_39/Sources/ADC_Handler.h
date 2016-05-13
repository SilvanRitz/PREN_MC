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
void cmdPrintfAkku(const char *fmt, ...);
void handleADC(void);
void Lenk_pidDoWork(void);
uint16 lenkUpdateNomValue(void);
void checkAkku1();
void checkAkku2();
void flexAuswertung();
void cmdPrintfAkku(const char *fmt, ...);

#endif /* SOURCES_ADC_HANDLER_H_ */

