#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gatt_common_api.h"

#define GATTS_TAG "M5CS3_GATTS"

/// Service / Characteristic UUID
#define GATTS_SERVICE_UUID   0x00FF //0x180A
#define GATTS_CHAR_UUID      0xFF01 //0x2A57
#define GATTS_NUM_HANDLE     4

#define BT_DEVICE_ID                "01"

#define BT_DEVICE_NAME_PREFIX       "M5mrb"
#define BT_DEVICE_NAME_FORMAT       BT_DEVICE_NAME_PREFIX "%02d"
static char sDeviceName[] = BT_DEVICE_NAME_PREFIX BT_DEVICE_ID;
#define BT_DEVICE_NAME              sDeviceName

static uint16_t gatt_service_handle;
static esp_gatt_srvc_id_t gatt_service_id;
static uint16_t gatt_char_handle;
static esp_bt_uuid_t gatt_char_uuid;

static esp_gatt_if_t gatts_if_global = 0;
static uint16_t conn_id = 0;
static bool notify_enabled = false;

static uint8_t char_value[20] = "Hello CoreS3!";

/// ADV parameters
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .peer_addr          = {0},
    .peer_addr_type     = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

/// advertising data
static uint8_t adv_service_uuid128[16] = {
    /* 128bit UUIDならここに設定 */
    0xfb, 0x34, 0x9b, 0x5f,
    0x80, 0x00, 0x00, 0x80,
    0x00, 0x10, 0x00, 0x00,
    0x57, 0x2A, 0x0A, 0x18, // UUID 0x180A, Char 0x2A57
};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp        = false,
    .include_name        = true,
    .include_txpower     = true,
    .min_interval        = 0x0006,
    .max_interval        = 0x0010,
    .appearance          = 0x00,
    .manufacturer_len    = 0,
    .p_manufacturer_data = NULL,
    .service_data_len    = 0,
    .p_service_data      = NULL,
    .service_uuid_len    = sizeof(adv_service_uuid128),
    .p_service_uuid      = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

/// GAP callback
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        esp_ble_gap_start_advertising(&adv_params);
        break;
    default:
        break;
    }
}

/// GATT callback
static void gatts_event_handler(esp_gatts_cb_event_t event,
                                esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
    case ESP_GATTS_REG_EVT:
    {
        ESP_LOGI(GATTS_TAG, "REGISTER_APP_EVT");
        gatt_service_id.is_primary = true;
        gatt_service_id.id.inst_id = 0x00;
        gatt_service_id.id.uuid.len = ESP_UUID_LEN_16;
        gatt_service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID;

        // esp_ble_gap_set_device_name("M5S3MRB01");
        esp_ble_gap_set_device_name(sDeviceName);
        esp_ble_gap_config_adv_data(&adv_data);

        esp_ble_gatts_create_service(gatts_if, &gatt_service_id, GATTS_NUM_HANDLE);
        break;
    }

    case ESP_GATTS_CREATE_EVT:
    {
        ESP_LOGI(GATTS_TAG, "CREATE_SERVICE_EVT, service_handle=%d", param->create.service_handle);
        gatt_service_handle = param->create.service_handle;

        gatt_char_uuid.len = ESP_UUID_LEN_16;
        gatt_char_uuid.uuid.uuid16 = GATTS_CHAR_UUID;

        esp_gatt_char_prop_t char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;

        esp_err_t add_char_ret = esp_ble_gatts_add_char(gatt_service_handle,
                                                        &gatt_char_uuid,
                                                        ESP_GATT_PERM_READ,
                                                        char_property,
                                                        NULL, NULL);
        if (add_char_ret) {
            ESP_LOGE(GATTS_TAG, "add char failed, error code =%x", add_char_ret);
        }
        esp_ble_gatts_start_service(gatt_service_handle);
        break;
    }

    case ESP_GATTS_ADD_CHAR_EVT:
    {
        ESP_LOGI(GATTS_TAG, "ADD_CHAR_EVT, char_handle = %d", param->add_char.attr_handle);
        gatt_char_handle = param->add_char.attr_handle;
        break;
    }

    case ESP_GATTS_CONNECT_EVT:
    {
        ESP_LOGI(GATTS_TAG, "CONNECT_EVT, conn_id = %d", param->connect.conn_id);
        gatts_if_global = gatts_if;
        conn_id = param->connect.conn_id;
        break;
    }

    case ESP_GATTS_DISCONNECT_EVT:
    {
        ESP_LOGI(GATTS_TAG, "DISCONNECT_EVT, reason = 0x%x", param->disconnect.reason);
        esp_ble_gap_start_advertising(&adv_params);
        notify_enabled = false;
        break;
    }

    case ESP_GATTS_READ_EVT:
    {
        ESP_LOGI(GATTS_TAG, "GATT_READ_EVT");
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
        rsp.attr_value.len = strlen((char *)char_value);
        memcpy(rsp.attr_value.value, char_value, rsp.attr_value.len);
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id,
                                    param->read.trans_id, ESP_GATT_OK, &rsp);
        break;
    }
    
    case ESP_GATTS_CONF_EVT:
    {
        ESP_LOGI(GATTS_TAG, "CONF_EVT");
        break;
    }

    case ESP_GATTS_SET_ATTR_VAL_EVT:
    {
        ESP_LOGI(GATTS_TAG, "SET_ATTR_VAL_EVT");
        break;
    }

    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        break;

    case ESP_GATTS_EXEC_WRITE_EVT:
    case ESP_GATTS_WRITE_EVT:
    {
        if (param->write.handle == gatt_char_handle + 1) { // CCCD
            if (param->write.len == 2 && param->write.value[0] == 1) {
                notify_enabled = true;
                ESP_LOGI(GATTS_TAG, "Notify enabled");
            } else {
                notify_enabled = false;
                ESP_LOGI(GATTS_TAG, "Notify disabled");
            }
        }
        break;
    }

    default:
        break;
    }
}

/// Notifyタスク
void notify_task(void *pvParameter)
{
    while (1) {
        if (notify_enabled) {
            snprintf((char *)char_value, sizeof(char_value), "Tick %lu", (unsigned long)xTaskGetTickCount());
            esp_ble_gatts_send_indicate(gatts_if_global, conn_id, gatt_char_handle,
                                        strlen((char *)char_value), char_value, false);
            ESP_LOGI(GATTS_TAG, "Notify: %s", char_value);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// void app_main(void)
void gatt_app_main(void)
{
    // esp_err_t ret;

    // ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));

    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(gatts_event_handler));
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(gap_event_handler));
    ESP_ERROR_CHECK(esp_ble_gatts_app_register(0));

    xTaskCreate(notify_task, "notify_task", 4096, NULL, 5, NULL);
}