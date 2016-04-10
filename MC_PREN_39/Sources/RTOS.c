/*
 * RTOS.c
 *
 *  Created on: 03.11.2015
 *      Author: Silvan Ritz
 */
#include "FRTOS1.h"
#include "UART_Shell.h"
#include "UltraSonic.h"
#include "ADC_Handler.h"
#include "DCDrive.h"
#include "AutonomBeladen.h"

static portTASK_FUNCTION(Task_Shell, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	handleCommunication();
    FRTOS1_vTaskDelay(25/(portTICK_RATE_MS*16));
  }
}

static portTASK_FUNCTION(Task_Us, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	startMeasurement();
    FRTOS1_vTaskDelay(1000/(portTICK_RATE_MS*16));
  }
}

static portTASK_FUNCTION(Task_Drive, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	DCDhandleSpeed();
    FRTOS1_vTaskDelay(50/(portTICK_RATE_MS*16));
  }
}

static portTASK_FUNCTION(Task_IR, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	getDistance();
    FRTOS1_vTaskDelay(100/(portTICK_RATE_MS*16));
  }
}

static portTASK_FUNCTION(Task_Handl_Act, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	handleActions();
    FRTOS1_vTaskDelay(100/(portTICK_RATE_MS*16));
  }
}



bool CreateTasks(void){
#if CFG_SHELL
	if (FRTOS1_xTaskCreate(
        Task_Shell,  /* pointer to the task */
        "Shell", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
		tskIDLE_PRIORITY+1,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_ULTRASONIC
	if (FRTOS1_xTaskCreate(
        Task_Us,  /* pointer to the task */
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
        Task_IR,  /* pointer to the task */
        "InfraRed", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_DCDRIVE
	if (FRTOS1_xTaskCreate(
        Task_Drive,  /* pointer to the task */
        "DCDrive", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
		//1,
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_HANDLE_ACTIONS
	if (FRTOS1_xTaskCreate(
        Task_Handl_Act,  /* pointer to the task */
        "Handl_Act", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY+2,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

	FRTOS1_vTaskStartScheduler();
	return TRUE;
}

