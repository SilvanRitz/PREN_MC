/*
 * UltraSonic.h
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

#ifndef SOURCES_ULTRASONIC_H_
#define SOURCES_ULTRASONIC_H_

void startMeasurement(void);
void US_Init(void);
uint16_t US_Measure_us(void);
uint16_t US_usToCentimeters(uint16_t microseconds, uint8_t temperatureCelsius);
void debugPrintfUltraSonic(const char *fmt, ...);
void US_EventEchoCapture(LDD_TUserData *UserDataPtr);
void US_EventEchoOverflow(LDD_TUserData *UserDataPtr);
void US_Send_Impuls();
#endif /* SOURCES_ULTRASONIC_H_ */
