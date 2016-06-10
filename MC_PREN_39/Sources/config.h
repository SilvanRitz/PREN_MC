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

volatile uint16 beladen_Counter;
volatile uint8 beladen_Active;
volatile bool secondBeladenWait;
volatile uint16 zweiteDistanz;
#define NICHT_BELADEN			0
#define ANFAHREN_BELADEN		1
#define ANFAHREN_BELADEN_CNT	2
#define AUFLADEN				3

/*
 * Module
 */
#define CFG_SHELL			(1)
#define CFG_ULTRASONIC		(1)
#define CFG_ADC				(1)
#define CFG_DCDRIVE			(1)
#define CFG_HANDLE_ACTIONS	(1)
#define CFG_CHECK_RASP		(0)				//1 enabled den check ob rasp da ist (Kann zu fehler führen)


/*
 * Messages
 */
#if CFG_SHELL
#define CFG_DEBUG_MSG			(1)			//immer 1 lassen!

	//DEBUG
	#define CFG_FLEXSENSOR_DBG		(0)			//SPAM
	#define CFG_DCDRIVE_MSG_DGB		(0)			//fast ungebraucht
	#define CFG_SHELL_DEBUG_OUT		(1)			//ungebraucht
	#define CFG_SERVOPARSE_MSG 		(1)			//ungebraucht
	#define CFG_ENCODER_MSG			(0)			//SPAM => Für Debug DC Motor


	//COMMANDS and INFOS for Rasp
	#define CFG_ULTRASONIC_MSG		(1)
	#define CFG_FLEXSENSOR_CMD		(1)
	#define CFG_IR_CMD				(0)
	#define CFG_AKKU				(0)			//Meldet Low akku (schaltet nicht ab)
	#define	CFG_RASP_CHECK			(1)			//auf 1 lassen, rasp check über task ausschalten
	#define CFG_AENTLADEN_MSG		(1)
	#define CFG_ABELADEN_MSG 		(1)
	#define CFG_DCDRIVE_MSG_CMD		(0)			//SPAM
#endif


/*
 * Aktionen
 */
#define FLEX_LENK_ENABLE			(0)			//Schaltet die Flexlenkung aus
#define AKKU_ABSCHALTEN				(0)			//Akku überwachung
#define MULTIPLE_STOP_ENABLE		(0)			//normally 1


//TEST's	Im normalfall 0 !
#define TEST_AUFLADEN				(0)			//initiirt nur den Aufladeprozess (kein Anfahren)
#define TEST_CONTAINER_ANHALTEN		(0)
#define TEST_LADEN_NO_ACTION		(0)			//ist stärker als TEST_CONTAINER_ANHALTEN
												//Wenn 1 fährt am Container vorbei

#endif /* SOURCES_CONFIG_H_ */
