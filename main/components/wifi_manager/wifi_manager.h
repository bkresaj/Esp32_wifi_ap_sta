#pragma once

#include <variant>

#include "esp_log.h"
#include "wifi_state_machine_interface.h"
#include "wifi_transition.h"

class WifiManager : public WifiStateMachineInterface
{
   public:
    /**
     * Function for getting static instance.
     *
     * @return WifiManager instance.
     */
    static WifiManager& getInstance();

    /**
     * @brief Dispatches a WiFi event to handle state transitions.
     *
     * This function is responsible for handling WiFi events and managing state
     * transitions within the WiFi manager. It takes a WifiEvent as a parameter
     * and processes the event, updating the internal state accordingly.
     *
     * @param wifi_event The WiFi event to be dispatched and processed.
     * @return true if the dispatch and state transition were successful, false
     * otherwise.
     *
     * @note This function is part of the WifiStateMachineInterface and must be
     * implemented by classes that derive from it.
     */
    bool dispatch(WifiEvent wifi_event) override;

   private:
    WifiManager();
    ~WifiManager() = default;

    WifiState wifi_state_;
};