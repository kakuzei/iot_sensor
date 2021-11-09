#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "dht.h"

// logger
static const char *TAG = "Dht::Dht";

Dht::Dht(gpio_num_t pin): pin(pin)
{
  this->dht.setDHTgpio(pin);
  this->dataReady = false;
}

int Dht::readData()
{
  int retryCount = 0;
  int ret = -1;
  this->dataReady = false;
  while (retryCount < 5 && ret != DHT_OK)
  {
    ESP_LOGI(TAG, "Reading DHT22 data...");
    ret = this->dht.readDHT();
    this->dht.errorHandler(ret);
    if (retryCount > 0) {
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    retryCount++;
  }
  if (ret == DHT_OK) {
    this->dataReady = true;
  }
  return ret;
}

float Dht::temperature()
{
  if (!this->dataReady) {
    ESP_LOGE(TAG, "Data are not ready");
  }
  return dht.getTemperature();
}

float Dht::humidity()
{
  if (!this->dataReady) {
    ESP_LOGE(TAG, "Data are not ready");
  }
  return dht.getHumidity();
}
