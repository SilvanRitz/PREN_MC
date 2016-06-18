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
#include "RaspCheck.h"
#include "handleActions.h"

static portTASK_FUNCTION(Task_Shell, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	handleCommunication();
    FRTOS1_vTaskDelay(5/(portTICK_RATE_MS));
  }
}

static portTASK_FUNCTION(Task_Us, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	startMeasurement();
    FRTOS1_vTaskDelay(500/(portTICK_RATE_MS));
  }
}

static portTASK_FUNCTION(Task_Drive, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	DCDhandleSpeed();
    FRTOS1_vTaskDelay(50/(portTICK_RATE_MS));
  }
}

static portTASK_FUNCTION(Task_ADC, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	 handleADC();
    FRTOS1_vTaskDelay(100/(portTICK_RATE_MS));
  }
}

static portTASK_FUNCTION(Task_Handl_Act, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	handleActions();
    FRTOS1_vTaskDelay(100/(portTICK_RATE_MS));
  }
}

static portTASK_FUNCTION(Task_Check_Rasp, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	checkRasp();
    FRTOS1_vTaskDelay(1000/(portTICK_RATE_MS));
  }
}




bool CreateTasks(void){
#if CFG_SHELL
	if (FRTOS1_xTaskCreate(
        Task_Shell,  /* pointer to the task */
        "Shell", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE+100, /* task stack size */
        (void*)NULL, /* optional task startup argument */
		tskIDLE_PRIORITY+6,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_ULTRASONIC
	if (FRTOS1_xTaskCreate(
        Task_Us,  /* pointer to the task */
        "UltraSonic", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE+50, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY+2,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_ADC
	if (FRTOS1_xTaskCreate(
        Task_ADC,  /* pointer to the task */
        "HandleADC", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE+50, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY+1,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_DCDRIVE
	if (FRTOS1_xTaskCreate(
        Task_Drive,  /* pointer to the task */
        "DCDrive", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE+50, /* task stack size */
        (void*)NULL, /* optional task startup argument */
		//1,
        tskIDLE_PRIORITY+3,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_HANDLE_ACTIONS
	if (FRTOS1_xTaskCreate(
        Task_Handl_Act,  /* pointer to the task */
        "Handl_Act", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE+100, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY+4,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
#endif

#if CFG_CHECK_RASP
	if (FRTOS1_xTaskCreate(
		Task_Check_Rasp,  /* pointer to the task */
        "Check_Rasp", /* task name for kernel awareness debugging */
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

