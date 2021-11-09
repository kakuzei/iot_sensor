#ifndef LED_H
#define LED_H

#include "driver/gpio.h"

class Led
{
  public:
    Led(gpio_num_t pin);
    void turnOn();
    void turnOff();
    void blink(int times, int repeat);
    void deepSleep();
  private:
    gpio_num_t pin;
};

#endif
