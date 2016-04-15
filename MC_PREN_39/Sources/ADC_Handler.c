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
#include "LENK_SERVO2.h"

enum adStates_t{
	INIT,
	START_MEASUREMENT,
	IR_GET_DISTANCE,
	FLEX_GET_DISTANCE,
	EXIT
}adStates;

enum adChannels_t{
	AD_AKKU_5V_2,
	AD_FLEX1,
	AD_FLEX2,
	AD_AKKU_5V_1
};

#define FLEX1_MSG_CMD		"Fldist1"

#define LENK_OFFSET		126
#define IST_WERT_OFFSET	46000
#define SOLLWERT		8000				//Sollwert bereits abgezogen

uint16 adValue[AD1_CHANNEL_COUNT];
uint16 istWert=0;

//PID
static uint16 nomValue=0;		//OFF
static uint16 nomValueOld=0;		//istwert
static uint16 setValue=SOLLWERT;
static uint16 setValueOld;		//sollwert
static int16 integ, devOld;
static int16 val,dev;
static uint8 delay=0;
static uint8 kp=120;		//10
static uint8 ki=20;			//5
static uint8 kd=60;			//1


void handleADC(void){
	static uint16_t IRvalue;
	switch (adStates){
	case INIT:
		debugPrintfInfraRedSensor("%s :Init AD Converter\r\n",DEBUG_MSG_CMD);
		adStates=START_MEASUREMENT;
		break;
	case START_MEASUREMENT:
		(void)AD1_Measure(FALSE);
		adStates=IR_GET_DISTANCE;
		break;
	case IR_GET_DISTANCE:
		if(AD_finished){
			(void)AD1_GetValue16(&adValue[AD_AKKU_5V_2]); // get the result into value variable
			AD_finished=FALSE;
			debugPrintfInfraRedSensor("IR Distance (ADWert): %d\r\n",adValue[AD_AKKU_5V_2]);
			adStates=FLEX_GET_DISTANCE;
		}
		break;
	case FLEX_GET_DISTANCE:
		istWert=adValue[AD_FLEX1]-IST_WERT_OFFSET;
		debugPrintfFlexSensor("%s: %d\r\n",FLEX1_MSG_CMD,istWert);
		Lenk_pidDoWork();
		adStates=START_MEASUREMENT;
		break;
	case EXIT:
		debugPrintfInfraRedSensor("Exit\r\n");
		break;
	}
}
uint16 lenkUpdateNomValue(void){
	nomValue=istWert;
	return nomValue;
}

void debugPrintfInfraRedSensor(const char *fmt, ...) {
#if CFG_ADC_MSG
	debugPrintf(fmt);
#endif
}

void debugPrintfFlexSensor(const char *fmt, ...) {
#if CFG_FLEXSENSOR_DBG
	debugPrintf(fmt);
#endif
}

void cmdPrintfFlexSensor(const char *fmt, ...) {
	#if CFG_FLEXSENSOR_CMD
		debugPrintf(fmt);
	#endif
}

void Lenk_pidDoWork(void)
{
	// readSpeed from Encoder(in value nomValue)
	nomValueOld = nomValue;			//ist Wert (old)
	nomValue=lenkUpdateNomValue();


  nomValue = (nomValueOld+nomValue) >> 1;



  if (setValue == 0) val = integ = devOld = 0;
  else
  {
    // deviation (max devL = +1000 - -1000 = 2000)
    //dev = (setValue - nomValue) / 8;
	 dev = (setValue - nomValue)/32;
    // P-Part (max kpL =
    val = (kp * dev) / 32;

    // I-Part with anti-windup
    if (ki != 0) integ += dev;
    val += (ki * integ) / 32;
    //val+=(ki*integ)/32;

    // D-Part
    val += (kd*(dev-devOld)/32);
    devOld = dev;
    val=val/64;
    // limit control point
    if (val > 125)
    {
      val = 100;
      integ -= dev;
    }
    else if (val < -125)
    {
      val = -125;
      integ -= dev;
    }
  }
  LENK_SERVO2_SetPos(LENK_OFFSET+val);
  debugPrintfFlexSensor("%s :Gradwert %i\r\n",DEBUG_MSG_CMD,LENK_OFFSET+val);
  //setDutyCycle(100-val);
}
