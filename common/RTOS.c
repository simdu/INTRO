/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#if PL_CONFIG_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"
#include "LED.h"
#include "Event.h"
#include "Keys.h"
#include "Application.h"

static void AppTask(void* param) {
  (void)param; /* avoid compiler warning */

  for(;;) {
	#if PL_CONFIG_HAS_KEYS
		KEY_Scan();
	#endif
	#if PL_CONFIG_HAS_EVENTS
		EVNT_HandleEvent(APP_EventHandler, TRUE);
	#endif

    /* \todo handle your application code here */
    FRTOS1_vTaskDelay(pdMS_TO_TICKS(500));
  }
}

static void LedTask(void* param) {
  const int *whichLED = (int*)param;

  (void)param; /* avoid compiler warning */
  for(;;) {
    if (*whichLED==1) {
      LED1_Neg();
    } else if (*whichLED==2) {
      LED2_Neg();
    }
    /* \todo handle your application code here */
    //FRTOS1_vTaskDelay(pdMS_TO_TICKS(500));
  }
}



void RTOS_Init(void) {
  static const int led1 = 1;
  static const int led2 = 2;

  #if PL_CONFIG_HAS_EVENTS
  	  EVNT_SetEvent(EVNT_STARTUP);
  #endif
	#if PL_CONFIG_HAS_SHELL && CLS1_DEFAULT_SERIAL
	  CLS1_SendStr((uint8_t*)"Hello World!\r\n", CLS1_GetStdio()->stdOut);
	#endif
  /*! \todo Create tasks here */
  if (FRTOS1_xTaskCreate(AppTask, (signed portCHAR *)"AppTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }

  if (FRTOS1_xTaskCreate(LedTask, (signed portCHAR *)"LED1Task", configMINIMAL_STACK_SIZE, (void*)&led1, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }

  if (FRTOS1_xTaskCreate(LedTask, (signed portCHAR *)"LED2Task", configMINIMAL_STACK_SIZE, (void*)&led2, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }

  vTaskStartScheduler(); /* start the RTOS, create the IDLE task and run my tasks (if any) */
  /* does usually not return! */
}


void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
