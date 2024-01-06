#include "wifi_transition.h"

static WifiAp wifi_ap;
static WifiSta wifi_sta;
static Disconnected disconnected;

template <typename... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};
template <typename... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

WifiState processTransition(const WifiState& currentState,
                            const WifiEvent& event)
{
    return std::visit(
        Overloaded{
            [](const auto& state, const auto& event)
            { return wifiTransition(state, event); },
        },
        currentState, event);
}

WifiState wifiTransition(const Initial& state, const Start& event)
{
    ESP_LOGI("Wifi transition", "Transition from Initial to WifiAp");
    return wifi_ap;
}

WifiState wifiTransition(const WifiAp& state,
                         const LoginCredentialsReceived& event)
{
    ESP_LOGI("Wifi transition", "Transition from WifiAp to WifiSta");
    return wifi_sta;
}

WifiState wifiTransition(const WifiAp& state, const Disconnect& event)
{
    ESP_LOGI("Wifi transition", "Transition from WifiAp to Disconnected");
    return disconnected;
}

WifiState wifiTransition(const WifiSta& state,
                         const LoginToNetworkFailed& event)
{
    ESP_LOGI("Wifi transition", "Transition from WifiSta to WifiAp");
    return wifi_ap;
}

WifiState wifiTransition(const WifiSta& state, const StaConnectionFailed& event)
{
    ESP_LOGI("Wifi transition", "Transition from WifiSta to WifiAp");
    return wifi_ap;
}

WifiState wifiTransition(const WifiSta& state, const Disconnect& event)
{
    ESP_LOGI("Wifi transition", "Transition from WifiSta to Disconnected");
    return disconnected;
}

WifiState wifiTransition(const Disconnected& state, const Start& event)
{
    ESP_LOGI("Wifi transition", "Transition from Disconnected to WifiAp");
    return wifi_ap;
}