#include <stdio.h>
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "wifi_connect.h"

static const char *TAG = "MQTT";

#define MQTT_BROKER_URI "mqtt://192.168.189.69"  // Change this to your MQTT broker
#define MQTT_TOPIC "sensor/raw"                  // Topic to subscribe to

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected!");
            esp_mqtt_client_subscribe(client, MQTT_TOPIC, 1);
            ESP_LOGI(TAG, "Subscribed to topic: %s", MQTT_TOPIC);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received data on topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Message: %.*s", event->data_len, event->data);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT Disconnected!");
            break;

        default:
            break;
    }
}

void mqtt_app_start(void) {
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,  // Correct field assignment
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    ESP_LOGI(TAG, "MQTT client started...");
}
