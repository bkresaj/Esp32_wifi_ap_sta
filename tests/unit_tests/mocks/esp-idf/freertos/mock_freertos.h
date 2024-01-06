#pragma once

#include <gmock/gmock.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/port_macro.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "singleton_template.h"

class MockFreertos : public SingletonTemplate<MockFreertos>
{
   public:
    MOCK_METHOD(EventGroupHandle_t, xEventGroupCreate, (), ());
    MOCK_METHOD(EventBits_t, xEventGroupWaitBits,
                (const EventGroupHandle_t xEventGroup,
                 const EventBits_t uxBitsToWaitFor,
                 const BaseType_t xClearOnExit,
                 const BaseType_t xWaitForAllBits, TickType_t xTicksToWait),
                ());
    MOCK_METHOD(void, vEventGroupDelete, (EventGroupHandle_t xEventGroup), ());
    MOCK_METHOD(void, vTaskDelay, (const TickType_t xTicksToDelay), ());
    MOCK_METHOD(BaseType_t, xTaskCreate,
                (TaskFunction_t pvTaskCode, const char *const pcName,
                 configSTACK_DEPTH_TYPE usStackDepth, void *pvParameters,
                 UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask),
                ());
    MOCK_METHOD(EventBits_t, xEventGroupSetBits,
                (EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet),
                ());
    MOCK_METHOD(SemaphoreHandle_t, xSemaphoreCreateMutex, (), ());
    MOCK_METHOD(BaseType_t, xSemaphoreTake,
                (SemaphoreHandle_t xSemaphore, TickType_t xBlockTime), ());
    MOCK_METHOD(BaseType_t, xSemaphoreGive, (SemaphoreHandle_t xSemaphore), ());
    MOCK_METHOD(void, vSemaphoreDelete, (SemaphoreHandle_t xSemaphore), ());
    MOCK_METHOD(void, vTaskSuspend, (TaskHandle_t xTaskToSuspend), ());
    MOCK_METHOD(void, vTaskResume, (TaskHandle_t xTaskToResume), ());
};