#include "mock_freertos.h"

EventGroupHandle_t xEventGroupCreate()
{
    return MockFreertos::getInstance().xEventGroupCreate();
}

EventBits_t xEventGroupWaitBits(const EventGroupHandle_t xEventGroup,
                                const EventBits_t uxBitsToWaitFor,
                                const BaseType_t xClearOnExit,
                                const BaseType_t xWaitForAllBits,
                                TickType_t xTicksToWait)
{
    return MockFreertos::getInstance().xEventGroupWaitBits(
        xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits,
        xTicksToWait);
}

void vEventGroupDelete(EventGroupHandle_t xEventGroup)
{
    return MockFreertos::getInstance().vEventGroupDelete(xEventGroup);
}

void vTaskDelay(const TickType_t xTicksToDelay)
{
    return MockFreertos::getInstance().vTaskDelay(xTicksToDelay);
}

BaseType_t xTaskCreate(TaskFunction_t pvTaskCode, const char *const pcName,
                       configSTACK_DEPTH_TYPE usStackDepth, void *pvParameters,
                       UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask)
{
    return MockFreertos::getInstance().xTaskCreate(pvTaskCode, pcName,
                                                   usStackDepth, pvParameters,
                                                   uxPriority, pxCreatedTask);
}

EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup,
                               const EventBits_t uxBitsToSet)
{
    return MockFreertos::getInstance().xEventGroupSetBits(xEventGroup,
                                                          uxBitsToSet);
}

SemaphoreHandle_t xSemaphoreCreateMutex()
{
    return MockFreertos::getInstance().xSemaphoreCreateMutex();
}

BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xBlockTime)
{
    return MockFreertos::getInstance().xSemaphoreTake(xSemaphore, xBlockTime);
}

BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore)
{
    return MockFreertos::getInstance().xSemaphoreGive(xSemaphore);
}

void vSemaphoreDelete(SemaphoreHandle_t xSemaphore)
{
    return MockFreertos::getInstance().vSemaphoreDelete(xSemaphore);
}

void vTaskSuspend(TaskHandle_t xTaskToSuspend)
{
    return MockFreertos::getInstance().vTaskSuspend(xTaskToSuspend);
}

void vTaskResume(TaskHandle_t xTaskToResume)
{
    return MockFreertos::getInstance().vTaskResume(xTaskToResume);
}
