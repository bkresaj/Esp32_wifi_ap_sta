#pragma once

#include "wifi_events.h"

/**
 * @brief Interface for a WiFi state machine.
 *
 * The WifiStateMachineInterface class defines an interface for a WiFi state
 * machine. Classes implementing this interface should handle the dispatching of
 * WiFi events to manage state transitions and overall WiFi system behavior.
 */
class WifiStateMachineInterface
{
   public:
    /**
     * @brief Virtual function to dispatch WiFi events for state transitions.
     *
     * This function is responsible for processing WiFi events and managing
     * state transitions within the WiFi state machine.
     *
     * @param wifi_event The WiFi event to be dispatched and processed.
     * @return true if the dispatch and state transition were successful, false
     * otherwise.
     */
    virtual bool dispatch(WifiEvent wifi_event) = 0;

    /**
     * @brief Virtual destructor for the WifiStateMachineInterface class.
     *
     * This ensures proper destruction of derived classes through the interface
     * pointer.
     */
    virtual ~WifiStateMachineInterface() = default;
};