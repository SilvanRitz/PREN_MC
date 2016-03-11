/*
 * UART_Shell.c
 *
 *  Created on: 03.11.2015
 *      Author: Silvan Ritz
 */
/*
/*!
** @file UART_Shell.c
** @version 01.01
** @brief
**         Shell module.
**         This module contains the interface to the UART Shell.
*/
/*
/*!
**  @addtogroup UART_Shell_module  UART_Shell module documentation
**  @{
*/
#include <stdio.h>
#include "CLS1.h"
#include "LED1.h"
#include "SERVO1.h"
#include "XF1.h"
#include "config.h"
#include "FRTOS1.h"
#include "DCDrive.h"
#include "UART_Shell.h"


enum shellStates_t{
	INIT,
	GET_COMMANDS,
	EXIT
}shellStates;

  unsigned char cmd[100];

  /* table with shell parser/handler */
  static const CLS1_ParseCommandCallback CmdParserTable[] =
  {
    CLS1_ParseCommand,
    LED1_ParseCommand,
	SERVO1_ParseCommand,
	//SERVO2_ParseCommand,
	FRTOS1_ParseCommand,
	PWM3_ParseCommand,
    NULL /* sentinel */
  };

void getCommands(void){
	CLS1_ReadAndParseWithCommandTable(cmd,sizeof(cmd),CLS1_GetStdio(),CmdParserTable);
}


static void MyPutChar(void *arg, char c) {
  CLS1_StdIO_OutErr_FctType fct = arg;

  fct(c);
}
/*
/**
 * \brief Prints the Parameter to the shell.
 * \param *fmt The String to print.
 */
unsigned int MyXprintf(const char *fmt, ...) {
  va_list args;
  unsigned int count;

  va_start(args,fmt);
  count = XF1_xvformat(MyPutChar, CLS1_GetStdio()->stdOut, fmt, args);
  va_end(args);
  return count;
}


unsigned int debugPrintf(const char *fmt, ...) {
#if CFG_DEBUG_MSG
		return MyXprintf(fmt);
#endif
}



void handleCommunication(void){
	switch (shellStates){
	case INIT:
		debugPrintf("\nWelcome\r\n");
		shellStates=GET_COMMANDS;
		break;
	case GET_COMMANDS:
		getCommands();
		break;
	case EXIT:
		debugPrintf("Exit\r\n");
		break;
	}
}

void debugPrintfShell(const char *fmt, ...) {
#if CFG_SHELL_DEBUG_OUT
	debugPrintf(fmt);
#endif
}
/*
/*!
** @}
*/
