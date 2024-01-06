#pragma once

#include "FreeRTOS.h"
#include "projdefs.h"
#include "event_groups.h"

typedef void* SemaphoreHandle_t;

SemaphoreHandle_t xSemaphoreCreateMutex();
BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xBlockTime);
BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);
void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
