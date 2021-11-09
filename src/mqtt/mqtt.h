#ifndef MQTT_H
#define MQTT_H

#include "mqtt_client.h"

class Mqtt
{
  public:
    Mqtt(const char *uri, const char *username, const char *password, const char *clientId);
    void connect();
    void disconnect();
    void publish(const char *topic, const char *data);
  private:
    const char *uri;
    const char *username;
    const char *password;
    const char *clientId;
    esp_mqtt_client_handle_t client;
};

#endif
