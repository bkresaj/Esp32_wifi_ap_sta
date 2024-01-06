#pragma once

#include "port_macro.h"

typedef TickType_t EventBits_t;

struct EventGroupDef_t;
typedef struct EventGroupDef_t *EventGroupHandle_t;

EventGroupHandle_t xEventGroupCreate();
EventBits_t xEventGroupWaitBits(const EventGroupHandle_t xEventGroup,
                                const EventBits_t uxBitsToWaitFor,
                                const BaseType_t xClearOnExit,
                                const BaseType_t xWaitForAllBits,
                                TickType_t xTicksToWait);
void vEventGroupDelete(EventGroupHandle_t xEventGroup);
EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup,
                               const EventBits_t uxBitsToSet);
