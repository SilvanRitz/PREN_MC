/*
 * UART_Shell.c
 *
 *  Created on: 03.11.2015
 *      Author: Silvan Ritz
 */
#include "CLS1.h"
#include "LED1.h"
#include "XF1.h"

  unsigned char cmd[100];

  /* table with shell parser/handler */
  static const CLS1_ParseCommandCallback CmdParserTable[] =
  {
    CLS1_ParseCommand,
    LED1_ParseCommand,
    NULL /* sentinel */
  };

void getCommands(void){
	CLS1_ReadAndParseWithCommandTable(cmd,sizeof(cmd),CLS1_GetStdio(),CmdParserTable);
}

void debugPrint(){
	  char buf[64];
	  XF1_xsprintf(buf, "Hello world\r\n");
	  CLS1_SendStr((unsigned char*)buf, CLS1_GetStdio()->stdOut);
}

