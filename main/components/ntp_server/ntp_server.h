#ifndef NTP_SERVER_H_
#define NTP_SERVER_H_

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Initialize the NTP (Network Time Protocol) server.
     *
     * This function initializes the NTP server, allowing the application
     * to synchronize its time with a network time server.
     *
     * @return true if the NTP server initialization is successful, false
     * otherwise.
     */
    bool initNtpServer();

    /**
     * Function for getting time and date.
     *
     * @param time_buffer Buffer in which time and date will be copied to.
     * @param size Size of buffer.
     * @return Bool returning true on success or false on failure.
     */
    bool getTimeDate(char* time_buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // NTP_SERVER_H_