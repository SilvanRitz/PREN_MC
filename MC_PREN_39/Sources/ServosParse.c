/*
 * DCDrive.c

 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

#include "ServoParse.h"
#include "UART_Shell.h"
#include "SERVO1.h"
#include "SERVO2.h"
#include "SERVO3.h"
#include "SERVO4.h"
#include "SERVO5.h"
#include "config.h"

#define LENKSERVO_MSG_CMD		"LeSer"
#define LENKSERVO_GRAD_CMD		"deg"
#define LENKSERVO_DEFAULT_CMD	"def"

#define SERVO_Lenkung_CMD_HELP   			"help"
#define SERVO_Lenkung_CMD_STATUS 			"status"
#define PWM3_SHELL_NAME_STR      	"PWM3" /*!< Name used for servo in shell */
/*#define TIMER_PERIOD_DUR			500		//in us
#define PWM3_PERIOD_VALUE_PROZENT 	TIMER_PERIOD_DUR/100

enum dcDriveStates_t{
	INIT,
	HANDLE_SPEED,
	EXIT
}dcDriveStates;
*/

#define SERVO1_MSG_CMD		"DCDr"
#define SERVO2_MSG_CMD		"DCDr"
#define SERVO3_MSG_CMD		"DCDr"
#define SERVO4_MSG_CMD		"DCDr"
#define SERVO5_MSG_CMD		"DCDr"
#define SERVO_GNRL			"SERVO"


//AD1_Measure();



void debugPrintfServoParse(const char *fmt, ...) {
#if CFG_SERVOPARSE_MSG
	debugPrintf(fmt);
#endif
}

/*void setDutyCycle(unsigned int val){	//get called by Shell

	PWM3_SetDutyUS((uint16_t)(val*PWM3_PERIOD_VALUE_PROZENT));
}*/


//--------Shellpart------------

uint8_t SERVO_Lenkung_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, SERVO_Lenkung_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "PWM3 help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)LENKSERVO_MSG_CMD, (unsigned char*)"Group of Lenkservo commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "LENKSERVO_GRAD_CMD, (unsigned char*)"Die Gradänderung relativ zur momentanen Position\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "LENKSERVO_DEFAULT_CMD, (unsigned char*)"Lenkung auf geradeaus fahren\r\n", io->stdOut);
		return ERR_OK;
  } else if ((UTIL1_strcmp((char*)cmd, SERVO_Lenkung_CMD_STATUS)==0) || (UTIL1_strcmp((char*)cmd, "PWM* status")==0)) {
		*handled = TRUE;
		  CLS1_SendStatusStr((unsigned char*)LENKSERVO_MSG_CMD, (unsigned char*)"\r\n", io->stdOut);
		return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)LENKSERVO_MSG_CMD " "LENKSERVO_GRAD_CMD, sizeof(LENKSERVO_MSG_CMD" "LENKSERVO_GRAD_CMD)-1)==0) {
      p = cmd+sizeof(LENKSERVO_MSG_CMD" "LENKSERVO_GRAD_CMD);
      if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=100) {
    	 debugPrintfServoParse("%s %s: Funktion aufgerufen\r\n",DEBUG_MSG_CMD, LENKSERVO_DEFAULT_CMD);
		*handled = TRUE;
      } else {
        CLS1_SendStr((const unsigned char*)"Wrong pos argument, must be in the range 0..100\r\n", io->stdErr);
        res = ERR_FAILED;
      }
  return ERR_OK;
  }
 /* else if (strncmp((const char*)cmd, (const char*)PWM3_SHELL_NAME_STR " vdir", sizeof(PWM3_SHELL_NAME_STR " pos")-1)==0) {
	  Bit_DC_Ruck_ClrVal();
	  Bit_DC_Vor_SetVal();
  return ERR_OK;
  }
  else if (strncmp((const char*)cmd, (const char*)PWM3_SHELL_NAME_STR " rdir", sizeof(PWM3_SHELL_NAME_STR " pos")-1)==0) {
	  Bit_DC_Vor_ClrVal();
	  Bit_DC_Ruck_SetVal();
  return ERR_OK;
  }*/
}

