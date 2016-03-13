/*
 * UltraSonic.c
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

//#include "BitIoLdd1.h"
#include "PWRManagement.h"
#include "Bit_5V_2_Enable.h"
#include "config.h"
#include "UART_Shell.h"


static enum PWR_GenaralState_ {
  PWR_INIT, /* device not used */
  PWR_MEASURE,
  PWR_DECIDE, /* measuring echo pulse */
} PWR_GenaralState;

void startPWMonitoring(void){
	for(;;){
			switch (PWR_GenaralState){
			case PWR_INIT:
				PWRMan_Init();
				debugPrintfPWRMeasurement("PWR: Init done.\r\n");
				PWR_GenaralState=PWR_MEASURE;
			break;
			case PWR_MEASURE:
				//Get data from ADC (do not trigger ADC Measurement)
				PWR_GenaralState=PWR_DECIDE;
				break;
			case PWR_DECIDE:
				PWR_GenaralState=PWR_MEASURE;
				break;
			}
	}
}
void PWRMan_Init(void){
	Bit_5V_2_Enable_SetVal();	//activate second 5V Akku
}





void debugPrintfPWRMeasurement(const char *fmt, ...) {
#if CFG_PWR_MSG
	debugPrintf(fmt);
#endif
}
