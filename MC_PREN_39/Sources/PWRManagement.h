/*
 * UltraSonic.h
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_ULTRASONIC_H_
#define SOURCES_ULTRASONIC_H_

void startPWMonitoring(void);
void PWRMan_Init(void);
void debugPrintfPWRMeasurement(const char *fmt, ...);
#endif /* SOURCES_ULTRASONIC_H_ */
