/*
 * UART_Shell.h
 *
 *  Created on: 03.11.2015
 *      Author: Silvan Ritz
 */


#ifndef SOURCES_UART_SHELL_H_
#define SOURCES_UART_SHELL_H_

#include "config.h"

#define DEBUG_MSG_CMD "DBG"

unsigned int MyXprintf(const char *fmt, ...);

/*
/**
 * \brief Gets all Commands from the Shell.
 */
void getCommands(void);
/*
/**
 * \brief Mainfunction of this module. Call frequently in a task.
 */
void handleCommunication(void);
void debugPrintfShell(const char *fmt, ...);

unsigned int debugPrintf(const char *fmt, ...);

#endif /* SOURCES_UART_SHELL_H_ */
