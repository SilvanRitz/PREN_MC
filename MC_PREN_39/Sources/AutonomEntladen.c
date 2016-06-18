/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "AutonomEntladen.h"
#include "UART_Shell.h"
#include "config.h"
#include "handleActions.h"

#include "ENTLADEN_SERVO5.h"
#include "FRTOS1.h"
#include "DCDrive.h"


//-----------Shell Autonom Beladen--------
#define A_ENTLADEN_CMD_HELP			"help"

#define A_ENTLADEN_SHELL_NAME_STR 	"StE"
#define A_ENTLADEN_POS_CMD			"d"
	#define A_ENTLADEN_MAXDIST		1000
	#define A_ENTLADEN_MAXDIST_STR	"1000"
#define A_ENTLADEN_FIN_RESP			"StEf"

#define ENTLADEN_SERVO_OFFEN		30
#define ENTLADEN_SERVO_ZU			245

#define SERVO_DELAY_ALG_E				500
#define SERVO_DELAY_LONG_E			2000

//--------Variabeln---------

//#define NICHT_ENTLADEN	0
//#define ENTLADEN			1
//static volatile bool autoEntladenFlg=NICHT_ENTLADEN;


enum aBeladenStates_t{
	DO_NOTHING,
	HALT,
	OPEN,
	CLOSE,
	EXIT
}aEntladenStates=DO_NOTHING;


void autoEntladen(void){
	switch (aEntladenStates){
	case DO_NOTHING:
		aEntladenStates=HALT;
		debugPrintfAEntladen("%s\r\n",A_ENTLADEN_FIN_RESP);
		break;
	case HALT:
		setDCSpeed(0);
		aEntladenStates=OPEN;
		FRTOS1_vTaskDelay(SERVO_DELAY_ALG_E/(portTICK_RATE_MS));
		break;
	case OPEN:
		ENTLADEN_SERVO5_SetPos(ENTLADEN_SERVO_OFFEN);
		aEntladenStates=CLOSE;
		FRTOS1_vTaskDelay(SERVO_DELAY_LONG_E/(portTICK_RATE_MS));
		break;
	case CLOSE:
		ENTLADEN_SERVO5_SetPos(ENTLADEN_SERVO_ZU);
		aEntladenStates=EXIT;
		FRTOS1_vTaskDelay(SERVO_DELAY_ALG_E/(portTICK_RATE_MS));
		break;
	case EXIT:
		changeToFertig();
		break;
	}
}


void debugPrintfAEntladen(const char *fmt, ...) {
#if CFG_AENTLADEN_MSG
	debugPrintf(fmt);
#endif
}

//---------Shellpart----------


uint8_t A_Entladen_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, A_ENTLADEN_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, A_ENTLADEN_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)A_ENTLADEN_SHELL_NAME_STR, (unsigned char*)"Group of Statusübergabe commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "A_ENTLADEN_POS_CMD, (unsigned char*)"Leitet den Entladeprozess ein, mit der ungefähren Position zum Container in mm\r\n", io->stdOut);
		return ERR_OK;
} else if ((UTIL1_strcmp((char*)cmd, A_ENTLADEN_POS_CMD)==0) || (UTIL1_strcmp((char*)cmd, A_ENTLADEN_SHELL_NAME_STR" status")==0)) {
	*handled = TRUE;
	  CLS1_SendStatusStr((unsigned char*)A_ENTLADEN_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
	return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)A_ENTLADEN_SHELL_NAME_STR " " A_ENTLADEN_POS_CMD, sizeof(A_ENTLADEN_SHELL_NAME_STR " "A_ENTLADEN_POS_CMD)-1)==0) {
p = cmd+sizeof(A_ENTLADEN_SHELL_NAME_STR" "A_ENTLADEN_POS_CMD);
if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=A_ENTLADEN_MAXDIST) {
	*handled = TRUE;
	changeToEntladen();
} else {
	  *handled = TRUE;
  CLS1_SendStr((const unsigned char*)"Wrong "A_ENTLADEN_POS_CMD" argument, must be in the range 0.."A_ENTLADEN_MAXDIST_STR"\r\n",io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
return ERR_OK;
}
