/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "AutonomBeladen.h"
#include "UART_Shell.h"
#include "config.h"


//-----------Shell Autonom Beladen--------
#define A_BELADEN_CMD_HELP			"help"

#define A_BELADEN_SHELL_NAME_STR 	"StA"
#define A_BELADEN_POS_CMD			"d"
	#define A_BELADEN_MAXDIST		1000
	#define A_BELADEN_MAXDIST_STR	"1000"
#define A_BELADEN_FIN_RESP			"StAf"

//--------Variabeln---------
#define NICHT_BELADEN	0
#define BELADEN			1
static volatile bool autoBeladenFlg=NICHT_BELADEN;

enum aBeladenStates_t{
	DO_NOTHING,
	INIT,
	CHECK_IR,
	CONTAINER_FOUND,
	GET_CONTAINER,
	EMPTY_CONTAINER,
	PLACE_CONTAINER,
	SEND_REPORT,
	EXIT
}aBeladenStates=DO_NOTHING;


void autoBeladen(void){
	switch (aBeladenStates){
	case DO_NOTHING:
		if (autoBeladenFlg){
			autoBeladenFlg=NICHT_BELADEN;
			aBeladenStates=INIT;
			debugPrintfABeladen("%s\r\n",A_BELADEN_FIN_RESP);
		}
		break;
	case INIT:


		//debugPrintfABeladen("%s %s: freigegeben\r\n",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR);
		//setSpeed()
		//handle Direction
		//measure time
		//time elapsed =>Check Ir
		aBeladenStates=CHECK_IR;

		//---TEMPORÄR-----
		aBeladenStates=DO_NOTHING;
		changeToDrive();
		//-------
		break;
	case CHECK_IR:
		//get IR Interrupt
		//measure "On Time"
		//in tolerance? => contaier found and stop
		break;
	case CONTAINER_FOUND:
		//init container greifen
		break;
	case GET_CONTAINER:
		//init container heben
		break;
	case EMPTY_CONTAINER:
		//init container zurückstellen (herunterfahren)
		break;
	case PLACE_CONTAINER:
		// Container loslassen
		break;
	case SEND_REPORT:
		//command an Raspberry
		break;
	}
}


void debugPrintfABeladen(const char *fmt, ...) {
#if CFG_ABELADEN_MSG
	debugPrintf(fmt);
#endif
}

//---------Shellpart----------

uint8_t A_Beladen_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, A_BELADEN_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, A_BELADEN_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)A_BELADEN_SHELL_NAME_STR, (unsigned char*)"Group of Statusübergabe commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "A_BELADEN_POS_CMD, (unsigned char*)"Leitet den Aufldeprozess ein, mit der ungefähren Position zum Container in mm\r\n", io->stdOut);
		return ERR_OK;
} else if ((UTIL1_strcmp((char*)cmd, A_BELADEN_POS_CMD)==0) || (UTIL1_strcmp((char*)cmd, A_BELADEN_SHELL_NAME_STR" status")==0)) {
	*handled = TRUE;
	  CLS1_SendStatusStr((unsigned char*)A_BELADEN_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
	return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)A_BELADEN_SHELL_NAME_STR " " A_BELADEN_POS_CMD, sizeof(A_BELADEN_SHELL_NAME_STR " "A_BELADEN_POS_CMD)-1)==0) {
p = cmd+sizeof(A_BELADEN_SHELL_NAME_STR" "A_BELADEN_POS_CMD);
if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=A_BELADEN_MAXDIST) {
	autoBeladenFlg=BELADEN;
	*handled = TRUE;
	changeToBeladen();
} else {
	  *handled = TRUE;
  CLS1_SendStr((const unsigned char*)"Wrong "A_BELADEN_POS_CMD" argument, must be in the range 0.."A_BELADEN_MAXDIST_STR"\r\n",io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
return ERR_OK;
}
