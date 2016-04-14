/*
 * UltraSonic.c
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

//#include "BitIoLdd1.h"

#include "config.h"
#include "UART_Shell.h"
#include "RaspCheck.h"



static enum RaspCheck_GenaralState_ {
  INIT, /* device not used */
  CHECK
} RaspCheck_GenaralState;
#define RASP_CHECK_DBG_ID	"RaspCheck"
//-----------Shell RaspCheck--------
#define RASP_CHECK_CMD_HELP			"help"

#define RASP_CHECK_SHELL_NAME_STR 	"Ja"
#define RASP_CHECK_RESPONSE			"a"


//------------Frd Response
#define RASP_CHECK_MSG_CMD	"There"

static bool strReceived=false;

void checkRasp(void){
		switch (RaspCheck_GenaralState){
			case INIT:
				debugPrintfRaspCheck("%s %s: Init done.\r\n",DEBUG_MSG_CMD,RASP_CHECK_DBG_ID);
				RaspCheck_GenaralState=CHECK;
				debugPrintfRaspCheck("%s",RASP_CHECK_MSG_CMD);
			break;
			case CHECK:

				if(strReceived){
					//no error
					strReceived=false;
				}
				else{
					//error
					debugPrintfRaspCheck("%s %s: NO RASP Response in time (ignored).\r\n",DEBUG_MSG_CMD,RASP_CHECK_DBG_ID);
				}
				debugPrintfRaspCheck("%s",RASP_CHECK_MSG_CMD);
				break;
		}
}


void debugPrintfRaspCheck(const char *fmt, ...) {
#if CFG_RASP_CHECK
	debugPrintf(fmt);
#endif
}


uint8_t Rasp_Check_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io){
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, RASP_CHECK_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, RASP_CHECK_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)RASP_CHECK_SHELL_NAME_STR, (unsigned char*)"Group of Statusübergabe commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "RASP_CHECK_RESPONSE, (unsigned char*)"benötigte Response", io->stdOut);
		return ERR_OK;
} else if ((UTIL1_strcmp((char*)cmd, RASP_CHECK_SHELL_NAME_STR)==0) || (UTIL1_strcmp((char*)cmd, RASP_CHECK_SHELL_NAME_STR" status")==0)) {
	*handled = TRUE;
	  CLS1_SendStatusStr((unsigned char*)RASP_CHECK_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
	return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)RASP_CHECK_SHELL_NAME_STR" "RASP_CHECK_RESPONSE, sizeof(RASP_CHECK_SHELL_NAME_STR" "RASP_CHECK_RESPONSE)-1)==0) {
	  *handled = TRUE;
	  strReceived=true;
return ERR_OK;
}
return ERR_OK;
}
