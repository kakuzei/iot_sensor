#ifndef NTP_H
#define NTP_H

class Ntp
{
  public:
    Ntp();
    time_t getCurrentTime();
    int64_t getTimeMicroSecond();
    int64_t getTimeNanoSecond();
};

#endif
