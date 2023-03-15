/**
   my_rtc.h

   this is a simple set of useful functions for interacting with the
   RTC built into the Arduino MKR WiFi 1010 boards. under the hood
   it uses the RTCZero library, but provides some friendlier functions
   for setting the initial date and time and returning nicely formatted
   date / time strings.

   author: alex shenfield
   date:   24-10-2022
*/

// prevent recursive includes
#ifndef MyRTC_h
#define MyRTC_h

// include the basic Arduino commands and the RTCZero library for access
// to the MKR WiFi 1010 RTC
#include "Arduino.h"
#include <RTCZero.h>

// this is my rtc library
class MyRTC
{
  public:
    MyRTC();
    void set_rtc();
    String get_rtc_time();
    String get_rtc_date();
  private:
    RTCZero _rtc;
};

#endif
