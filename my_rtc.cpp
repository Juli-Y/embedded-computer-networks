/**
   my_rtc.cpp

   this is a simple set of useful functions for interacting with the
   RTC built into the Arduino MKR WiFi 1010 boards. under the hood
   it uses the RTCZero library, but provides some friendlier functions
   for setting the initial date and time and returning nicely formatted
   date / time strings.

   author: alex shenfield
   date:   24-10-2022
*/

// my library includes
#include "Arduino.h"
#include "my_rtc.h"

// constructor
MyRTC::MyRTC()
{
  _rtc.begin();
}

// initialise the rtc time and date to the compile time and date
void MyRTC::set_rtc()
{
  // set the time ...

  // get the compile time from the predefined macro - see
  // https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
  // e.g. 10:15:30
  String t = __TIME__;
  uint8_t hours   = t.substring(0, 2).toInt();
  uint8_t minutes = t.substring(3, 5).toInt();
  uint8_t seconds = t.substring(6, 8).toInt();
  _rtc.setTime(hours, minutes, seconds);

  // set the date ...

  // get compile date from the predefined macro - e.g. Oct 08 2022
  String d = __DATE__;

  String  s_month = d.substring(0, 3);
  uint8_t day     = d.substring(4, 6).toInt();
  uint8_t year    = d.substring(9, 11).toInt();

  // parse the month to get as an integer (there are cleverer ways to
  // do this, but they are quite complicated!)
  uint8_t month = 0;
  if (s_month.equals("Jan"))
  {
    month = 1;
  }
  else if (s_month.equals("Feb"))
  {
    month = 2;
  }
  else if (s_month.equals("Mar"))
  {
    month = 3;
  }
  else if (s_month.equals("Apr"))
  {
    month = 4;
  }
  else if (s_month.equals("May"))
  {
    month = 5;
  }
  else if (s_month.equals("Jun"))
  {
    month = 6;
  }
  else if (s_month.equals("Jul"))
  {
    month = 7;
  }
  else if (s_month.equals("Aug"))
  {
    month = 8;
  }
  else if (s_month.equals("Sep"))
  {
    month = 9;
  }
  else if (s_month.equals("Oct"))
  {
    month = 10;
  }
  else if (s_month.equals("Nov"))
  {
    month = 11;
  }
  else if (s_month.equals("Dec"))
  {
    month = 12;
  }

  // set the date
  _rtc.setDate(day, month, year);
}

// get the date as a nicely formatted string
String MyRTC::get_rtc_date()
{
  // create a nicely formatted string
  char date_str[12];
  sprintf(date_str, "%02d/%02d/%02d",
          _rtc.getDay(), _rtc.getMonth(), _rtc.getYear());

  // return trhis character array wrapped in an arduino string
  return String(date_str);

}

// get the time as a nicely formatted string
String MyRTC::get_rtc_time()
{
  // create a nicely formatted string
  char time_str[12];
  sprintf(time_str, "%02d:%02d:%02d",
          _rtc.getHours(), _rtc.getMinutes(), _rtc.getSeconds());

  // return trhis character array wrapped in an arduino string
  return String(time_str);

}
