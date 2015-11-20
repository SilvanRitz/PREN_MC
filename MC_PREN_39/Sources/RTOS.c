/*
 * RTOS.c
 *
 *  Created on: 03.11.2015
 *      Author: Silvan Ritz
 */
#include "FRTOS1.h"
#include "UART_Shell.h"
#include "UltraSonic.h"
#include "InfraredSensor.h"

static portTASK_FUNCTION(Task1, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	handleCommunication();
    FRTOS1_vTaskDelay(100/portTICK_RATE_MS);
  }
}

static portTASK_FUNCTION(Task2, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	startMeasurement();
    FRTOS1_vTaskDelay(100/portTICK_RATE_MS);
  }
}

static portTASK_FUNCTION(Task3, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	getDistance();
    FRTOS1_vTaskDelay(100/portTICK_RATE_MS);
  }
}



bool CreateTasks(void){
#if CFG_SHELL
	if (FRTOS1_xTaskCreate(
        Task1,  /* pointer to the task */
        "Shell", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_ULTRASONIC
	if (FRTOS1_xTaskCreate(
        Task2,  /* pointer to the task */
        "UltraSonic", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_INFRARED
	if (FRTOS1_xTaskCreate(
        Task3,  /* pointer to the task */
        "InfraRed", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

	FRTOS1_vTaskStartScheduler();
	return TRUE;
}

