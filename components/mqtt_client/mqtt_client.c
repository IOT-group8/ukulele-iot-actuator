#include <stdio.h>
#include "mqtt_client.h"
#include "esp_log.h"

static const char *TAG = "MQTT";

#define MQTT_BROKER_URI "mqtt://broker.hivemq.com"  // Change this to your MQTT broker
#define MQTT_TOPIC "esp32/ukulele"                  // Topic to subscribe to

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected!");
            esp_mqtt_client_subscribe(client, MQTT_TOPIC, 1);  // Subscribe to topic
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
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_BROKER_URI,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    ESP_LOGI(TAG, "MQTT client started...");
}
