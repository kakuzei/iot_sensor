#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/rtc_io.h"
#include "led.h"

Led::Led(gpio_num_t pin): pin(pin)
{
  gpio_reset_pin(pin);
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void Led::turnOn()
{
  gpio_set_level(pin, 1);
}

void Led::turnOff()
{
  gpio_set_level(pin, 0);
}

void Led::blink(int times, int repeat)
{
  for (int y=0; y<repeat; y++)
  {
    for (int i=0; i<times; i++)
    {
      this->turnOn();
      vTaskDelay(pdMS_TO_TICKS(200));
      this->turnOff();
      vTaskDelay(pdMS_TO_TICKS(200));
    }
    vTaskDelay(pdMS_TO_TICKS(600));
  }
}

void Led::deepSleep()
{
  gpio_set_level(pin, 0);
  rtc_gpio_isolate(pin);
}
