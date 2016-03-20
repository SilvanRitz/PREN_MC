/*
 * Encoder.h
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_ENCODER_H_
#define SOURCES_ENCODER_H_

#define ENCODER_MSG_CMD		"Encode"
//void startPWMonitoring(void);
//void PWRMan_Init(void);
void debugPrintfEncoder(const char *fmt, ...);
void enableEncoder(void);

volatile unsigned int counterA;


#endif /* SOURCES_ULTRASONIC_H_ */
