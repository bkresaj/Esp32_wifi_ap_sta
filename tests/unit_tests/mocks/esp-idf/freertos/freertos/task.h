#pragma once

#include "FreeRTOSConfig.h"
#include "port_macro.h"
#include "projdefs.h"

//fake
#define configMINIMAL_STACK_SIZE 100
#define configMAX_PRIORITIES 10

struct tskTaskControlBlock; /* The old naming convention is used to prevent
                               breaking kernel aware debuggers. */
typedef struct tskTaskControlBlock *TaskHandle_t;

void vTaskDelay(const TickType_t xTicksToDelay);
BaseType_t xTaskCreate(TaskFunction_t pvTaskCode, const char *const pcName,
                       configSTACK_DEPTH_TYPE usStackDepth, void *pvParameters,
                       UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask);
void vTaskSuspend(TaskHandle_t xTaskToSuspend);
void vTaskResume(TaskHandle_t xTaskToResume);
