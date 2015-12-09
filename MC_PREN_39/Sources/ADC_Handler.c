/*
 * ADConverter.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "ADC_Handler.h"
#include "PE_Types.h"
#include "AD1.h"
#include "config.h"
#include "UART_Shell.h"

enum adStates_t{
	INIT,
	START_MEASUREMENT,
	IR_GET_DISTANCE,
	FLEX_GET_DISTANCE,
	EXIT
}adStates;

enum adChannels_t{
	AD_IR1,
	AD_FLEX1,
};

volatile uint16_t adValue[AD1_CHANNEL_COUNT];


void getDistance(void){
	static uint16_t IRvalue;
	switch (adStates){
	case INIT:
		debugPrintfInfraRedSensor("Init IR Distance\r\n");
		adStates=START_MEASUREMENT;
		break;
	case START_MEASUREMENT:
		(void)AD1_Measure(FALSE);
		adStates=IR_GET_DISTANCE;
		break;
	case IR_GET_DISTANCE:
		if(AD_finished){
			(void)AD1_GetValue16(&adValue[AD_IR1]); // get the result into value variable
			AD_finished=FALSE;
			debugPrintfInfraRedSensor("IR Distance (ADWert): %d\r\n",adValue[AD_IR1]);
			adStates=FLEX_GET_DISTANCE;
		}
		break;
	case FLEX_GET_DISTANCE:
		(void)AD1_GetValue16(&adValue[AD_FLEX1]); // get the result into value variable
		debugPrintfInfraRedSensor("Flex Distance (ADWert): %d\r\n",adValue[AD_FLEX1]);
		adStates=START_MEASUREMENT;
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

void debugPrintfFlexSensor(const char *fmt, ...) {
#if CFG_FLEXSENSOR_MSG
	debugPrintf(fmt);
#endif
}
