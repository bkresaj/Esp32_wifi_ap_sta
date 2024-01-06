#pragma once

#include <esp_log.h>

#include <variant>

#include "wifi_events.h"
#include "wifi_states.h"

WifiState processTransition(const WifiState& currentState,
                            const WifiEvent& event);

template <typename StateType, typename EventType>
WifiState wifiTransition(const StateType& state, const EventType& event)
{
    // Default behavior: just return the current state, effectively ignoring the
    // event
    ESP_LOGI("Wifi transition", "Ignoring event, staying in current state.");
    return state;
}

WifiState wifiTransition(const Initial& state, const Start& event);

WifiState wifiTransition(const WifiAp& state,
                         const LoginCredentialsReceived& event);

WifiState wifiTransition(const WifiAp& state, const Disconnect& event);

WifiState wifiTransition(const WifiSta& state,
                         const LoginToNetworkFailed& event);

WifiState wifiTransition(const WifiSta& state,
                         const StaConnectionFailed& event);

WifiState wifiTransition(const WifiSta& state, const Disconnect& event);

WifiState wifiTransition(const Disconnect& state, const Start& event);
