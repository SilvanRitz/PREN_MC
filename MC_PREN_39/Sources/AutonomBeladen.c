/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "AutonomBeladen.h"
#include "UART_Shell.h"
#include "config.h"
#include "handleActions.h"
#include "FRTOS1.h"
#include "DCDrive.h"

#include "GREIF_SERVO3.h"
#include "LADEN_SERVO4.h"
#include "LED_BLUE.h"


//-----------Shell Autonom Beladen--------
#define A_BELADEN_CMD_HELP			"help"

#define A_BELADEN_SHELL_NAME_STR 	"StA"
#define A_BELADEN_POS_CMD			"d"
	#define A_BELADEN_MAXDIST		1000
	#define A_BELADEN_MAXDIST_STR	"1000"
#define A_BELADEN_FIN_RESP			"StAf"

#define BELADEN_SPD					100
#define BELADEN_SICHERHEIT_DIST		30
#define CONTAINER_LENGTH			25		//in mm

//--------Variabeln---------
//#define NICHT_BELADEN	0
//#define BELADEN			1
//static volatile bool autoBeladenFlg=NICHT_BELADEN;

enum aBeladenStates_t{
	DO_NOTHING,
	INIT,
	ENABLE_IR,
	CHECK_IR,
	CONTAINER_FOUND,
	GET_CONTAINER,
	EMPTY_CONTAINER,
	PLACE_CONTAINER,
	SEND_REPORT,
	EXIT
}aBeladenStates=DO_NOTHING;


static uint16 distance=0;
uint16 IR_Counter=0;

void autoBeladen(void){
	static uint16 wait_time=0;
	switch (aBeladenStates){
	case DO_NOTHING:
		aBeladenStates=INIT;
		debugPrintfABeladen("%s\r\n",A_BELADEN_FIN_RESP);
	break;
	case INIT:
		//debugPrintfABeladen("%s %s: freigegeben\r\n",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR);
		//setSpeed()
		//handle Direction
		//measure time
		//time elapsed =>Check Ir
		setDCSpeed(BELADEN_SPD);	//fahre mit 10cm pro sekunde
		if(distance>BELADEN_SICHERHEIT_DIST){
			wait_time=((uint32)(distance*1000)/BELADEN_SPD);
			debugPrintfABeladen("%s %s: Warte ms: %i",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR,wait_time);
			FRTOS1_vTaskDelay(wait_time/(portTICK_RATE_MS));
		}
		else{
			debugPrintfABeladen("%s %s: Distanz zum Container zu klein!",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR);
		}
		aBeladenStates=ENABLE_IR;

		//---TEMPORÄR-----
		//aBeladenStates=DO_NOTHING;
		//changeToDrive();
		//-------
		break;
	case ENABLE_IR:
		IR_changed=0;
		LED_BLUE_On();
		aBeladenStates=CHECK_IR;
	case CHECK_IR:
		//get IR Interrupt

		//measure "On Time"
		//in tolerance? => container found and stop
		if(IR_changed){
			IR_changed=0;
			if(((uint16)(CONTAINER_LENGTH)<((uint32)(IR_LastCounter*20*BELADEN_SPD)/1000)) && IR_flanke==OBJEKT_WEG){
				debugPrintfABeladen("%s %s: Der Container wurde gefunden!",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR);
				IR_LastCounter=0;
				//cotainer found
				setDCSpeed(0);
			}
		}
		break;
	case CONTAINER_FOUND:
		//init container greifen
		GREIF_SERVO3_SetPos(100);
	    FRTOS1_vTaskDelay(1000/(portTICK_RATE_MS));
		break;
	case GET_CONTAINER:
		//init container heben
		LADEN_SERVO4_SetPos(100);
		 FRTOS1_vTaskDelay(1000/(portTICK_RATE_MS));
		break;
	case EMPTY_CONTAINER:
		//init container zurückstellen (herunterfahren)
		LADEN_SERVO4_SetPos(0);
		 FRTOS1_vTaskDelay(1000/(portTICK_RATE_MS));
		break;
	case PLACE_CONTAINER:
		// Container loslassen
		GREIF_SERVO3_SetPos(100);
		FRTOS1_vTaskDelay(1000/(portTICK_RATE_MS));
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
	*handled = TRUE;
	distance=val;
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
