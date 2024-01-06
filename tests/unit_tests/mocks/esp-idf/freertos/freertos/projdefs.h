#pragma once

#define pdFALSE 0
#define pdTRUE 1

#define pdPASS pdTRUE
#define pdFAIL pdFALSE

typedef void (*TaskFunction_t)(void *);

// real
//#define pdMS_TO_TICKS(xTimeInMs)                                               \
//    ((TickType_t)(((TickType_t)(xTimeInMs) * (TickType_t)configTICK_RATE_HZ) / \
//                  (TickType_t)1000U))
// fake
#define pdMS_TO_TICKS(xTimeInMs)  0
