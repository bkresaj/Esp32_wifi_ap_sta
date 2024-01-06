#pragma once

#include "wifi_state_machine_interface.h"

/**
 * @brief Interface for WiFi states.
 *
 * The StateInterface class defines an interface for various WiFi states.
 * Classes representing specific states should implement this interface to
 * ensure a consistent structure for managing state-related tasks and
 * transitions.
 */
class StateInterface
{
   public:
    /**
     * @brief Virtual function to stop task associated with the state.
     */
    virtual void stopTask() = 0;

    /**
     * @brief Virtual function to start tasks associated with the state.
     */
    virtual void startTask() = 0;

    /**
     * @brief Virtual function to set the context for the state.
     *
     * The context typically refers to the WiFi state machine interface,
     * allowing the state to interact with the overall WiFi management system.
     *
     * @param context A pointer to the WiFi state machine interface.
     */
    virtual void setContext(WifiStateMachineInterface* context) = 0;

    /**
     * @brief Virtual destructor for the StateInterface class.
     *
     * This ensures proper destruction of derived classes through the interface
     * pointer.
     */
    virtual ~StateInterface() = default;
};