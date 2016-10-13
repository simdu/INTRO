/**
 * \file
 * \brief Timer driver
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements the driver for all our timers.
  */

#include "Platform.h"
#if PL_CONFIG_HAS_TIMER
#include "Timer.h"
#if PL_CONFIG_HAS_EVENTS
  #include "Event.h"
#endif
#if PL_CONFIG_HAS_TRIGGER
  #include "Trigger.h"
#endif
#if PL_CONFIG_HAS_MOTOR_TACHO
  #include "Tacho.h"
#endif


void TMR_OnInterrupt(void) {
	static int count_OnInterrupt = 0;

	count_OnInterrupt++;
#if PL_CONFIG_HAS_EVENTS
	if(count_OnInterrupt >= (1000/TMR_TICK_MS)){
		EVNT_SetEvent(EVNT_LED_HEARTBEAT);
		count_OnInterrupt = 0;
	}
#endif
}

void TMR_Init(void) {
}

void TMR_Deinit(void) {
}

#endif /* PL_CONFIG_HAS_TIMER*/
