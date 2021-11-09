#ifndef DHT_H
#define DHT_H

#include "driver/gpio.h"
#include "DHT.hpp"

class Dht
{
  public:
    Dht(gpio_num_t pin);
    int readData();
    float temperature();
    float humidity();
  private:
    DHT dht;
    bool dataReady;
    gpio_num_t pin;
};

#endif
