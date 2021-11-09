#ifndef WIFI_H
#define WIFI_H

#define MAXIMUM_WIFI_CONNECTION_RETRY 5

class Wifi
{
  public:
    Wifi(const char *ssid, const char *password);
    void connect();
    void disconnect();
    void deepSleep();
  private:
    const char *ssid;
    const char *password;
};

#endif
