#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_sleep.h"

#include "dht/dht.h"
#include "led/led.h"
#include "mqtt/mqtt.h"
#include "ntp/ntp.h"
#include "nvs/nvs.h"
#include "wifi/wifi.h"
#include "env.h"

static const char TAG[] = "MAIN";

extern "C" {
  void app_main(void);
}

void main_task(void *pvParameters)
{
  ESP_LOGI(TAG, "Start the execution of the main task");
  Led busyLed = Led(GPIO_NUM_2); // GPIO_NUM_27 = D4 // GPIO_NUM_2 = D9 = Internal LED
  busyLed.turnOn();
  vTaskDelay(pdMS_TO_TICKS(1000));
  
  ESP_LOGI(TAG, "Initialize the default NVS partition");
  Nvs nvs = Nvs();
  nvs.initialize();

  ESP_LOGI(TAG, "Connecting to Wifi");
  Wifi wifi = Wifi(WIFI_SSID, WIFI_PASSWORD);
  wifi.connect();

  ESP_LOGI(TAG, "Retrieving current date...");
  Ntp ntp = Ntp();
  time_t currentTime = ntp.getCurrentTime();
  ESP_LOGI(TAG, "Current time: %ld\n", currentTime);

  ESP_LOGI(TAG, "Reading sensor data...");
  Dht dht = Dht(GPIO_NUM_5); // GPIO_NUM_5 = D8
  int status = dht.readData();
  char* message;
  if (status == ESP_OK) {
    asprintf(
      &message,
      "sensors,location=tom_room,sensor_type=dht22,temperature_unit=celsius,humidity_unit=relative_humidity,environment=production temperature=%.2f,humidity=%.2f %lld",
      dht.temperature(),
      dht.humidity(),
      ntp.getTimeNanoSecond()
    );
    ESP_LOGI(TAG, "Message: %s", message);

    ESP_LOGI(TAG, "Connecting to MQTT");
    Mqtt mqtt = Mqtt(MQTT_URI, MQTT_USERNAME, MQTT_PASSWORD, MQTT_CLIENT_ID);
    mqtt.connect();
    ESP_LOGI(TAG, "Sending message to MQTT");
    mqtt.publish("sensors/temperature", message);
    ESP_LOGI(TAG, "Disconnecting from MQTT");
    mqtt.disconnect();
  } else {
    ESP_LOGE(TAG, "Unable to read sensor data!");
    busyLed.blink(5, 3);
  }

  ESP_LOGI(TAG, "Disconnecting from Wifi");
  wifi.disconnect();

  ESP_LOGI(TAG, "Configure deep sleep timer");
  ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(5ULL * 60 * 1000 * 1000)); // sleep 5 minutes

  ESP_LOGI(TAG, "Entering in deep sleep");
  busyLed.blink(3, 2);
  busyLed.deepSleep();
  rtc_gpio_isolate(GPIO_NUM_12); // as the doc say
  esp_deep_sleep_start();
}

void app_main() {
  TaskHandle_t taskHanlde = NULL;
  xTaskCreate(
    main_task,
    "main_task",
    8192,
    (void* ) 0,
    (tskIDLE_PRIORITY + 1),
    &taskHanlde
  );
}