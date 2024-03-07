#include "wifi_manager.h"

#include <typeinfo>

#include "https_server.h"
#include "spiffs.h"

WifiManager& WifiManager::getInstance()
{
    static WifiManager wifi_manager;
    return wifi_manager;
}

WifiManager::WifiManager() : wifi_state_(Initial())
{
}

bool WifiManager::dispatch(WifiEvent wifi_event)
{
    std::visit(
        [this](const auto& wifi_state)
        {
            const char* typeName = typeid(wifi_state).name();
            // Remove the first character from the type name
            ESP_LOGI("WifiState", "Current state: %s", typeName + 1);
        },
        wifi_state_);

    std::visit([](auto& wifi_state) { wifi_state.stopTask(); }, wifi_state_);

    wifi_state_ = processTransition(wifi_state_, wifi_event);

    std::visit([this](auto& wifi_state) { wifi_state.setContext(&(*this)); },
               wifi_state_);

    std::visit([](auto& wifi_state) { wifi_state.startTask(); }, wifi_state_);
    return true;
}
