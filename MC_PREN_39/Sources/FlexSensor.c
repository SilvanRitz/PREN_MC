/*
 * InfraredSensor.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "FlexSensor.h"
#include "PE_Types.h"
#include "AD1.h"
#include "config.h"
#include "UART_Shell.h"

enum felxtates_t{
	INIT,
	START_MEASUREMENT,
	GET_DISTANCE,
	EXIT
}flexStates;



void getFlexDistance(void){
	static uint16_t flexValue;
	switch (flexStates){
	case INIT:
		debugPrintfFlexSensor("Init IR Distance\r\n");
		flexStates=START_MEASUREMENT;
		break;
	case START_MEASUREMENT:
		(void)AD1_Measure(FALSE);
		flexStates=GET_DISTANCE;
		break;
	case GET_DISTANCE:
		if(FLEX_AD_finished){
			(void)AD1_GetValue16(&flexValue); // get the result into value variable
			FLEX_AD_finished=FALSE;
			flexStates=START_MEASUREMENT;
			debugPrintfFlexSensor("Flex (ADWert): %d\r\n",flexValue);
		}
		break;
	case EXIT:
		debugPrintfFelxSensor("Exit\r\n");
		break;
	}
}


void debugPrintfFlexSensor(const char *fmt, ...) {
#if CFG_FLEXSENSOR_MSG
	debugPrintf(fmt);
#endif
}
