#pragma once

#include <cstdint>
typedef const char*
    esp_event_base_t; /**< unique pointer to a subsystem that exposes events */
typedef void* esp_event_loop_handle_t; /**< a number that identifies an event
                                          with respect to a base */
typedef void (*esp_event_handler_t)(
    void* event_handler_arg, esp_event_base_t event_base, int32_t event_id,
    void* event_data); /**< function called when an event is posted to the queue
                        */
typedef void* esp_event_handler_instance_t; /**< context identifying an instance
                                               of a registered event handler */

#define ESP_EVENT_ANY_BASE NULL /**< register handler for any event base */
#define ESP_EVENT_ANY_ID -1     /**< register handler for any event id */