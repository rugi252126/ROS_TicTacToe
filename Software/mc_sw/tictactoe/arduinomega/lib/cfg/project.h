#ifndef PROJECT_H_
#define PROJECT_H_

#include <Arduino.h>

//! Project related declarations

// For SW development only. Comment-out when releasing offical SW.
// #define SW_DEVELOPMENT_DEBUG_PORT_USED

// Task based on T3_BCU scheduler
const int BASE_TASK_MS = 10;

// Led indicator base task in milli-second form
const int LED_INDICATOR_BASE_TASK_MS = 20;
// Led indicator application task in milli-second form
const int LED_INDICATOR_TASK_MS = 100;

// SwitchIn application task in milli-second form
const int SWITCH_IN_TASK_MS = 50;

// Led board application task in milli-second form
const int LED_BOARD_TASK_MS = 50;

// Buzzer application task  in milli-second form
const int BUZZER_TASK_MS = 50;

#endif // PROJECT_H_
