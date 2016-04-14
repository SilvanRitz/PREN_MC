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
	#define CFG_ULTRASONIC_MSG		(1)
	#define CFG_DEBUG_MSG			(1)
	#define CFG_ADC_MSG				(0)
	#define CFG_FLEXSENSOR_DBG			(0)				//SPAM
	#define CFG_FLEXSENSOR_CMD			(0)
	#define CFG_DCDRIVE_MSG_DGB			(0)
	#define CFG_DCDRIVE_MSG_CMD			(1)
	#define CFG_SHELL_DEBUG_OUT		(1)
	#define CFG_PWR_MSG 			(0)
	#define CFG_SERVOPARSE_MSG 		(1)
	#define CFG_ENCODER_MSG			(0)
	#define CFG_ABELADEN_MSG 		(1)
	#define CFG_AENTLADEN_MSG		(1)
	#define	CFG_RASP_CHECK			(0)
#endif




#endif /* SOURCES_CONFIG_H_ */
