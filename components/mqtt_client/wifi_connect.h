#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define WIFI_SSID "MQTT"
#define WIFI_PASS "sibingeorge1"

void wifi_init_sta(void);

#endif // WIFI_CONNECT_H
