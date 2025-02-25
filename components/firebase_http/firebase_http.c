#include <stdio.h>
#include "esp_http_client.h"
#include "esp_log.h"

#define FIREBASE_URL "https://your-project-id.firebaseio.com/data.json"
#define TAG "FIREBASE"

static void firebase_http_task(void *pvParameter) {
    esp_http_client_config_t config = {
        .url = FIREBASE_URL
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    while (1) {
        esp_http_client_perform(client);
        
        int status = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "HTTP Status = %d", status);

        if (status == 200) {
            char response[1024] = {0};
            esp_http_client_read(client, response, sizeof(response)-1);
            ESP_LOGI(TAG, "Received: %s", response);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);  // Poll every 5 seconds
    }

    esp_http_client_cleanup(client);
    vTaskDelete(NULL);
}

void start_firebase_subscription() {
    xTaskCreate(&firebase_http_task, "firebase_http_task", 8192, NULL, 5, NULL);
}
