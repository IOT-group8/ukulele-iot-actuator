#include <stdio.h>
#include <string.h>
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "firebase_http.h"
#include "esp_timer.h"

#define TAG "FIREBASE_HTTP"

static char firebase_auth_token[1024] = {0}; // Store the OAuth 2.0 access token

/**
 * Generates and retrieves an OAuth 2.0 token using Firebase Service Account.
 */
void firebase_get_oauth_token() {
    const char *post_data = "{"
        "\"grant_type\": \"urn:ietf:params:oauth:grant-type:jwt-bearer\","
        "\"assertion\": \"GENERATED_JWT\""
    "}";

    esp_http_client_config_t config = {
        .url = FIREBASE_TOKEN_URI,
        .method = HTTP_METHOD_POST,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        char response[1024] = {0};
        esp_http_client_read(client, response, sizeof(response) - 1);
        ESP_LOGI(TAG, "Token Response: %s", response);

        // Parse JSON
        cJSON *json = cJSON_Parse(response);
        if (json) {
            cJSON *token = cJSON_GetObjectItem(json, "access_token");
            if (token) {
                snprintf(firebase_auth_token, sizeof(firebase_auth_token), "Bearer %s", token->valuestring);
                ESP_LOGI(TAG, "Firebase Auth Token: %s", firebase_auth_token);
            }
            cJSON_Delete(json);
        }
    } else {
        ESP_LOGE(TAG, "OAuth Token Request Failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

/**
 * Sends JSON data to Firebase Realtime Database.
 */
void firebase_send_data(const char *topic, float temperature, float humidity) {
    if (strlen(firebase_auth_token) == 0) {
        ESP_LOGE(TAG, "No Auth Token Available. Requesting...");
        firebase_get_oauth_token();
    }

    char url[256];
    snprintf(url, sizeof(url), "%s%s.json", FIREBASE_DATABASE_URL, topic);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature", temperature);
    cJSON_AddNumberToObject(root, "humidity", humidity);
    cJSON_AddNumberToObject(root, "timestamp", (double) esp_timer_get_time() / 1000);
    char *post_data = cJSON_Print(root);
    cJSON_Delete(root);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_PUT,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", firebase_auth_token);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Sent to topic %s: %s", topic, post_data);
    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    free(post_data);
}

/**
 * Fetches JSON data from Firebase Realtime Database.
 */
void firebase_fetch_data(const char *topic) {
    if (strlen(firebase_auth_token) == 0) {
        ESP_LOGE(TAG, "No Auth Token Available. Requesting...");
        firebase_get_oauth_token();
    }

    char url[256];
    snprintf(url, sizeof(url), "%s%s.json", FIREBASE_DATABASE_URL, topic);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Authorization", firebase_auth_token);
    esp_http_client_perform(client);

    int status = esp_http_client_get_status_code(client);
    if (status == 200) {
        char response[1024] = {0};
        esp_http_client_read(client, response, sizeof(response) - 1);
        ESP_LOGI(TAG, "Received from topic %s: %s", topic, response);

        // Parse JSON
        cJSON *json = cJSON_Parse(response);
        if (json) {
            cJSON *temperature = cJSON_GetObjectItem(json, "temperature");
            cJSON *humidity = cJSON_GetObjectItem(json, "humidity");
            if (temperature && humidity) {
                ESP_LOGI(TAG, "Temperature: %.2f°C, Humidity: %.2f%%", temperature->valuedouble, humidity->valuedouble);
            } else {
                ESP_LOGW(TAG, "No temperature or humidity data found.");
            }
            cJSON_Delete(json);
        } else {
            ESP_LOGE(TAG, "Failed to parse JSON");
        }
    } else {
        ESP_LOGE(TAG, "HTTP Error: %d", status);
    }

    esp_http_client_cleanup(client);
}
