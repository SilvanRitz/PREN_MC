/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "handleActions.h"
#include "UART_Shell.h"
#include "config.h"
#include "AutonomBeladen.h"
#include "AutonomEntladen.h"
#include "DCDrive.h"
#include "Bit_5V_2_Enable.h"

#include "CAM_SERVO1.h"
#include "LENK_SERVO2.h"
#include "GREIF_SERVO3.h"
#include "LADEN_SERVO4.h"
#include "ENTLADEN_SERVO5.h"
#include "LED1.h"

#include "ServoParse.h"
#include "UART_Shell.h"
#include "AutonomBeladen.h"
#include "config.h"



static handle_actions_t hadleActionsState=INIT_ALL;
//static handle_actions_t hadleActionsState=INIT_ALL;

//-----------Shell Autonom Beladen--------
#define HANDLE_ACTION_MSG_CMD			"HA"

#define A_ENTLADEN_CMD_HELP			"help"

#define A_ENTLADEN_SHELL_NAME_STR 	"StE"
#define A_ENTLADEN_POS_CMD			"d"
	#define A_ENTLADEN_MAXDIST		1000
	#define A_ENTLADEN_MAXDIST_STR	"1000"
#define A_ENTLADEN_FIN_RESP			"StEf"

#define START_CMD_HELP				"help"

#define START_SHELL_NAME_STR		"StartFrd"
#define START_CMD					"a"

#define START_FIN_RESP				"Ready"


#define STOP_SHELL_NAME_STR			"Stop"
#define STOP_CMD					"a"

#define A_BELADEN_FIN_RESP			"StAf"		//auch im autonom beladen

//---------SERVO Konstanten---------

//#define CAM_SERVO_INIT		125
#define LENK_SERVO_INIT		125
#define GREIF_SERVO_INIT	200				//45 =>Greifklemme offen
#define LADE_SERVO_INIT		200				//Hebel 0°
#define	ENTLADE_SERVO_INIT	245				//Entladeklappe geschlossen



//--------Variabeln---------
static uint8 beladenCount=0;





void handleActions(void){
	switch (hadleActionsState){
	case INIT_ALL:
		//Init DC DCDrive
		setDCSpeed(0);

		//Init Servos
		CAM_SERVO1_SetPos(CAM_SERVO_GERADE);
		LENK_SERVO2_SetPos(LENK_SERVO_INIT);
		GREIF_SERVO3_SetPos(GREIF_SERVO_INIT);
		LADEN_SERVO4_SetPos(LADE_SERVO_INIT);
		ENTLADEN_SERVO5_SetPos(ENTLADE_SERVO_INIT);

		beladen_Counter=0;
		beladen_Active=NICHT_BELADEN;
		secondBeladenWait=FALSE;
		break;

	case INIT_DONE:
		setDCVorwaerts();
		Bit_5V_2_Enable_SetVal();		//Enable second Akku
		debugPrintfHandleActions("%s\r\n",START_FIN_RESP);
		changeToDrive();
		break;
	case DRIVE:
		break;
	case BELADEN:
		autoBeladen();
		break;
	case ENTLADEN:
		autoEntladen();
		break;
	case FERTIG:
		setDCSpeed(0);
		CAM_SERVO1_SetPos(CAM_SERVO_GERADE);
		LENK_SERVO2_SetPos(126);
		Bit_5V_2_Enable_ClrVal();
		changeToInitAll();
		break;
	case AKKU_LEER:
		setDCSpeed(0);
		CAM_SERVO1_SetPos(CAM_SERVO_GERADE);
		LENK_SERVO2_SetPos(126);
		Bit_5V_2_Enable_ClrVal();
		debugPrintfHandleActions("%s: Akku leer!!!\r\n",DEBUG_MSG_CMD);
		debugPrintfHandleActions("%s\r\n",STOP_SHELL_NAME_STR);
		LED_Blink();
		break;
	}
}

void changeToBeladen(void){
	debugPrintfHandleActions("%s %s State Beladen aktiv\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	#if TEST_LADEN_NO_ACTION==0
	if(hadleActionsState!=DRIVE){
		debugPrintfHandleActions("%s %s Ungültiger state wechsel zu beladen!! Vorher %u\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD,hadleActionsState);
	}
	hadleActionsState=BELADEN;
	beladenCount++;
#endif
#if TEST_LADEN_NO_ACTION
	debugPrintfABeladen("%s\r\n",A_BELADEN_FIN_RESP);		//Response for Rasp
#endif
}

void changeToInitAll(void){
	debugPrintfHandleActions("%s %s State Init All\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	hadleActionsState=INIT_ALL;
}

void changeToEntladen(void){
	debugPrintfHandleActions("%s %s State Entladen aktiv\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	if(hadleActionsState!=DRIVE){
		debugPrintfHandleActions("%s %s Ungültiger state wechsel zu entladen!! Vorher %u\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD,hadleActionsState);
	}
	if(beladenCount!=2){
		debugPrintfHandleActions("%s %s Beladen nicht zwei mal ausgeführt. Anz: %u\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD,beladenCount);
	}
	hadleActionsState=ENTLADEN;
}

void changeToDrive(void){
	debugPrintfHandleActions("%s %s State Drive aktiv\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	hadleActionsState=DRIVE;
#if MULTIPLE_STOP_ENABLE
	if(secondBeladenWait==TRUE){
		int16 result= zweiteDistanz-beladen_Counter*BELADEN_SPD/1000;
		if (result<0){
			result=0;
		}
		setDistance(result);
		changeToBeladen();
	}
#endif
}


void changeToFertig(void){
	debugPrintfHandleActions("%s %s State Fertig aktiv\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	debugPrintfHandleActions("%s\r\n",STOP_SHELL_NAME_STR);
	beladen_Active=NICHT_BELADEN;
	beladenCount=0;
	hadleActionsState=FERTIG;
}

void changeToInitDone(void){
	debugPrintfHandleActions("%s %s State Init Done aktiv\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	hadleActionsState=INIT_DONE;
}

void changeToAkkuLeer(void){
	debugPrintfHandleActions("%s %s State Akku Leer aktiv\r\n",DEBUG_MSG_CMD,HANDLE_ACTION_MSG_CMD);
	hadleActionsState=AKKU_LEER;
}



void debugPrintfHandleActions(const char *fmt, ...) {
	debugPrintf(fmt);
}

handle_actions_t getHandleActionsState(){
	return hadleActionsState;
}

//----------Shellpart------------
uint8_t start_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, START_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, A_ENTLADEN_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)START_SHELL_NAME_STR, (unsigned char*)"Group of Statusübergabe commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "START_CMD, (unsigned char*)"Startet das Programm\r\n", io->stdOut);
		return ERR_OK;
} else if (strncmp((const char*)cmd, (const char*)START_SHELL_NAME_STR " " START_CMD, sizeof(START_SHELL_NAME_STR " "START_CMD)-1)==0) {
	*handled = TRUE;
	changeToInitDone();
	return ERR_OK;
}
return ERR_OK;
}


//----------Shellpart------------
uint8_t stopp_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, START_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, A_ENTLADEN_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)STOP_SHELL_NAME_STR, (unsigned char*)"Stoppt das Programm\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "STOP_CMD, (unsigned char*)"Stoppt das Programm\r\n", io->stdOut);
		return ERR_OK;
} else if (strncmp((const char*)cmd, (const char*)STOP_SHELL_NAME_STR " " STOP_CMD, sizeof(STOP_SHELL_NAME_STR " "STOP_CMD)-1)==0) {
	*handled = TRUE;
	changeToFertig();
	return ERR_OK;
}
return ERR_OK;
}


void LED_Blink(void){
	static uint8 ledCounter=0;
	ledCounter++;
	if (ledCounter<20){
		LED1_ClrVal();
	}
	else if(ledCounter<40){
		LED1_SetVal();
	}
	else{
		ledCounter=0;
	}
}
