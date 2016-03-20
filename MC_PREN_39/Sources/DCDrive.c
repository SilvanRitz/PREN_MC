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



#define PWM3_CMD_HELP   			"help"
#define PWM3_CMD_STATUS 			"status"
#define PWM3_SHELL_NAME_STR      	"PWM3" /*!< Name used for servo in shell */
#define TIMER_PERIOD_DUR			500		//in us
#define PWM3_PERIOD_VALUE_PROZENT 	TIMER_PERIOD_DUR/100

#define DCDRIVE_CMD_SPEED			"spd"
#define DCDRIVE_SHELL_NAME_STR		"PWM3"//"DCDr"
#define DC_MAXSPEED					190
#define DC_MAXSPEED_STR				"190"

#define TICKS_MAXSPEED				100
#define DC_SPD_PER_TICK				TICKS_

static uint16 actSpeedValue=0;

static uint16 nomValue=100;		//OFF
static uint16 nomValueOld=100;		//istwert
static uint16 setValue,setValueOld;		//sollwert
static uint16 val,integ, devOld,dev;
static uint8 delay=0;
static uint8 kp=60;
static uint8 ki=10;
static uint8 kd=20;

/*  kiL = kiR = 10; //10 max 20
  kpL = kpR = 60; //60 max 128
  kdL = kdR = 0; //10  max 40*/

enum dcDriveStates_t{
	INIT,
	HANDLE_SPEED,
	EXIT
}dcDriveStates;


#define DCDRIVE_MSG_CMD		"DCDr"

//AD1_Measure();

void DCDhandleSpeed(void){
	switch (dcDriveStates){
	case INIT:
		debugPrintfDCDrive("%s %s: intialized\r\n",DEBUG_MSG_CMD,DCDRIVE_MSG_CMD);
		dcDriveStates=HANDLE_SPEED;
		Bit_DC_Vor_SetVal();
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
	debugPrintfDCDrive("%s Der Wert ist: %d\r\n", DEBUG_MSG_CMD,val);
	PWM3_SetDutyUS((uint16_t)(val*PWM3_PERIOD_VALUE_PROZENT));
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
  if (UTIL1_strcmp((char*)cmd, PWM3_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "PWM3 help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)"PWM3", (unsigned char*)"Group of PWM3 commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  pos", (unsigned char*)"Values 0..100 �ndert den dutycycle (0 = off)\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  vdir", (unsigned char*)"Drehe in Vorw�rtsrichtung\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  rdir", (unsigned char*)"Drehe in R�ckw�rtsrichtung\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  spd", (unsigned char*)"Gibt die Geschwindigkeit in mm pro s an\nWerte zwischen 0 und "DC_MAXSPEED_STR"\r\n", io->stdOut);
		return ERR_OK;
  } else if ((UTIL1_strcmp((char*)cmd, PWM3_CMD_STATUS)==0) || (UTIL1_strcmp((char*)cmd, "PWM* status")==0)) {
		*handled = TRUE;
		  CLS1_SendStatusStr((unsigned char*)"PWM3", (unsigned char*)"\r\n", io->stdOut);
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
        CLS1_SendStr((const unsigned char*)"Wrong pos argument, must be in the range 0.."DC_MAXSPEED_STR"\r\n",io->stdErr);
        res = ERR_FAILED;
      }
  return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)PWM3_SHELL_NAME_STR " pos", sizeof(PWM3_SHELL_NAME_STR " pos")-1)==0) {
      p = cmd+sizeof(PWM3_SHELL_NAME_STR " pos");
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
  else if (strncmp((const char*)cmd, (const char*)PWM3_SHELL_NAME_STR " vdir", sizeof(PWM3_SHELL_NAME_STR " pos")-1)==0) {
	  Bit_DC_Ruck_ClrVal();
	  Bit_DC_Vor_SetVal();
  return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)PWM3_SHELL_NAME_STR " rdir", sizeof(PWM3_SHELL_NAME_STR " pos")-1)==0) {
	  Bit_DC_Vor_ClrVal();
	  Bit_DC_Ruck_SetVal();
  return ERR_OK;
  }
}


//------------PID Part---------------
void pidDoWork(void)
{
	// readSpeed from Encoder(in value nomValue)
	nomValueOld = nomValue;
	nomValue=updateNomValue();
  if (delay > 0)
  {
    delay--;
    return;
  }

  if (setValueOld == 0 && setValue != 0)
  {
    delay = 2;
  }
  setValueOld = setValue;


  nomValue = (nomValueOld+nomValue) >> 1;


  // left wheel
  if (setValue == 0) val = integ = devOld = 0;
  else
  {
    // deviation (max devL = +1000 - -1000 = 2000)
    dev = (setValue - nomValue) / 8;

    // P-Part (max kpL =
    val = (kp * dev) / 32;

    // I-Part with anti-windup
    if (ki != 0) integ += dev;
    val += (ki * integ) / 32;

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
