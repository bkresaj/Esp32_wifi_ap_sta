#pragma once

#include <variant>

/**
 * @brief Represents the event when the WiFi system is started.
 */
struct Start
{
};

/**
 * @brief Represents the event when login credentials are received.
 */
struct LoginCredentialsReceived
{
};

/**
 * @brief Represents the event when login to the network fails.
 */

struct LoginToNetworkFailed
{
};

/**
 * @brief Represents the event when the station (Sta) connection fails.
 */
struct StaConnectionFailed
{
};

/**
 * @brief Represents the event when disconnect should occur.
 */
struct Disconnect
{
};

/**
 * @brief Type alias for a variant representing various WiFi events.
 *
 * The WifiEvent variant can hold instances of Start, LoginCredentialsReceived,
 * LoginToNetworkFailed, StaConnectionFailed, or Disconnect.
 */
using WifiEvent =
    std::variant<Start, LoginCredentialsReceived, LoginToNetworkFailed,
                 StaConnectionFailed, Disconnect>;