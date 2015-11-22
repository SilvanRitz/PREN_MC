/*
 * DCDrive.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */
#include "DCDrive.h"

enum dcDriveStates_t{
	INIT,
	HANDLE_SPEED,
	EXIT
}dcDriveStates;


//AD1_Measure();

void DCDhandleSpeed(void){
	switch (dcDriveStates){
	case INIT:
		debugPrintfDCDrive("\nDCDrive intialized\r\n");
		dcDriveStates=HANDLE_SPEED;
		break;
	case HANDLE_SPEED:
		//getCommands();
		break;
	case EXIT:
		debugPrintfDCDrive("Exit\r\n");
		break;
	}
}


void debugPrintfDCDrive(const char *fmt, ...) {
#if CFG_DCDRIVE_MSG
	debugPrintf(fmt);
#endif
}
