#include "wifi_states.h"

#include <esp_log.h>

void Initial::setContext(WifiStateMachineInterface* context)
{
    ESP_LOGI("INITIAL", "Setting context");

    if (context_ == nullptr)
    {
        if (context != nullptr)
        {
            context_ = std::shared_ptr<WifiStateMachineInterface>(context);
        }
        else
        {
            ESP_LOGE("INITIAL", "Wrong context parameter");
        }
    }
    else
    {
        ESP_LOGI("INITIAL", "Context already set");
    }
}

std::shared_ptr<WifiStateMachineInterface> WifiAp::context_;
bool WifiAp::stop_task{false};

void WifiAp::AP_task(void* pvParameters)
{
    // cppcheck-suppress variableScope
    std::string wifi_sta_ssid{};

    ESP_LOGI("WIFI_AP", "Entering task");

    for (;;)
    {
        wifi_sta_ssid = Wifi::getInstance().getWifiSsidSta();

        if (!wifi_sta_ssid.empty())
        {
            if (context_ != nullptr)
            {
                wifi_sta_ssid.clear();
                if (context_->dispatch(LoginCredentialsReceived()))
                {
                    if (stop_task)
                    {
                        stop_task = false;
                        vTaskSuspend(nullptr);
                    }
                }
            }
            else
            {
                ESP_LOGI("WIFI_AP", "Wifi ap context is nullptr");
            }
        }
        ESP_LOGI("WIFI_AP", "Wifi ap task loop");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

WifiAp::WifiAp()
{
    ESP_LOGI("WIFI_AP", "Constructor");

    if (xTaskCreate(AP_task, "AP task", 4096, nullptr, 5, &AP_task_handle) !=
        pdPASS)
    {
        assert(!"Failed to create AP_task");
    }
    vTaskSuspend(AP_task_handle);
}

void WifiAp::startTask()
{
    ESP_LOGI("WIFI_AP", "Start task");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    Wifi::getInstance().startAP();
    vTaskResume(AP_task_handle);
}

void WifiAp::stopTask()
{
    ESP_LOGI("WIFI_AP", "Stop task");

    vTaskDelay(500 / portTICK_PERIOD_MS);

    Wifi::getInstance().stopAP();
    stop_task = true;
}

void WifiAp::setContext(WifiStateMachineInterface* context)
{
    ESP_LOGI("WIFI_AP", "Setting context");

    if (context_ == nullptr)
    {
        if (context != nullptr)
        {
            context_ = std::shared_ptr<WifiStateMachineInterface>(context);
        }
        else
        {
            ESP_LOGE("WIFI_AP", "Wrong context parameter");
        }
    }
    else
    {
        ESP_LOGI("WIFI_AP", "Context already set");
    }
}

std::shared_ptr<WifiStateMachineInterface> WifiSta::context_;
bool WifiSta::login_network_fail{false};
bool WifiSta::stop_task{false};

void WifiSta::STA_task(void* pvParameters)
{
    // cppcheck-suppress variableScope
    std::string time_date{};
    const size_t string_size{100};
    time_date.resize(string_size);

    ESP_LOGI("WIFI_STA", "Entering wifi sta task");

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    for (;;)
    {
        if (login_network_fail)
        {
            login_network_fail = false;
            context_->dispatch(LoginToNetworkFailed());
        }

        if (!Wifi::getInstance().getWifiStaStatus())
        {
            context_->dispatch(StaConnectionFailed());
        }

        if (stop_task)
        {
            stop_task = false;
            vTaskSuspend(nullptr);
        }

        if (getTimeDate(time_date.data(), string_size))
        {
            ESP_LOGI("Wifi state machine", "%s", time_date.data());
            time_date.clear();
        }
        ESP_LOGI("WIFI_STA", "Wifi sta task loop");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

WifiSta::WifiSta()
{
    ESP_LOGI("WIFI_STA", "Constructor");

    if (xTaskCreate(STA_task, "STA task", 4096, nullptr, 5, &STA_task_handle) !=
        pdPASS)
    {
        assert(!"Failed to create STA_task");
    }

    vTaskSuspend(STA_task_handle);
}

void WifiSta::startTask()
{
    ESP_LOGI("WIFI_STA", "Start task");

    vTaskDelay(500 / portTICK_PERIOD_MS);

    if (!Wifi::getInstance().startSTA())
    {
        login_network_fail = true;
    }
    vTaskResume(STA_task_handle);
}

void WifiSta::stopTask()
{
    ESP_LOGI("WIFI_STA", "Stop task");

    vTaskDelay(500 / portTICK_PERIOD_MS);

    Wifi::getInstance().stopSTA();
    stop_task = true;
}

void WifiSta::setContext(WifiStateMachineInterface* context)
{
    ESP_LOGI("WIFI_STA", "Setting context");

    if (context_ == nullptr)
    {
        if (context != nullptr)
        {
            context_ = std::shared_ptr<WifiStateMachineInterface>(context);
        }
        else
        {
            ESP_LOGE("WIFI_STA", "Wrong context parameter");
        }
    }
    else
    {
        ESP_LOGI("WIFI_STA", "Context already set");
    }
}

void Disconnected::setContext(WifiStateMachineInterface* context)
{
    ESP_LOGI("DISCONNECTED", "Setting context");

    if (context_ == nullptr)
    {
        if (context != nullptr)
        {
            context_ = std::shared_ptr<WifiStateMachineInterface>(context);
        }
        else
        {
            ESP_LOGE("DISCONNECTED", "Wrong context parameter");
        }
    }
    else
    {
        ESP_LOGI("DISCONNECTED", "Context already set");
    }
}