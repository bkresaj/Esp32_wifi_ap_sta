#include "mock_esp_event.h"

esp_err_t esp_event_loop_create_default()
{
    return MockEspEvent::getInstance().esp_event_loop_create_default();
}

esp_err_t esp_event_handler_instance_register(
    esp_event_base_t event_base, int32_t event_id,
    esp_event_handler_t event_handler, void *event_handler_arg,
    esp_event_handler_instance_t *instance)
{
    return MockEspEvent::getInstance().esp_event_handler_instance_register(
        event_base, event_id, event_handler, event_handler_arg, instance);
}
