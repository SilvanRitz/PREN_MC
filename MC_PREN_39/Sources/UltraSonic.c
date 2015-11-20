/*
 * UltraSonic.c
 *
 *  Created on: 06.11.2015
 *      Author: Wallpaper
 */

//#include "BitIoLdd1.h"
#include "WAIT_UltaSonic.h"
#include "TU2.h"
//#include "PwmLdd2.h"
#include "TRIG.h"
#include "UltraSonic.h"
#include "UART_Shell.h"
#include "config.h"

static enum US_GenaralState_ {
  US_INIT, /* device not used */
  US_MEASURE, /* started trigger pulse */
  US_CALC_DIST, /* measuring echo pulse */
} US_GenaralState;

typedef enum {
  ECHO_IDLE, /* device not used */
  ECHO_TRIGGERED, /* started trigger pulse */
  ECHO_MEASURE, /* measuring echo pulse */
  ECHO_OVERFLOW, /* measurement took too long */
  ECHO_FINISHED /* measurement finished */
} US_EchoState;

typedef struct {
  LDD_TDeviceData *trigDevice; /* device handle for the Trigger pin */
  LDD_TDeviceData *echoDevice; /* input capture device handle (echo pin) */
  volatile US_EchoState state; /* state */
  TU2_TValueType capture; /* input capture value */
} US_DeviceType;

static US_DeviceType usDevice;

void startMeasurement(void){
	uint16_t echotime_us=0;
	uint16_t distance_cm=0;
	for(;;){
		switch (US_GenaralState){
		case US_INIT:
			debugPrintfUltraSonic("US: Init done.\r\n");
			US_GenaralState=US_MEASURE;
			US_Init();
		break;
		case US_MEASURE:
			echotime_us=US_Measure_us();
			if (!echotime_us){
				debugPrintfUltraSonic("Keine Messung! Zu grosse Distanz?\r\n");
				echotime_us=0;		//nur für den Breakpoint
			}
			US_GenaralState=US_CALC_DIST;
			break;
		case US_CALC_DIST:
			distance_cm=US_usToCentimeters(echotime_us,25);
			debugPrintfUltraSonic("Distanz: %i\r\n",distance_cm);
			US_GenaralState=US_MEASURE;
			break;
		}
		WAIT_UltaSonic_Waitms(100);
	}
}

void US_Init(void) {
  usDevice.state = ECHO_IDLE;
  usDevice.capture = 0;
  usDevice.trigDevice = TRIG_Init(NULL);
  usDevice.echoDevice = TU2_Init(&usDevice);
}

void US_EventEchoOverflow(LDD_TUserData *UserDataPtr) {
  US_DeviceType *ptr = (US_DeviceType*)UserDataPtr;

  ptr->state = ECHO_OVERFLOW;
}


void US_EventEchoCapture(LDD_TUserData *UserDataPtr){
	US_DeviceType *ptr = (US_DeviceType*)UserDataPtr;
	if (ptr->state==ECHO_TRIGGERED) { /* 1st edge, this is the raising edge, start measurement */
	  TU2_ResetCounter(ptr->echoDevice);
	  ptr->state = ECHO_MEASURE;
	} else if (ptr->state==ECHO_MEASURE) { /* 2nd edge, this is the falling edge: use measurement */
	  (void)TU2_GetCaptureValue(ptr->echoDevice, 0, &ptr->capture);
	  ptr->state = ECHO_FINISHED;
	}
}

uint16_t US_Measure_us(void){
  uint16_t us;

  /* send 10us pulse on TRIG line. */
  TRIG_SetVal(usDevice.trigDevice);
  WAIT_UltaSonic_Waitus(10);
  usDevice.state = ECHO_TRIGGERED;
  TRIG_ClrVal(usDevice.trigDevice);
  while(usDevice.state!=ECHO_FINISHED) {
    /* measure echo pulse */
    if (usDevice.state==ECHO_OVERFLOW) { /* measurement took too long? */
      usDevice.state = ECHO_IDLE;
      return 0; /* no echo, error case */
    }
  }
  us = (usDevice.capture*1000UL)/(TU2_CNT_INP_FREQ_U_0/1000);
  return us;
}


static uint16_t calcAirspeed_dms(uint8_t temperatureCelsius) {
  /* Return the airspeed depending on the temperature, in deci-meter per second */
  unsigned int airspeed; /* decimeters per second */

  airspeed = 3313 + (6 * temperatureCelsius); /* dry air, 0% humidity, see http://en.wikipedia.org/wiki/Speed_of_sound */
  airspeed -= (airspeed/100)*15; /* factor in ~15% for a relative humidity of ~40% */
  return airspeed;
}

uint16_t US_usToCentimeters(uint16_t microseconds, uint8_t temperatureCelsius) {
  return (microseconds*100UL)/calcAirspeed_dms(temperatureCelsius)/2; /* 2 because of two way */
}

void debugPrintfUltraSonic(const char *fmt, ...) {
#if CFG_ULTRASONIC_MSG
	debugPrintf(fmt);
#endif
}
