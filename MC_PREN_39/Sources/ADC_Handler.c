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
#include "handleActions.h"

enum adStates_t{
	INIT,
	START_MEASUREMENT,
	VERARBEITE,
	EXIT
}adStates;

enum adChannels_t{
	AD_AKKU_5V_2,
	AD_FLEX1,
	AD_FLEX2,
	AD_AKKU_5V_1,
	AD_FLEX_REF
};

#define FLEX1_MSG_CMD		"Fld1"
#define FLEX2_MSG_CMD		"Fl_Ref"

#define AKKU1_MSG_CMD		"Akku1"
#define AKKU2_MSG_CMD		"Akku2"

#define LENK_OFFSET		126
#define IST_WERT_OFFSET	46000
#define SOLLWERT		50				//Sollwert bereits abgezogen

#define AKKU1_SCHWELLWERT	31513//31513//38673	//30222 alt
#define AKKU2_SCHWELLWERT	41592


//FLEXSENSOR Werte
#define MAXDIST			80		//distance in mm¨
#define	MINVAL_FLEX		1000
#define MAXVAL_FLEX		14000

int16 adValue[AD1_CHANNEL_COUNT];
uint16 uAdValue[AD1_CHANNEL_COUNT];
uint16 volatile istWert=0;

//PID
static uint16 nomValue=0;		//OFF
static uint16 nomValueOld=0;		//istwert
static uint16 setValue=SOLLWERT;
static uint16 setValueOld;		//sollwert
static int16 integ, devOld;
static int16 val,dev;
static uint8 delay=0;
static uint8 kp1=5;		//10
static uint8 kp2=4;		//10

static uint8 ki=0;			//5
static uint8 kd=0;			//1

static uint8 lowAkku2Counter=0;
static uint8 lowAkku1Counter=0;

void handleADC(void){
	static uint16_t IRvalue;
	switch (adStates){
	case INIT:
		debugPrintfInfraRedSensor("%s :Init AD Converter\r\n",DEBUG_MSG_CMD);
		adStates=START_MEASUREMENT;
		break;
	case START_MEASUREMENT:
		(void)AD1_Measure(FALSE);

		adStates=VERARBEITE;
		break;
	case VERARBEITE:
		if(AD_finished){
			AD_finished=FALSE;
			(void)AD1_GetValue16(adValue); // get the result into value variable
			uAdValue[AD_AKKU_5V_1]=(unsigned)adValue[AD_AKKU_5V_1];
			uAdValue[AD_AKKU_5V_2]=(unsigned)adValue[AD_AKKU_5V_2];
			checkAkku1();

			checkAkku2();

			//FLEX Sensor
			flexAuswertung();

			adStates=START_MEASUREMENT;
		}

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
#if CFG_IR_CMD
	debugPrintf(fmt);
#endif
}

void cmdPrintfAkku(const char *fmt, ...) {
#if CFG_AKKU
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
	handle_actions_t actionsState=getHandleActionsState();
	if(actionsState!=INIT_ALL && actionsState!=FERTIG && actionsState!=AKKU_LEER){	// ||actionsState==INIT_ALL
		debugPrintf(fmt);
	}
	#endif
}

void checkAkku1(){
	//Akku 1 (7.1V)
	//debugPrintfFlexSensor("%s: %d\r\n",AKKU1_MSG_CMD,uAdValue[AD_AKKU_5V_1]);
	if(uAdValue[AD_AKKU_5V_1]<AKKU1_SCHWELLWERT){
		lowAkku1Counter++;
		if (lowAkku1Counter>100){
			cmdPrintfAkku("%s: Akku1 leer %d\r\n",AKKU1_MSG_CMD,uAdValue[AD_AKKU_5V_1]);
#if AKKU_ABSCHALTEN
			changeToAkkuLeer();
#endif
		}
	}
	else{
		lowAkku1Counter=0;
	}
}

void checkAkku2(){
	//Akku 2 (11V)
	//debugPrintfFlexSensor("%s: %d\r\n",AKKU2_MSG_CMD,uAdValue[AD_AKKU_5V_2]);
	if(uAdValue[AD_AKKU_5V_2]<AKKU2_SCHWELLWERT){
		lowAkku2Counter++;
		if (lowAkku2Counter>100){
			cmdPrintfAkku("%s: Akku2 leer %d\r\n",AKKU2_MSG_CMD,uAdValue[AD_AKKU_5V_2]);
#if AKKU_ABSCHALTEN
			changeToAkkuLeer();
#endif
		}
	}
	else{
		lowAkku2Counter=0;
	}
}

void Lenk_pidDoWork(void)
{
	// readSpeed from Encoder(in value nomValue)
	nomValueOld = nomValue;			//ist Wert (old)
	nomValue=lenkUpdateNomValue();
  //nomValue = (nomValueOld+nomValue);
  if (setValue == 0) val = integ = devOld = 0;
  else
  {
    // deviation (max devL = +1000 - -1000 = 2000)
    //dev = (setValue - nomValue) / 8;
	 dev = (setValue - nomValue);
    // P-Part (max kpL =
	 if(dev>0){
		 val = (kp2 * dev);
	 }
	 else{
		 val = (kp1 * dev);
	 }

    // I-Part with anti-windup
    if (ki != 0) integ += dev;
    val += (ki * integ)/64;
    //val+=(ki*integ)/32;

    // D-Part
    val += (kd*(dev-devOld))/64;
    devOld = dev;
    val=val;
    // limit control point
    if (val > 45)
    {
      val = 45;
      integ -= dev;
    }
    else if (val < -45)
    {
      val = -45;
      integ -= dev;
    }
  }
  LENK_SERVO2_SetPos(LENK_OFFSET-val);
  debugPrintfFlexSensor("%s :Gradwert %i\r\n",DEBUG_MSG_CMD,LENK_OFFSET-val);
  //setDutyCycle(100-val);
}

void flexAuswertung(){
	int16 temp=0;
	//istWert=adValue[AD_FLEX1]-adValue[AD_FLEX_REF];
	//debugPrintfFlexSensor("%s: %d\r\n",FLEX1_MSG_CMD,adValue[AD_FLEX1]);
	//debugPrintfFlexSensor("%s: %d\r\n",FLEX2_MSG_CMD,adValue[AD_FLEX2]);
	temp=adValue[AD_FLEX2]-adValue[AD_FLEX1]+2000;
	debugPrintfFlexSensor("FLEX_DIFF: %d\r\n",temp);
	if(temp>0){
		istWert=temp;
	}
	else{
		istWert=0;
	}
	calcDistanceFlex();
	debugPrintfFlexSensor("FLEX_DIFF: %d\r\n",istWert);
#if FLEX_LENK_ENABLE
			Lenk_pidDoWork();
#endif

}

void calcDistanceFlex(void){
	uint8 distance=0;
	if (istWert<MINVAL_FLEX){
		distance=MAXDIST;
	}
	else if(istWert>MAXVAL_FLEX){
		distance=0;
	}
	else{
		distance=MAXDIST-(MAXDIST*(istWert-MINVAL_FLEX))/(MAXVAL_FLEX-MINVAL_FLEX);
	}
	istWert=distance;
	cmdPrintfFlexSensor("%s: %d\r\n",FLEX1_MSG_CMD,distance);
	debugPrintfFlexSensor("Distance mm: %d\r\n",distance);
}
