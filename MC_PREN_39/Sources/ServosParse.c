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

enum CamPos{
	GERADE,
	R_VOR,
	R_KUR,
	L_KUR
}camPosEnum;

//-----------Shell------------
#define SERVO_CMD_HELP   			"help"
#define SERVO_CMD_STATUS 			"status"

//-----------Lenkservo--------
#define LENKSERVO_SHELL_NAME_STR 	"LeS"
#define LENKSERVO_GRAD_CMD			"deg"
	#define LENKSERVO_MAXGRAD		190
	#define LENKSERVO_MAXGRAD_STR	"190"
#define LENKSERVO_DEFAULT_CMD		"def"

//-----------Kameraservo--------
#define KAMERASERVO_SHELL_NAME_STR 	"CamP"
#define KAMERASERVO_GRAD_CMD		"p"
	#define KAMERASERVO_MAXGRAD		190
	#define KAMERASERVO_MAXGRAD_STR	"190"
#define KAMERASERVO_RECHTS_CMD		"r"
#define KAMERASERVO_GERADE_CMD		"g"



enum dcDriveStates_t{
	INIT,
	HANDLE_SPEED,
	EXIT
}dcDriveStates;



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
  if (UTIL1_strcmp((char*)cmd, SERVO_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, LENKSERVO_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)LENKSERVO_SHELL_NAME_STR, (unsigned char*)"Group of Lenkservo commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "LENKSERVO_GRAD_CMD, (unsigned char*)"Die Gradänderung relativ zur momentanen Position\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "LENKSERVO_DEFAULT_CMD, (unsigned char*)"Lenkung auf geradeaus fahren\r\n", io->stdOut);
		return ERR_OK;
} else if ((UTIL1_strcmp((char*)cmd, LENKSERVO_GRAD_CMD)==0) || (UTIL1_strcmp((char*)cmd, LENKSERVO_SHELL_NAME_STR" status")==0)) {
	*handled = TRUE;
	  CLS1_SendStatusStr((unsigned char*)LENKSERVO_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
	return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)LENKSERVO_SHELL_NAME_STR " " LENKSERVO_GRAD_CMD, sizeof(LENKSERVO_SHELL_NAME_STR " "LENKSERVO_GRAD_CMD)-1)==0) {
p = cmd+sizeof(LENKSERVO_SHELL_NAME_STR" "LENKSERVO_GRAD_CMD);
if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=LENKSERVO_MAXGRAD) {
	  //setDCSpeed(val);
	*handled = TRUE;
} else {
	  *handled = TRUE;
  CLS1_SendStr((const unsigned char*)"Wrong "LENKSERVO_GRAD_CMD" argument, must be in the range 0.."LENKSERVO_MAXGRAD_STR"\r\n",io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)LENKSERVO_SHELL_NAME_STR " "LENKSERVO_DEFAULT_CMD, sizeof(LENKSERVO_SHELL_NAME_STR " "LENKSERVO_DEFAULT_CMD)-1)==0) {
	*handled = TRUE;
return ERR_OK;
}
return ERR_OK;
}

uint8_t SERVO_Kamera_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, SERVO_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, KAMERASERVO_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)KAMERASERVO_SHELL_NAME_STR, (unsigned char*)"Group of Kameraservoservo commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "KAMERASERVO_GRAD_CMD, (unsigned char*)"Die absolute Position des Servos\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "KAMERASERVO_RECHTS_CMD, (unsigned char*)"Kameraservo nach rechts ausrichten\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "KAMERASERVO_GERADE_CMD, (unsigned char*)"Kameraservo nach vorne ausrichten\r\n", io->stdOut);
		return ERR_OK;
} else if ((UTIL1_strcmp((char*)cmd, KAMERASERVO_GRAD_CMD)==0) || (UTIL1_strcmp((char*)cmd, KAMERASERVO_SHELL_NAME_STR" status")==0)) {
	*handled = TRUE;
	  CLS1_SendStatusStr((unsigned char*)KAMERASERVO_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
	return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)KAMERASERVO_SHELL_NAME_STR " " KAMERASERVO_GRAD_CMD, sizeof(KAMERASERVO_SHELL_NAME_STR " "KAMERASERVO_GRAD_CMD)-1)==0) {
p = cmd+sizeof(KAMERASERVO_SHELL_NAME_STR" "KAMERASERVO_GRAD_CMD);
if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=KAMERASERVO_MAXGRAD) {
	  //setDCSpeed(val);
	*handled = TRUE;
} else {
	  *handled = TRUE;
  CLS1_SendStr((const unsigned char*)"Wrong "KAMERASERVO_GRAD_CMD" argument, must be in the range 0.."KAMERASERVO_MAXGRAD_STR"\r\n",io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)KAMERASERVO_SHELL_NAME_STR " "KAMERASERVO_RECHTS_CMD, sizeof(KAMERASERVO_SHELL_NAME_STR " "KAMERASERVO_RECHTS_CMD)-1)==0) {
	*handled = TRUE;
return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)KAMERASERVO_SHELL_NAME_STR " "KAMERASERVO_GERADE_CMD, sizeof(KAMERASERVO_SHELL_NAME_STR " "KAMERASERVO_GERADE_CMD)-1)==0) {
	*handled = TRUE;
return ERR_OK;
}
return ERR_OK;
}

/*
 * uint8_t SERVO_Lenkung_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
	  int32_t val;
	  const unsigned char *p;
	  uint8_t res=ERR_OK;
  if (UTIL1_strcmp((char*)cmd, SERVO_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, LENKSERVO_SHELL_NAME_STR" help")==0) {
		*handled = TRUE;
		CLS1_SendHelpStr((unsigned char*)LENKSERVO_SHELL_NAME_STR, (unsigned char*)"Group of Lenkservo commands\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "LENKSERVO_GRAD_CMD, (unsigned char*)"Die Gradänderung relativ zur momentanen Position\r\n", io->stdOut);
		CLS1_SendHelpStr((unsigned char*)" "LENKSERVO_DEFAULT_CMD, (unsigned char*)"Lenkung auf geradeaus fahren\r\n", io->stdOut);
		return ERR_OK;
} else if ((UTIL1_strcmp((char*)cmd, LENKSERVO_GRAD_CMD)==0) || (UTIL1_strcmp((char*)cmd, LENKSERVO_SHELL_NAME_STR" status")==0)) {
	*handled = TRUE;
	  CLS1_SendStatusStr((unsigned char*)LENKSERVO_SHELL_NAME_STR, (unsigned char*)"\r\n", io->stdOut);
	return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)LENKSERVO_SHELL_NAME_STR " " LENKSERVO_GRAD_CMD, sizeof(LENKSERVO_SHELL_NAME_STR " "LENKSERVO_GRAD_CMD)-1)==0) {
p = cmd+sizeof(LENKSERVO_SHELL_NAME_STR" "LENKSERVO_GRAD_CMD);
if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=LENKSERVO_MAXGRAD) {
	  //setDCSpeed(val);
	*handled = TRUE;
} else {
	  *handled = TRUE;
  CLS1_SendStr((const unsigned char*)"Wrong "LENKSERVO_GRAD_CMD" argument, must be in the range 0.."LENKSERVO_MAXGRAD_STR"\r\n",io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)LENKSERVO_SHELL_NAME_STR " "LENKSERVO_GRAD_CMD, sizeof(LENKSERVO_SHELL_NAME_STR " "LENKSERVO_GRAD_CMD)-1)==0) {
p = cmd+sizeof(LENKSERVO_SHELL_NAME_STR " "LENKSERVO_GRAD_CMD);
if (UTIL1_xatoi(&p, &val)==ERR_OK && val>=0 && val<=100) {
//
	*handled = TRUE;
} else {
  CLS1_SendStr((const unsigned char*)"Wrong "LENKSERVO_GRAD_CMD" argument, must be in the range 0..100\r\n", io->stdErr);
  res = ERR_FAILED;
}
return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)LENKSERVO_SHELL_NAME_STR " "LENKSERVO_DEFAULT_CMD, sizeof(LENKSERVO_SHELL_NAME_STR " "LENKSERVO_DEFAULT_CMD)-1)==0) {
	*handled = TRUE;
//
return ERR_OK;
}
else if (strncmp((const char*)cmd, (const char*)LENKSERVO_SHELL_NAME_STR " rdir", sizeof(LENKSERVO_SHELL_NAME_STR " pos")-1)==0) {
	*handled = TRUE;
//
return ERR_OK;
}
return ERR_OK;
}*/



