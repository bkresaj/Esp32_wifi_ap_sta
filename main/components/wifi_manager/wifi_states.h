#pragma once

#include <memory>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ntp_server.h"
#include "wifi.h"
#include "wifi_state_interface.h"

/**
 * @brief Represents the initial state of the WiFi system.
 *
 * The Initial class is derived from the StateInterface and serves as the
 * starting state in the WiFi state machine.
 */
class Initial : public StateInterface
{
   public:
    Initial() = default;
    void startTask() override{};
    void stopTask() override{};
    void setContext(WifiStateMachineInterface* context) override;

   private:
    std::shared_ptr<WifiStateMachineInterface> context_;
};

/**
 * @brief Represents the state when the WiFi system is in Access Point (AP)
 * mode.
 *
 * The WifiAp class is derived from the StateInterface and provides specific
 * implementations for the pure virtual functions defined in StateInterface.
 */
class WifiAp : public StateInterface
{
   public:
    WifiAp();
    void startTask() override;
    void stopTask() override;
    void setContext(WifiStateMachineInterface* context) override;

   private:
    static void AP_task(void* pvParameters);
    static std::shared_ptr<WifiStateMachineInterface> context_;
    static bool stop_task;
    TaskHandle_t AP_task_handle{};
    std::string wifi_ap_ssid{};
    std::string wifi_ap_password{};
};

/**
 * @brief Represents the state when the WiFi system is in Station (STA) mode.
 *
 * The WifiSta class is derived from the StateInterface and provides specific
 * implementations for the pure virtual functions defined in StateInterface.
 */
class WifiSta : public StateInterface
{
   public:
    WifiSta();
    void startTask() override;
    void stopTask() override;
    void setContext(WifiStateMachineInterface* context) override;

   private:
    static void STA_task(void* pvParameters);
    static std::shared_ptr<WifiStateMachineInterface> context_;
    TaskHandle_t STA_task_handle{};
    static bool login_network_fail;
    static bool stop_task;
    std::string wifi_sta_ssid;
    std::string wifi_sta_password;
};

/**
 * @brief Represents the state when the WiFi system is in disconnected mode.
 *
 * The WifiSta class is derived from the StateInterface and provides specific
 * implementations for the pure virtual functions defined in StateInterface.
 */
class Disconnected : public StateInterface
{
   public:
    Disconnected() = default;
    void startTask() override{};
    void stopTask() override{};
    void setContext(WifiStateMachineInterface* context) override;

   private:
    std::shared_ptr<WifiStateMachineInterface> context_;
};

/**
 * @brief Type alias for a variant representing various WiFi states.
 *
 * The WifiState variant can hold instances of Initial, WifiAp,
 * WifiSta, or Disconnected.
 */
using WifiState = std::variant<Initial, WifiAp, WifiSta, Disconnected>;
