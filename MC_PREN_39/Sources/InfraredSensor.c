/*
 * InfraredSensor.c
 *
 *  Created on: 20.11.2015
 *      Author: Wallpaper
 */

enum infraRedStates_t{
	INIT,
	GET_DISTANCE,
	EXIT
}infraRedStates;


//AD1_Measure();

void getDistance(void){
	switch (infraRedStates){
	case INIT:
		debugPrintf("\nInfrared\r\n");
		infraRedStates=GET_DISTANCE;
		break;
	case GET_DISTANCE:
		//getCommands();
		break;
	case EXIT:
		debugPrintf("Exit\r\n");
		break;
	}
}


void debugPrintfInfraRedSensor(const char *fmt, ...) {
#if CFG_INFRAREDSENSOR_MSG
	debugPrintf(fmt);
#endif
}
