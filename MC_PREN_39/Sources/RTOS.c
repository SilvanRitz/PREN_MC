/*
 * RTOS.c
 *
 *  Created on: 03.11.2015
 *      Author: Silvan Ritz
 */
#include "FRTOS1.h"
#include "UART_Shell.h"

static portTASK_FUNCTION(Task1, pvParameters) {
  (void)pvParameters; /* parameter not used */
  for(;;) {
	 handleCommunication();
    FRTOS1_vTaskDelay(100/portTICK_RATE_MS);
  }
}


bool CreateTasks(void){
	if (FRTOS1_xTaskCreate(
        Task1,  /* pointer to the task */
        "Task1", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS){
		return FALSE;
	  }
	return TRUE;
}

