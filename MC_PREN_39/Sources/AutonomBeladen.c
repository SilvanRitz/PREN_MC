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
#define A_BELADEN_FIN_RESP			"StAf"		//auch im handleActions



//------Aufladen distanzen-------
#define BELADEN_SICHERHEIT_DIST		30
#define CONTAINER_LENGTH			25		//in mm

//-----SERVO Positionen-------
#define GREIF_SERVO_ZU				190				//45 =>Greifklemme offen
#define GREIF_SERVO_ZU_GREIFEN		80				//45 =>Greifklemme offen
#define GREIF_SERVO_OFFEN			230
#define GREIF_SERVO_HALB_OFFEN		220

#define LADE_SERVO_OBEN				200				//Hebel 45°
#define LADE_SERVO_OBEN_MITTEL		130				//Hebel 45°
#define LADE_SERVO_UNTEN_MITTEL		100				//Hebel 0°
#define LADE_SERVO_UNTEN			60				//Hebel 0° //60
#define LADE_SERVO_UNTEN_ABDRUECKEN	LADE_SERVO_UNTEN-30


#define SERVO_DELAY_ALG				800
#define SERVO_DELAY_LONG			1500


#define DISTANCE_HALT				10



//--------Variabeln---------
//#define NICHT_BELADEN	0
//#define BELADEN			1
//static volatile bool autoBeladenFlg=NICHT_BELADEN;

enum aBeladenStates_t{
	INIT,
	ENABLE_IR,
	CHECK_IR,
	CONTAINER_FOUND,
	HALT,
	GREIF_ZU_1,
	HEBEL_RUNTER_2,
	GREIF_AUF_3,
	HEBEL_RUNTER_4,
	GET_CONTAINER_HALB,
	GET_CONTAINER,
	EMPTY_CONTAINER,
	HEBEL_RUNTER_7,
	LOCKER_CONTAINER,
	CONTAINER_ABDRUECKEN,
	PLACE_CONTAINER,
	HEBEL_HOCH_9,
	GREIF_ZU_10,
	HEBEL_HOCH_11,
	GREIF_OFFEN_12,
	SEND_REPORT,
	EXIT
}aBeladenStates=INIT;


static uint16 distance=0;
uint16 IR_Counter=0;

void autoBeladen(void){
	static uint16 wait_time=0;
	static uint16 wait_time_halt=0;
	switch (aBeladenStates){
		case INIT:
			debugPrintfABeladen("%s %s: freigegeben\r\n",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR);	//Debug Msg
			beladen_Active=ANFAHREN_BELADEN;
			#if TEST_AUFLADEN
			aBeladenStates=GREIF_ZU_1;
			break;
		#endif
			//------Setze Spd und warte gewisse Zeit------
			setDCSpeed(BELADEN_SPD);	//fahre mit 10cm pro sekunde
			if(distance>BELADEN_SICHERHEIT_DIST){
				wait_time=((uint32)(distance*1000)/BELADEN_SPD);
				debugPrintfABeladen("%s %s: Warte ms: %i",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR,wait_time);
				FRTOS1_vTaskDelay(wait_time/(portTICK_RATE_MS));
			}
			else{
				debugPrintfABeladen("%s %s: Distanz zum Container zu klein!",DEBUG_MSG_CMD,A_BELADEN_SHELL_NAME_STR);
				wait_time=((uint32)(BELADEN_SICHERHEIT_DIST*1000)/BELADEN_SPD);
				FRTOS1_vTaskDelay(wait_time/(portTICK_RATE_MS));
			}
			aBeladenStates=ENABLE_IR;
			break;
		case ENABLE_IR:
			IR_changed=0;
			//ev delete zeit
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
					//cotainer found!
					wait_time_halt=((uint32)(DISTANCE_HALT*1000)/BELADEN_SPD);
					FRTOS1_vTaskDelay(wait_time_halt/(portTICK_RATE_MS));
					aBeladenStates=HALT;
				}
			}
			break;
		case HALT:
			setDCSpeed(0);
			//if(beladen_Active==)
			beladen_Active=AUFLADEN;//ERROR
			//Container Found
			aBeladenStates=GREIF_ZU_1;
#if TEST_CONTAINER_ANHALTEN
			debugPrintfABeladen("%s\r\n",A_BELADEN_FIN_RESP);		//Response for Rasp
			aBeladenStates=INIT;
			changeToFertig();
			//changeToDrive();
#endif
			break;
		case GREIF_ZU_1:
			//init container greifen
			GREIF_SERVO3_SetPos(GREIF_SERVO_ZU);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=HEBEL_RUNTER_2;
			break;
		case HEBEL_RUNTER_2:
			LADEN_SERVO4_SetPos(LADE_SERVO_UNTEN_MITTEL);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=GREIF_AUF_3;
			break;
		case GREIF_AUF_3:
			GREIF_SERVO3_SetPos(GREIF_SERVO_OFFEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=HEBEL_RUNTER_4;
			break;
		case HEBEL_RUNTER_4:
			LADEN_SERVO4_SetPos(LADE_SERVO_UNTEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=GET_CONTAINER_HALB;
			break;
		case GET_CONTAINER_HALB:
			GREIF_SERVO3_SetPos(GREIF_SERVO_ZU);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=GET_CONTAINER;
		case GET_CONTAINER:
			GREIF_SERVO3_SetPos(GREIF_SERVO_ZU_GREIFEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=EMPTY_CONTAINER;
		case EMPTY_CONTAINER:
			LADEN_SERVO4_SetPos(LADE_SERVO_OBEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG*2/(portTICK_RATE_MS));
			aBeladenStates=HEBEL_RUNTER_7;
			break;
		case HEBEL_RUNTER_7:
			//init container zurückstellen (herunterfahren)
			LADEN_SERVO4_SetPos(LADE_SERVO_UNTEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_LONG/(portTICK_RATE_MS));
			aBeladenStates=LOCKER_CONTAINER;
			break;
		case LOCKER_CONTAINER:
			//init container zurückstellen (herunterfahren)
			GREIF_SERVO3_SetPos(GREIF_SERVO_HALB_OFFEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=CONTAINER_ABDRUECKEN;
			break;
		case CONTAINER_ABDRUECKEN:
			LADEN_SERVO4_SetPos(LADE_SERVO_UNTEN_ABDRUECKEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=PLACE_CONTAINER;
			break;
		case PLACE_CONTAINER:
			GREIF_SERVO3_SetPos(GREIF_SERVO_OFFEN);		// Container loslassen
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=HEBEL_HOCH_9;
			break;
		case HEBEL_HOCH_9:
			LADEN_SERVO4_SetPos(LADE_SERVO_UNTEN_MITTEL);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=GREIF_ZU_10;
			break;
		case GREIF_ZU_10:
			GREIF_SERVO3_SetPos(GREIF_SERVO_ZU);		// Container loslassen
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=HEBEL_HOCH_11;
			break;
		case HEBEL_HOCH_11:
			LADEN_SERVO4_SetPos(LADE_SERVO_OBEN);
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=GREIF_OFFEN_12;
			break;
		case GREIF_OFFEN_12:
			GREIF_SERVO3_SetPos(GREIF_SERVO_OFFEN);		// Container loslassen
			FRTOS1_vTaskDelay(SERVO_DELAY_ALG/(portTICK_RATE_MS));
			aBeladenStates=SEND_REPORT;
			break;
		case SEND_REPORT:
			debugPrintfABeladen("%s\r\n",A_BELADEN_FIN_RESP);		//Response for Rasp
			changeToDrive();
			aBeladenStates=INIT;
			FRTOS1_vTaskDelay(2000/(portTICK_RATE_MS));
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
	beladen_Aufgerufen(val);
} else {
	  *handled = TRUE;
  CLS1_SendStr((const unsigned char*)"Wrong "A_BELADEN_POS_CMD" argument, must be in the range 0.."A_BELADEN_MAXDIST_STR"\r\n",io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
return ERR_OK;
}

void beladen_Aufgerufen(uint16 dist){
	if(getHandleActionsState()==BELADEN){
		if(beladen_Active==ANFAHREN_BELADEN)
		beladen_Active=ANFAHREN_BELADEN_CNT;			//Startet die Zeitmessung
		secondBeladenWait=TRUE;
		beladen_Counter=0;
		zweiteDistanz=dist;
	}
	else{
		changeToBeladen();
		distance=dist;
	}
}

void setDistance(uint16 dist){
	distance=dist;
}
