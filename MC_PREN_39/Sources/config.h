/*
 * config.h
 *
 *  Created on: 09.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_CONFIG_H_
#define SOURCES_CONFIG_H_

enum runStates_{
	DEBUG,
	RUN
};

volatile bool IR_AD_finished;
/*
 * Module
 */
#define CFG_SHELL			(1)
#define CFG_ULTRASONIC		(1)
#define CFG_INFRARED		(1)
#define CFG_DCDRIVE			(1)



/*
 * Messages
 */
#if CFG_SHELL
	#define CFG_ULTRASONIC_MSG		(0)
	#define CFG_DEBUG_MSG			(1)
	#define CFG_INFRAREDSENSOR_MSG	(0)
	#define CFG_DCDRIVE_MSG			(1)
#endif
#endif /* SOURCES_CONFIG_H_ */
