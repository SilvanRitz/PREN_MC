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
#define CFG_INFRARED		(1)
#define CFG_DCDRIVE			(1)
#define CFG_AUTO_BELADEN	(0)



/*
 * Messages
 */
#if CFG_SHELL
	#define CFG_ULTRASONIC_MSG		(0)
	#define CFG_DEBUG_MSG			(1)
	#define CFG_INFRAREDSENSOR_MSG	(0)
	#define CFG_FLEXSENSOR_MSG		(0)
	#define CFG_DCDRIVE_MSG			(1)
	#define CFG_SHELL_DEBUG_OUT		(1)
	#define CFG_PWR_MSG 			(1)
	#define CFG_SERVOPARSE_MSG 		(1)
	#define CFG_ENCODER_MSG			(1)
	#define CFG_ABELADEN_MSG 		(1)
#endif




#endif /* SOURCES_CONFIG_H_ */
