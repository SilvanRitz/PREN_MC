/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "DCDrive.h"
#include "Bit_DC_Vor.h"
#include "Bit_DC_Ruck.h"
#include "UART_Shell.h"
#include "DebugBit.h"




//Shell
#define DCDRIVE_MSG_CMD				"DCDr"		//Shellout

#define DCDRIVE_SHELL_NAME_STR		"DCDr"
#define DCDR_CMD_HELP   			"help"
#define DCDR_CMD_STATUS 			"status"
#define DCDRIVE_CMD_SPEED			"spd"
#define DC_MAXSPEED					190
#define DC_MAXSPEED_STR				"190"


#define TIMER_PERIOD_DUR			50		//in us
#define PWM3_PERIOD_VALUE_PROZENT 	TIMER_PERIOD_DUR/100



#define TICKS_MAXSPEED				110
#define DC_SPD_PER_TICK				TICKS_
#define PWM3_DUTY_MULT				0x28F


enum DCPwr{
	SOFT,
	HARD
};

static uint16 actSpeedValue=0;

static uint16 nomValue=0;		//OFF
static uint16 nomValueOld=0;		//istwert
static uint16 setValue,setValueOld;		//sollwert
static int16 integ, devOld;
static int16 val,dev;
static uint8 delay=0;
static uint8 kp=20;		//10
static uint8 ki=10;			//5
static uint8 kd=2;			//1

/*  kiL = kiR = 10; //10 max 20
  kpL = kpR = 60; //60 max 128
  kdL = kdR = 0; //10  max 40*/

enum dcDriveStates_t{
	INIT,
	HANDLE_SPEED,
	EXIT
}dcDriveStates=INIT;


void DCDhandleSpeed(void){
	switch (dcDriveStates){
	case INIT:
		debugPrintfDCDrive("%s %s: intialized\r\n",DEBUG_MSG_CMD,DCDRIVE_MSG_CMD);
		dcDriveStates=HANDLE_SPEED;
		Bit_DC_Vor_SetVal();
		//setDutyCycle(0);
		setDCSpeed(20);
		break;
	case HANDLE_SPEED:
		pidDoWork();

		//getCommands();
		break;
	case EXIT:
		debugPrintfDCDrive("Exit\r\n");
		break;
	}
}


void debugPrintfDCDrive(const char *fmt, ...) {
#if CFG_DCDRIVE_MSG
	debugPrintf(fmt);
#endif
}

void setDutyCycle(unsigned int val){	//get called by Shell
	static unsigned int debug=0;
	debugPrintfDCDrive("%s Der Wert ist: %d\r\n", DEBUG_MSG_CMD,val);
	//PWM3_SetDutyUS((uint16_t)(val*PWM3_PERIOD_VALUE_PROZENT));
	PWM3_SetRatio16((uint16_t)val*PWM3_DUTY_MULT);
	if (debug==1){
		debug=0;
		DebugBit_PutVal(0);
	}
	else{
		debug=1;
		DebugBit_PutVal(1);
	}
}

void setDCSpeed(uint16 speed){
	setValue=speed*TICKS_MAXSPEED/DC_MAXSPEED;	//Umrechnung geschwindigkeit in sollspeed

	//update PID Regler
}

void setNomValue(uint16 actSpeed){
	actSpeedValue=actSpeed;
}

uint16 updateNomValue(){
	nomValue=actSpeedValue;
	return nomValue;
}

//--------Shellpart------------
uint8_t PWM3_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, DCDR_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, DCDRIVE_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)DCDRIVE_SHELL_NAME_STR, (unsigned char*)"Group of "DCDRIVE_SHELL_NAME_STR" commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  pos", (unsigned char*)"Values 0..100 ändert den dutycycle (100 = off)\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  vdir", (unsigned char*)"Drehe in Vorwärtsrichtung\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  rdir", (unsigned char*)"Drehe in Rückwärtsrichtung\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  spd", (unsigned char*)"Gibt die Geschwindigkeit in mm pro s an\nWerte zwischen 0 und "DC_MAXSPEED_STR"\r\n", io->stdOut);
		return ERR_OK;
  } else if ((UTIL1_strcmp((char*)cmd, DCDR_CMD_STATUS)==0) || (UTIL1_strcmp((char*)cmd, "PWM* status")==0)) {
		*handled = TRUE;
		  CLS1_SendStatusStr((unsigned char*)DCDRIVE_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
		return ERR_OK;
  }
 // else if ((UTIL1_strcmp((char*)cmd, DCDRIVE_CMD_SPEED)==0) || (UTIL1_strcmp((char*)cmd, DCDRIVE_CMD_MSG" "DCDRIVE_CMD_SPEED)==0))
  else if (strncmp((const char*)cmd, (const char*)DCDRIVE_SHELL_NAME_STR " " DCDRIVE_CMD_SPEED, sizeof(DCDRIVE_SHELL_NAME_STR " "DCDRIVE_CMD_SPEED)-1)==0) {
      p = cmd+sizeof(DCDRIVE_SHELL_NAME_STR" "DCDRIVE_CMD_SPEED);
      if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=DC_MAXSPEED) {
    	  //setDutyCycle(val);
        //PWM3_SetDutyMS((uint8_t)val);
    	  setDCSpeed(val);
		*handled = TRUE;
      } else {
    	  *handled = TRUE;
        CLS1_SendStr((const unsigned char*)"Wrong pos argument, must be in the range 0.."DC_MAXSPEED_STR"\r\n",io->stdErr);
        res = ERR_FAILED;
      }
  return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)DCDRIVE_SHELL_NAME_STR " pos", sizeof(DCDRIVE_SHELL_NAME_STR " pos")-1)==0) {
      p = cmd+sizeof(DCDRIVE_SHELL_NAME_STR " pos");
      if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=100) {
    	  setDutyCycle(val);
        //PWM3_SetDutyMS((uint8_t)val);
		*handled = TRUE;
      } else {
        CLS1_SendStr((const unsigned char*)"Wrong pos argument, must be in the range 0..100\r\n", io->stdErr);
        res = ERR_FAILED;
      }
  return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)DCDRIVE_SHELL_NAME_STR" vdir", sizeof(DCDRIVE_SHELL_NAME_STR" vdir")-1)==0) {
	  *handled = TRUE;
	  Bit_DC_Ruck_ClrVal();
	  Bit_DC_Vor_SetVal();
  return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)DCDRIVE_SHELL_NAME_STR" rdir", sizeof(DCDRIVE_SHELL_NAME_STR" rdir")-1)==0) {
	  *handled = TRUE;
	  Bit_DC_Vor_ClrVal();
	  Bit_DC_Ruck_SetVal();
  return ERR_OK;
  }
  return ERR_OK;
}


//------------PID Part---------------
void pidDoWork(void)
{
	// readSpeed from Encoder(in value nomValue)
	nomValueOld = nomValue;			//ist Wert (old)
	nomValue=updateNomValue();


  nomValue = (nomValueOld+nomValue) >> 1;



  if (setValue == 0) val = integ = devOld = 0;
  else
  {
    // deviation (max devL = +1000 - -1000 = 2000)
    //dev = (setValue - nomValue) / 8;
	 dev = (setValue - nomValue);
    // P-Part (max kpL =
    val = (kp * dev) / 32;

    // I-Part with anti-windup
    if (ki != 0) integ += dev;
    val += (ki * integ) / 32;
    //val+=(ki*integ)/32;

    // D-Part
    val += (kd*(dev-devOld)/32);
    devOld = dev;

    // limit control point
    if (val > 100)
    {
      val = 100;
      integ -= dev;
    }
    else if (val < 1)
    {
      val = 0;
      integ -= dev;
    }
  }
  setDutyCycle(100-val);
}
