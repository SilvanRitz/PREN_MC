/*
 * InfraredSensor.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "InfraredSensor.h"
#include "PE_Types.h"
#include "AD1.h"
#include "config.h"
#include "UART_Shell.h"

enum infraRedStates_t{
	INIT,
	START_MEASUREMENT,
	GET_DISTANCE,
	EXIT
}infraRedStates;




void getDistance(void){
	static uint16_t IRvalue;
	switch (infraRedStates){
	case INIT:
		debugPrintfInfraRedSensor("Init IR Distance\r\n");
		infraRedStates=START_MEASUREMENT;
		break;
	case START_MEASUREMENT:
		(void)AD1_Measure(FALSE);
		infraRedStates=GET_DISTANCE;
		break;
	case GET_DISTANCE:
		if(IR_AD_finished){
			(void)AD1_GetValue16(&IRvalue); // get the result into value variable
			IR_AD_finished=FALSE;
			infraRedStates=START_MEASUREMENT;
			debugPrintfInfraRedSensor("IR Distance (ADWert): %d\r\n",IRvalue);
		}
		break;
	case EXIT:
		debugPrintfInfraRedSensor("Exit\r\n");
		break;
	}
}


void debugPrintfInfraRedSensor(const char *fmt, ...) {
#if CFG_INFRAREDSENSOR_MSG
	debugPrintf(fmt);
#endif
}
