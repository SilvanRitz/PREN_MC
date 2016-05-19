/*
 * config.h
 *
 *  Created on: 09.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_CONFIG_H_
#define SOURCES_CONFIG_H_

#include "PE_Types.h"

enum runStates_{
	DEBUG,
	RUN
};

volatile bool AD_finished;
volatile uint16 IR_counter;
volatile uint16 IR_changed;
volatile uint16 IR_LastCounter;
volatile bool IR_flanke;
#define WAS_RISING	0
#define OBJEKT_WEG	1


/*
 * Module
 */
#define CFG_SHELL			(1)
#define CFG_ULTRASONIC		(1)
#define CFG_ADC				(1)
#define CFG_DCDRIVE			(1)
#define CFG_HANDLE_ACTIONS	(1)
#define CFG_CHECK_RASP		(1)



/*
 * Messages
 */
#if CFG_SHELL
	#define CFG_ULTRASONIC_MSG		(0)
	#define CFG_DEBUG_MSG			(1)
	#define CFG_ADC_MSG				(0)
	#define CFG_FLEXSENSOR_DBG			(0)				//SPAM
	#define CFG_FLEXSENSOR_CMD			(0)
	#define CFG_DCDRIVE_MSG_DGB			(0)
	#define CFG_DCDRIVE_MSG_CMD			(0)				//SPAM
	#define CFG_SHELL_DEBUG_OUT		(1)
	#define CFG_PWR_MSG 			(0)
	#define CFG_SERVOPARSE_MSG 		(1)
	#define CFG_ENCODER_MSG			(0)			//SPAM => Für Debug DC Motor
	#define CFG_ABELADEN_MSG 		(1)
	#define CFG_AENTLADEN_MSG		(1)
	#define	CFG_RASP_CHECK			(0)
	#define CFG_LOW_AKKU_MELDEN		(1)
	#define CFG_AKKU					(1)				//COMMAND
#endif


/*
 * Aktionen
 */
#define FLEX_LENK_ENABLE			(0)			//Schaltet die Flexlenkung aus
#define AKKU_ABSCHALTEN				(1)			//Schaltet die Flexlenkung aus


//TEST's	Im normalfall 0 !
#define TEST_AUFLADEN				(0)
#define TEST_CONTAINER_ANHALTEN		(1)





#endif /* SOURCES_CONFIG_H_ */
