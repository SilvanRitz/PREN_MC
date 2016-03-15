/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "DCDrive.h"
#include "Bit_DC_Vor.h"
#include "Bit_DC_Ruck.h"



#define PWM3_CMD_HELP   			"help"
#define PWM3_CMD_STATUS 			"status"
#define PWM3_SHELL_NAME_STR      	"PWM3" /*!< Name used for servo in shell */
#define TIMER_PERIOD_DUR			500		//in us
#define PWM3_PERIOD_VALUE_PROZENT 	TIMER_PERIOD_DUR/100

enum dcDriveStates_t{
	INIT,
	HANDLE_SPEED,
	EXIT
}dcDriveStates;


//AD1_Measure();

void DCDhandleSpeed(void){
	switch (dcDriveStates){
	case INIT:
		debugPrintfDCDrive("\nDCDrive intialized\r\n");
		dcDriveStates=HANDLE_SPEED;
		Bit_DC_Vor_SetVal();
		break;
	case HANDLE_SPEED:

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

	PWM3_SetDutyUS((uint8_t)(val*PWM3_PERIOD_VALUE_PROZENT));
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
		CLS1_SendHelpStr((unsigned char*)"  pos", (unsigned char*)"Values 0..100 ändert den dutycycle (0 = off)\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  vdir", (unsigned char*)"Drehe in Vorwärtsrichtung\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  rdir", (unsigned char*)"Drehe in Rückwärtsrichtung\r\n", io->stdOut);
		return ERR_OK;
  } else if ((UTIL1_strcmp((char*)cmd, PWM3_CMD_STATUS)==0) || (UTIL1_strcmp((char*)cmd, "PWM* status")==0)) {
		*handled = TRUE;
		  CLS1_SendStatusStr((unsigned char*)"PWM3", (unsigned char*)"\r\n", io->stdOut);
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

