/**
  This program has been created for my final year embedded
  computer nerworks assignment. This is a building automation
  system that quantifies enviromental data such as light,
  temperature and humidity. In addition, this program makes
  the use of PIR sensors to be used as motion detectors
  and potentiometers to be used as dimmer switches. The user
  interface is designed to be user friendly and can be easily
  navigated without instructions. Lastlty, this program is
  IoT enabled, enviromental parameters can be monitored in
  real time using the Adafruit dashboard. Some parameters can
  also be controled via the dashbord such as to enable smart
  monitoring of light and heating control, enable the alrm system
  and enabling the heater control and temperature setpoint.

  Author: Julian Yuste
  Date:   10/03/2023
*/

// INCLUDE
// Include the necessary libraries & files
#include <SPI.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "my_rtc.h"
#include "Adafruit_SHT4x.h"
#include "MyCredentials.h"
#include "mybuttons.h"


// INITIAL DEFINES
// create my rtc object
MyRTC rtc;

// Create the sensor object
Adafruit_SHT4x sht40 = Adafruit_SHT4x();

// LCD AND TOUCHSCREEN
// Define the custom pins
#define TFT_DC 6
#define TFT_CS 7

// Create display and touchscreen objects
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206   ts = Adafruit_FT6206();

// Set LCD size
const int lcd_width  = 320;
const int lcd_height = 240;


// INITIAL VARIABLES
// Potentiometers
const int zone1_pot = A1;
const int zone2_pot = A2;
const int zone3_pot = A3;

// Sensors
const int ldr_pin = A0;
const int alarm_pir_pin = 0;
const int light_pir_pin = 1;
const int heater_pir_pin = 2;

// LEDs
const int heater_led_pin = A5;
const int power_led_pin = A6;
const int zone1_led_pin = 3;
const int zone2_led_pin = 4;
const int zone3_led_pin = 5;
const int outdoor_led_pin = 13;
const int alarm_led_pin = 14;

// Virtual led radius (LED on screen)
const int led_r = 20;


// TIMING VARIABLES
// Alarm monitoring
long previous_time1 = 0;
long check_interval = 15000;

// Light monitoring
long previous_time2 = 0;
long check_interval2 = 500;
long check_new_interval = 15000;

// Heater monitoring
long previous_time3 = 0;
long check_interval3 = 500;
long check_new_interval2 = 15000;

// Checks flag if motion is detected for PIRs
bool alarm_motion = false;
bool lighting_motion = false;
bool heating_motion = false;


// STATE VARIABLES
int currentPage = 0;
int set_temp = 21;
int light_level = 0;
int ldr_scale_position = 2;

// Keeps track of the virtual led states
int heater_led_state = LOW;
int heater_led_new_state;
int alarm_led_state = LOW;
int alarm_led_new_state;
int heating_pir_led_state = LOW;
int heating_pir_led_new_state;
int lighting_pir_led_state = LOW;
int lighting_pir_led_new_state;


//SECURITY SCREEN PASSCODE VARIABLES
// Corret passcode
int passcode[4] = {6, 9, 1, 2};
// variable to store input digits
int input_passcode[4];
// Variable to store last pressed digit
int input_num;
// Variable to correlate the current digit
int slot = 0;
int count = 0;
int i = 0;
// Sets the 'x' coordinate for the passcode
// to be shown on the touchscreen
int digit[4] = {60, 124, 188, 252};


// WIFI INITIALISATION
// set the initial wifi status (to idle)
int status = WL_IDLE_STATUS;

// MQTT DECLARATIONS
// mqtt server details
const char server[] = "io.adafruit.com";
const long port     = 1883;

// Timing variables for MQTT - note: adafruit.io allows
// a maximum of 30 messages per minute
// any more than this and your account will be throttled
// and then blocked!
long previous_time = 0;
long connection_interval = 10000;

// Get a wifi client object and pass it to the mqtt client
WiFiClient wifi_client;
PubSubClient client(wifi_client);


// CODE
// This method runs once (when the sketch starts)
void setup()
{
  // Set up serial comms for debugging and display of DHCP allocated
  // IP address
  Serial.begin(115200);
  while (!Serial);
  Serial.println("starting mqtt client on arduino ...");

  // MQTT server and subscription callback
  client.setServer(server, port);
  client.setCallback(callback);

  // Attempt to connect to the wifi network
  while (status != WL_CONNECTED)
  {
    Serial.print("attempting to connect to network: ");
    Serial.println(my_ssid);

    // Connect to wifi network
    status = WiFi.begin(my_ssid, my_pass);

    // Wait for 5 seconds for wifi to come up
    delay(5000);
  }

  Serial.println("connected to WiFi network");

  // Print the IP address to the serial monitor
  IPAddress my_ip = WiFi.localIP();
  Serial.print("my ip address: ");
  Serial.println(my_ip);

  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("my signal strength (rssi):");
  Serial.print(rssi);
  Serial.println(" dBm");


  // Sets the pins as outputs or inputs
  pinMode(power_led_pin, OUTPUT);
  pinMode(heater_led_pin, OUTPUT);
  pinMode(outdoor_led_pin, OUTPUT);
  pinMode(zone1_led_pin, OUTPUT);
  pinMode(zone2_led_pin, OUTPUT);
  pinMode(zone3_led_pin, OUTPUT);
  pinMode(alarm_led_pin, OUTPUT);
  pinMode(alarm_pir_pin, INPUT);
  pinMode(light_pir_pin, INPUT);
  pinMode(heater_pir_pin, INPUT);

  // Initialise PIR sensors as LOW
  digitalWrite(alarm_pir_pin, LOW);
  digitalWrite(light_pir_pin, LOW);
  digitalWrite(heater_pir_pin, LOW);

  // Initialise the rtc with the compilation time and date
  rtc.set_rtc();

  // Start the temp and humidity sensor
  sht40.begin();

  // Configure the sensor to have high precision (i.e. high repeatability
  // and heater off
  sht40.setPrecision(SHT4X_HIGH_PRECISION);
  sht40.setHeater(SHT4X_NO_HEATER);

  // Start the touchscreen & make the screen black & horizontal
  ts.begin();
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  // Initialise touch screen with the Home display
  drawHomeScreen();

  // Pints a message to the serial monitor and turns on
  // power LED to show that the system is running
  Serial.println("System is running...");
  digitalWrite(power_led_pin, HIGH);
}


// This method loops continuously
void loop()
{
  // Initialise structures to contain the sensor readings
  sensors_event_t humidity;
  sensors_event_t temp;
  sht40.getEvent(&humidity, &temp);

  // Reads the light sensor and maps it to be 0 to 5
  // Also maps from 0 to 100% to send to MQTT
  int ldr_value = analogRead(ldr_pin);
  int light_level = map(ldr_value, 140, 980, 0, 5);
  int light_percent = map(ldr_value, 140, 980, 0, 100);

  // Reads the potentiometers for zone lighting (varies between 0-1023)
  int zone1_pot_value = analogRead(zone1_pot);
  int zone2_pot_value = analogRead(zone2_pot);
  int zone3_pot_value = analogRead(zone3_pot);


  //IoT
  // If the client isn't connected then try to reconnect
  if (!client.connected())
  {
    reconnect();
    previous_time = millis();
  }
  else
  {
    // Handle subscriptions to topics
    client.loop();

    // Periodically publish a message to a feed
    unsigned long current_time = millis();
    if (current_time - previous_time > connection_interval)
    {
      previous_time = current_time;
      send_lighting();
      send_temp_humid();
    }
  }


  // Home Screen
  if (currentPage == 0)
  {
    // Print date, time, temperature & humidity in a readable format
    // set the text colour, background colour, and text size
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 62);
    tft.print(rtc.get_rtc_date());
    tft.print("  ");
    tft.print(rtc.get_rtc_time());
    tft.print("  ");
    sht40.getEvent(&humidity, &temp);
    tft.setCursor(256, 62);
    tft.print(temp.temperature, 0);
    tft.print(" C");

    // Writes message at the bottom of the screen
    // refering to alarm system status
    if (alarm_led_new_state == LOW)
    {
      // If alarm is disabled, print correct message
      tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
      tft.setCursor(45, 210);
      tft.println("- Alarm Disabled -  ");
    }
    else if (alarm_motion == true)
    {
      // If modion has been detected, print correct message
      tft.setTextColor(ILI9341_RED, ILI9341_ORANGE);
      //tft.setTextSize(3);
      tft.setCursor(45, 210);
      tft.println("- Intruder Alert! -");
    }
    else
    {
      // If alarm is enabled, print correct message
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.setCursor(45, 210);
      tft.println("-  System Armed  -  ");
    }

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Get the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Check if eco button has been pressed
      if (eco_button.contains(x, y))
      {
        eco_button.press(true);
      }

      // Check if heating button has been pressed
      if (heating_button.contains(x, y))
      {
        heating_button.press(true);
      }

      // Check if lighting button has been pressed
      if (lighting_button.contains(x, y))
      {
        lighting_button.press(true);
      }

      // Check if security button has been pressed
      if (security_button.contains(x, y))
      {
        security_button.press(true);
      }
    }
    else
    {
      eco_button.press(false);
      heating_button.press(false);
      lighting_button.press(false);
      security_button.press(false);
    }


    // If the heating button is pressed, set current page to 2,
    // clear the screen and draw the heating screen
    if (heating_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 2;
      drawHeatingScreen();
    }

    // If the lighting button is pressed, set current page to 3,
    // clear the screen and draw the lighting screen
    if (lighting_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 3;
      drawLightingScreen();
    }

    // If the security button is pressed, set current page to 8,
    // clear the screen and draw the passcode screen
    if (security_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 8;
      drawPasscodeScreen();
    }

    // If the eco button is pressed, set current page to 1,
    // clear the screen and draw the energy saving screen
    if (eco_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 1;
      drawEcoScreen();
    }
  }


  // Eco Screen
  if (currentPage == 1)
  {
    // Print heating screen values
    // set the text colour, background colour, and text size
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(225, 140);
    tft.print(check_new_interval / 1000);
    tft.print("s  ");
    tft.setCursor(60, 140);
    tft.print(check_new_interval2 / 1000);
    tft.print("s  ");

    // Prints correct colour LED for lighting PIR and remembers the last state
    if (lighting_pir_led_new_state == LOW)
    {
      tft.fillCircle(lighting_pir_led_x, lighting_pir_led_y, led_r, ILI9341_RED);
    }
    else
    {
      tft.fillCircle(lighting_pir_led_x, lighting_pir_led_y, led_r, ILI9341_GREEN);
    }

    // Prints correct colour LED for heating PIR and remembers the last state
    if (heating_pir_led_new_state == LOW)
    {
      tft.fillCircle(heating_pir_led_x, heating_pir_led_y, led_r, ILI9341_RED);
    }
    else
    {
      tft.fillCircle(heating_pir_led_x, heating_pir_led_y, led_r, ILI9341_GREEN);
    }

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Get the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Check if the Home button has been pressed
      if (home_button.contains(x, y))
      {
        home_button.press(true);
      }

      // Check if the lighting PIR button has been pressed
      if (lighting_pir_button.contains(x, y))
      {
        lighting_pir_button.press(true);
      }

      // Check if the timer plus button has been pressed
      if (lighting_timer_plus_button.contains(x, y))
      {
        lighting_timer_plus_button.press(true);
      }

      // Check if the timer minus button has been pressed
      if (lighting_timer_minus_button.contains(x, y))
      {
        lighting_timer_minus_button.press(true);
      }

      // Check if the heating PIR button has been pressed
      if (heating_pir_button.contains(x, y))
      {
        heating_pir_button.press(true);
      }

      // Check if the timer plus button has been pressed
      if (heating_timer_plus_button.contains(x, y))
      {
        heating_timer_plus_button.press(true);
      }

      // Check if the timer minus button has been pressed
      if (heating_timer_minus_button.contains(x, y))
      {
        heating_timer_minus_button.press(true);
      }
    }
    else
    {
      home_button.press(false);
      lighting_pir_button.press(false);
      heating_pir_button.press(false);
      heating_timer_plus_button.press(false);
      heating_timer_minus_button.press(false);
      lighting_timer_minus_button.press(false);
      lighting_timer_plus_button.press(false);
    }

    // If the Home button is pressed, set current page to 0, clear the screen and draw the home screen
    if (home_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 0;
      drawHomeScreen();
    }

    // If the lighting PIR button is pressed,
    // change state & toggle LED
    if (lighting_pir_button.justPressed())
    {
      if (lighting_pir_led_state == LOW)
      {
        lighting_pir_led_state = HIGH;
        lighting_pir_led_new_state = lighting_pir_led_state;
        Serial.println("Light Monitoring Activated");
        client.publish((USER_ID "/feeds/light-monitor"), "ON");
      }
      else
      {
        lighting_pir_led_state = LOW;
        lighting_pir_led_new_state = lighting_pir_led_state;
        Serial.println("Light Monitoring Disabled");
        client.publish((USER_ID "/feeds/light-monitor"), "OFF");
      }
    }

    // If the heating PIR button is pressed,
    // change state & toggle LED
    if (heating_pir_button.justPressed())
    {
      if (heating_pir_led_state == LOW)
      {
        heating_pir_led_state = HIGH;
        heating_pir_led_new_state = heating_pir_led_state;
        Serial.println("Heater Monitoring Activated");
        client.publish((USER_ID "/feeds/heating-monitor"), "ON");
      }
      else
      {
        heating_pir_led_state = LOW;
        heating_pir_led_new_state = heating_pir_led_state;
        Serial.println("Heater Monitoring Disabled");
        client.publish((USER_ID "/feeds/heating-monitor"), "OFF");
      }
    }

    // If the heating minus button is pressed, decrement check_interval2 by 5s
    if (heating_timer_minus_button.justPressed())
    {
      check_new_interval2 = check_new_interval2  - 5000;
    }

    // If the heating plus button is pressed, increment check_interval2 by 5s
    if (heating_timer_plus_button.justPressed())
    {
      check_new_interval2 = check_new_interval2 + 5000;
    }

    // If the lighting minus button is pressed, decrement check_interval2 by 5s
    if (lighting_timer_minus_button.justPressed())
    {
      check_new_interval = check_new_interval  - 5000;
    }

    // If the lighting plus button is pressed, increment check_interval2 by 5s
    if (lighting_timer_plus_button.justPressed())
    {
      check_new_interval = check_new_interval + 5000;
    }
  }


  // Heating Screen
  if (currentPage == 2)
  {
    // Print heating screen values
    // set the text colour, background colour, and text size
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(2);
    sht40.getEvent(&humidity, &temp);
    tft.setCursor(35, 155);
    tft.print(temp.temperature, 0);
    tft.print(" C");
    tft.setCursor(35, 75);
    tft.print(humidity.relative_humidity, 0);
    tft.print(" %");
    tft.setCursor(195, 155);
    tft.print(set_temp);
    tft.print(" C");

    // If the heater monitoring is activated,
    if (heating_pir_led_state == HIGH)
    {
      if (heating_motion == true)
      {
        // Prints correct colour LED and remembers the last state
        if (heater_led_new_state == HIGH)
        {
          tft.fillCircle(heater_led_x, heater_led_y, led_r, ILI9341_GREEN);
        }
      }
    }
    else
    {
      // Prints correct colour LED and remembers the last state
      if (heater_led_new_state == LOW)
      {
        tft.fillCircle(heater_led_x, heater_led_y, led_r, ILI9341_RED);
      }
      else
      {
        tft.fillCircle(heater_led_x, heater_led_y, led_r, ILI9341_GREEN);
      }
    }

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Get the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Checks if the home button has been pressed
      if (home_button.contains(x, y))
      {
        home_button.press(true);
      }

      // Checks if heter ON/OFF button has been pressed
      if (heater_onoff_button.contains(x, y))
      {
        heater_onoff_button.press(true);
      }

      // Checks if the plus (increment) button has been pressed
      if (plus_button.contains(x, y))
      {
        plus_button.press(true);
      }

      // Checks if the minus (decrement) button has been pressed
      if (minus_button.contains(x, y))
      {
        minus_button.press(true);
      }
    }
    else
    {
      minus_button.press(false);
      plus_button.press(false);
      heater_onoff_button.press(false);
      home_button.press(false);
    }

    // If the home button is pressed, set current page to 0,
    // clear the screen and draw the home screen
    if (home_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 0;
      drawHomeScreen();
    }

    // If the heater ON/OFF button is pressed,
    // change state & toggle LED
    if (heater_onoff_button.justPressed())
    {
      if (heater_led_state == LOW)
      {
        heater_led_state = HIGH;
        heater_led_new_state = heater_led_state;
        Serial.println("Heater System Activated.");
        client.publish((USER_ID "/feeds/heater-system"), "ON");
      }
      else
      {
        heater_led_state = LOW;
        heater_led_new_state = heater_led_state;
        Serial.println("Heater System Disabled.");
        client.publish((USER_ID "/feeds/heater-system"), "OFF");
      }
    }

    // If the plus button is pressed, increment set_temp
    if (plus_button.justPressed())
    {
      set_temp = set_temp + 1;
    }

    // If the minus button is pressed, decrement set_temp
    if (minus_button.justPressed())
    {
      set_temp = set_temp - 1;
    }
  }


  // HEATER
  // (Code is placed here to makes sure signal is always updated)
  // Heating PIR Sensor (smart monitoring)
  // If eco mode is activated, Check the heating pir sensor after 500ms
  if (heating_pir_led_state == HIGH)
  {
    unsigned long current_time3 = millis();
    if (current_time3 - previous_time3 > check_interval3)
    {
      previous_time3 = current_time3;

      // If the pir sensor reads high
      if (digitalRead(heater_pir_pin) == HIGH)
      {
        // Print a message to the serial monitor & set flag to true.
        // Publish an announcement on the MQTT dashboard
        Serial.println("Motion Detected, (Heaters)");
        client.publish((USER_ID "/feeds/status-messages"), "Motion Detected (Heating)");
        heating_motion = true;

        // Check PIR sensor again after user specified
        // In real world would be minutes instead of s.
        check_interval3 = check_new_interval2;
      }
      else
      {
        // If no motion detected, set flag to false
        Serial.println("NO motion detected for heaters");
        heating_motion = false;

        // Check PIR sensor again after 500ms
        check_interval3 = 500;
      }
    }

    // IF motion has been detected & the heater is activated & the
    // room temp is lower than the set temp, turn the heater ON.
    if (heating_motion == true)
    {
      if (heater_led_new_state == HIGH && temp.temperature < set_temp)
      {
        digitalWrite(heater_led_pin, HIGH);
      }
      else
      {
        digitalWrite(heater_led_pin, LOW);

      }
    }
    else
    {
      digitalWrite(heater_led_pin, LOW);
    }
  }
  else
  {
    // If Eco mode is disabled, proceed without (smart monitoring)
    if (heater_led_new_state == HIGH && temp.temperature < set_temp)
    {
      digitalWrite(heater_led_pin, HIGH);
    }
    else
    {
      digitalWrite(heater_led_pin, LOW);
    }
  }


  // Lighting Screen
  if (currentPage == 3)
  {
    // Remap the potentiometer values between 0 and 255 (as that is
    // what the pwm signal needs) & sends the pwm signal to each LED for zone lighting
    int zone1_pwm_value = map(zone1_pot_value, 38, 1019, 0, 255);
    int zone2_pwm_value = map(zone2_pot_value, 38, 1019, 0, 255);
    int zone3_pwm_value = map(zone3_pot_value, 38, 1019, 0, 255);

    // If the light monitoring is activated,
    // Prints the correct message on the screen if LED is ON or OFF.
    if (lighting_pir_led_state == HIGH)
    {
      if (lighting_motion == true)
      {
        if (ldr_scale_position == 0 )
        {
          tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
          tft.setCursor(15, 75);
          tft.println("Outdoor:  OFF");
        }
        else if (light_level <= 1 && ldr_scale_position == 1)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 75);
          tft.println("Outdoor:  ON ");
        }
        else if (light_level < 2 && ldr_scale_position == 2)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 75);
          tft.println("Outdoor:  ON ");
        }
        else if (light_level < 3 && ldr_scale_position == 3)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 75);
          tft.println("Outdoor:  ON ");
        }
        else if (ldr_scale_position == 4)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 75);
          tft.println("Outdoor:  ON ");
        }
        else
        {
          tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
          tft.setCursor(15, 75);
          tft.println("Outdoor:  OFF");
        }

        // Prints the correct message on the screen
        // if PWM signal for zone 1 is registered (dimmer switch)
        if (zone1_pwm_value > 20)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 100);
          tft.println("Zone 1 :  ON ");
        }
        else
        {
          tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
          tft.setCursor(15, 100);
          tft.println("Zone 1 :  OFF");
        }

        // Prints the correct message on the screen
        // if PWM signal for zone 2 is registered (dimmer switch)
        if (zone2_pwm_value > 20)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 125);
          tft.println("Zone 2 :  ON ");
        }
        else
        {
          tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
          tft.setCursor(15, 125);
          tft.println("Zone 2 :  OFF");
        }

        // Prints the correct message on the screen
        // if PWM signal for zone 3 is registered (dimmer switch)
        if (zone3_pwm_value > 20)
        {
          tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
          tft.setCursor(15, 150);
          tft.println("Zone 3 :  ON ");
        }
        else
        {
          tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
          tft.setCursor(15, 150);
          tft.println("Zone 3 :  OFF");
        }
      }
      else
      {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.setCursor(15, 100);
        tft.println("Zone 1 :  OFF");
        tft.setCursor(15, 125);
        tft.println("Zone 2 :  OFF");
        tft.setCursor(15, 150);
        tft.println("Zone 3 :  OFF");
      }
    }
    else
    {
      if (ldr_scale_position == 0 )
      {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.setCursor(15, 75);
        tft.println("Outdoor:  OFF");
      }
      else if (light_level <= 1 && ldr_scale_position == 1)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 75);
        tft.println("Outdoor:  ON ");
      }
      else if (light_level < 2 && ldr_scale_position == 2)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 75);
        tft.println("Outdoor:  ON ");
      }
      else if (light_level < 3 && ldr_scale_position == 3)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 75);
        tft.println("Outdoor:  ON ");
      }
      else if (ldr_scale_position == 4)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 75);
        tft.println("Outdoor:  ON ");
      }
      else
      {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.setCursor(15, 75);
        tft.println("Outdoor:  OFF");
      }

      // Prints the correct message on the screen
      // if PWM signal for zone 1 is registered (dimmer switch)
      if (zone1_pwm_value > 20)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 100);
        tft.println("Zone 1 :  ON ");
      }
      else
      {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.setCursor(15, 100);
        tft.println("Zone 1 :  OFF");
      }

      // Prints the correct message on the screen
      // if PWM signal for zone 2 is registered (dimmer switch)
      if (zone2_pwm_value > 20)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 125);
        tft.println("Zone 2 :  ON ");
      }
      else
      {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.setCursor(15, 125);
        tft.println("Zone 2 :  OFF");
      }

      // Prints the correct message on the screen
      // if PWM signal for zone 3 is registered (dimmer switch)
      if (zone3_pwm_value > 20)
      {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.setCursor(15, 150);
        tft.println("Zone 3 :  ON ");
      }
      else
      {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.setCursor(15, 150);
        tft.println("Zone 3 :  OFF");
      }
    }


    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gets the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Checks if the home button has been pressed
      if (home_button.contains(x, y))
      {
        home_button.press(true);
      }

      // Checks if the outdoor button has been pressed
      if (outdoor_button.contains(x, y))
      {
        outdoor_button.press(true);
      }

      // Checks if the zone 1 button has been pressed
      if (zone1_button.contains(x, y))
      {
        zone1_button.press(true);
      }

      // Checks if the zone 2 button has been pressed
      if (zone2_button.contains(x, y))
      {
        zone2_button.press(true);
      }

      // Checks if the zone 3 button has been pressed
      if (zone3_button.contains(x, y))
      {
        zone3_button.press(true);
      }
    }
    else
    {
      zone3_button.press(false);
      zone2_button.press(false);
      zone1_button.press(false);
      outdoor_button.press(false);
      home_button.press(false);
    }

    // If the home button is pressed, set current page to 0,
    // clear the screen and draw the home screen
    if (home_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 0;
      drawHomeScreen();
    }

    // If the outdoor button is pressed, set current page to 4,
    // clear the screen and draw the outdoor screen
    if (outdoor_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 4;
      drawOutdoorScreen();
    }

    // If the zone 1 button is pressed, set current page to 5,
    // clear the screen and draw the zone 1 screen
    if (zone1_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 5;
      drawZone1Screen();
    }

    // If the zone 2 button is pressed, set current page to 6,
    // clear the screen and draw the zone 2 screen
    if (zone2_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 6;
      drawZone2Screen();
    }

    // If the zone 3 button is pressed, set current page to 7,
    // clear the screen and draw the zone 3 screen
    if (zone3_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 7;
      drawZone3Screen();
    }
  }


  // Outdoor Screen
  if (currentPage == 4)
  {
    if (ldr_scale_position == 0)
    {
      //Clear other bars
      tft.fillRect(103, 90, 20, 25, ILI9341_BLACK); //position 1
      tft.fillRect(145, 90, 20, 25, ILI9341_BLACK); //position 2
      tft.fillRect(187, 90, 20, 25, ILI9341_BLACK); //position 3
      tft.fillRect(230, 90, 20, 25, ILI9341_BLACK); //position 4

      // Display the bar
      tft.fillRect(60, 90, 20, 25, ILI9341_GREEN);
    }

    if (ldr_scale_position == 1)
    {
      //Clear other bars
      tft.fillRect(145, 90, 20, 25, ILI9341_BLACK); //position 2
      tft.fillRect(187, 90, 20, 25, ILI9341_BLACK); //position 3
      tft.fillRect(230, 90, 20, 25, ILI9341_BLACK); //position 4
      tft.fillRect(60, 90, 20, 25, ILI9341_BLACK); //position 0

      // Display the bar
      tft.fillRect(103, 90, 20, 25, ILI9341_GREEN);
    }

    if (ldr_scale_position == 2)
    {
      //Clear other bars
      tft.fillRect(103, 90, 20, 25, ILI9341_BLACK); //position 1
      tft.fillRect(187, 90, 20, 25, ILI9341_BLACK); //position 3
      tft.fillRect(230, 90, 20, 25, ILI9341_BLACK); //position 4
      tft.fillRect(60, 90, 20, 25, ILI9341_BLACK); //position 0

      // Display the bar
      tft.fillRect(145, 90, 20, 25, ILI9341_GREEN);
    }

    if (ldr_scale_position == 3)
    {
      // Clear other bars
      tft.fillRect(103, 90, 20, 25, ILI9341_BLACK); //position 1
      tft.fillRect(145, 90, 20, 25, ILI9341_BLACK); //position 2
      tft.fillRect(230, 90, 20, 25, ILI9341_BLACK); //position 4
      tft.fillRect(60, 90, 20, 25, ILI9341_BLACK); //position 0

      // Display the bar
      tft.fillRect(187, 90, 20, 25, ILI9341_GREEN);
    }

    if (ldr_scale_position == 4)
    {
      // Clear other bars
      tft.fillRect(103, 90, 20, 25, ILI9341_BLACK); //position 1
      tft.fillRect(145, 90, 20, 25, ILI9341_BLACK); //position 2
      tft.fillRect(187, 90, 20, 25, ILI9341_BLACK); //position 3
      tft.fillRect(60, 90, 20, 25, ILI9341_BLACK); //position 0

      // Display the bar
      tft.fillRect(230, 90, 20, 25, ILI9341_GREEN);
    }

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gets the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Checks if the back button has been pressed
      if (back_button.contains(x, y))
      {
        back_button.press(true);
      }

      // Checks if the plus button has been pressed
      if (plus_button.contains(x, y))
      {
        plus_button.press(true);
      }

      // Checks if the minus button has been pressed
      if (minus_button.contains(x, y))
      {
        minus_button.press(true);
      }
    }
    else
    {
      minus_button.press(false);
      plus_button.press(false);
      back_button.press(false);
    }

    // If the back button is pressed, set current page to 3,
    // clear the screen and draw the lighting screen
    if (back_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 3;
      drawLightingScreen();
    }

    // If the plus button is pressed, increment ldr_scale_position
    if (plus_button.justPressed())
    {
      ldr_scale_position = ldr_scale_position + 1;
    }

    // If the minus button is pressed, decrement ldr_scale_position
    if (minus_button.justPressed())
    {
      ldr_scale_position = ldr_scale_position - 1;
    }
  }


  // LIGHTING
  // Outdoor Lights
  // (Code is placed here to makes sure signal is always updated)
  // Turns ON outdoor light LED if the ldr reading is greater than
  // the scale position set by the user.
  if (ldr_scale_position == 0 )
  {
    digitalWrite(outdoor_led_pin, LOW);
  }
  else if (light_level <= 1 && ldr_scale_position == 1)
  {
    digitalWrite(outdoor_led_pin, HIGH);
  }
  else if (light_level < 2 && ldr_scale_position == 2)
  {
    digitalWrite(outdoor_led_pin, HIGH);
  }
  else if (light_level < 3 && ldr_scale_position == 3)
  {
    digitalWrite(outdoor_led_pin, HIGH);
  }
  else if (ldr_scale_position >= 4)
  {
    digitalWrite(outdoor_led_pin, HIGH);
  }
  else
  {
    digitalWrite(outdoor_led_pin, LOW);
  }


  // Zone 1 Screen
  if (currentPage == 5)
  {
    // Maps the potentiometer from 0 to 100 & 100 to 0 for negative fill
    int zone1_pot_percent = map(zone1_pot_value, 38, 1019, 0, 100);
    int zone1_pot_percent2 = map(zone1_pot_value, 38, 1019, 100, 0);

    // Calculate the bars height
    int zone1_bar_height = ((zone1_pot_percent * 1));
    int zone1_bar_height2 = ((zone1_pot_percent2 * 1));

    // Display the bar (green)
    tft.fillRect(240, 180, 35, -zone1_bar_height, ILI9341_GREEN);

    // Display the bar (green)
    tft.fillRect(240, 80, 35, zone1_bar_height2, ILI9341_RED);

    //Prints the potentiometer value as percentage on the display.
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(3);
    tft.setCursor(80, 130);
    tft.print(zone1_pot_percent);
    tft.print(" % ");

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gest the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Check if the back button has been pressed
      if (back_button.contains(x, y))
      {
        back_button.press(true);
      }
    }
    else
    {
      back_button.press(false);
    }

    // If the back button is pressed, set current page to 3,
    // clear the screen and draw the lighting screen
    if (back_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 3;
      drawLightingScreen();
    }
  }


  // Zone 2 Screen
  if (currentPage == 6)
  {
    // Maps the potentiometer from 0 to 100 & 100 to 0 for negative fill
    int zone2_pot_percent = map(zone2_pot_value, 38, 1019, 0, 100);
    int zone2_pot_percent2 = map(zone2_pot_value, 38, 1019, 100, 0);

    // Calculate the bars height
    int zone2_bar_height = ((zone2_pot_percent * 1));
    int zone2_bar_height2 = ((zone2_pot_percent2 * 1));

    // Display the bar (green)
    tft.fillRect(240, 180, 35, -zone2_bar_height, ILI9341_GREEN);

    // Display the bar (green)
    tft.fillRect(240, 80, 35, zone2_bar_height2, ILI9341_RED);

    //Prints the potentiometer value as percentage on the display.
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(3);
    tft.setCursor(80, 130);
    tft.print(zone2_pot_percent);
    tft.print(" % ");

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gest the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Check if the back button has been pressed
      if (back_button.contains(x, y))
      {
        back_button.press(true);
      }
    }
    else
    {
      back_button.press(false);
    }

    // If the back button is pressed, set current page to 3,
    // clear the screen and draw the lighting screen
    if (back_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 3;
      drawLightingScreen();
    }
  }


  // Zone 3 Screen
  if (currentPage == 7)
  {
    // Maps the potentiometer from 0 to 100 & 100 to 0 for negative fill
    int zone3_pot_percent = map(zone3_pot_value, 38, 1019, 0, 100);
    int zone3_pot_percent2 = map(zone3_pot_value, 38, 1019, 100, 0);

    // Calculate the bars height
    int zone3_bar_height = ((zone3_pot_percent * 1));
    int zone3_bar_height2 = ((zone3_pot_percent2 * 1));

    // Display the bar (green)
    tft.fillRect(240, 180, 35, -zone3_bar_height, ILI9341_GREEN);

    // Display the bar (green)
    tft.fillRect(240, 80, 35, zone3_bar_height2, ILI9341_RED);

    //Prints the potentiometer value as percentage on the display.
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(3);
    tft.setCursor(80, 130);
    tft.print(zone3_pot_percent);
    tft.print(" % ");

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gest the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Check if the back button has been pressed
      if (back_button.contains(x, y))
      {
        back_button.press(true);
      }
    }
    else
    {
      back_button.press(false);
    }

    // If the back button is pressed, set current page to 3,
    // clear the screen and draw the lighting screen
    if (back_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 3;
      drawLightingScreen();
    }
  }


  // LIGHTING
  // Zone lights PIR Sensor (smart monitoring)
  // If eco mode activated, Check the lighting pir
  // sensor after 500ms
  if (lighting_pir_led_state == HIGH)
  {
    unsigned long current_time2 = millis();
    if (current_time2 - previous_time2 > check_interval2)
    {
      previous_time2 = current_time2;

      // If the pir sensor reads high
      if (digitalRead(light_pir_pin) == HIGH)
      {
        // Print a message to the serial monitor & set flag to true.
        // Publish an announcement on the MQTT dashboard
        Serial.println("Motion detected in Zones");
        client.publish((USER_ID "/feeds/status-messages"), "Motion Detected (Lighting)");
        lighting_motion = true;

        // Check PIR sensor again after user specified
        // In real world would be minutes instead of s.
        check_interval2 = check_new_interval;
      }
      else
      {
        // If no motion detected, set flag to false
        Serial.println("NO motion detected in Zones");
        lighting_motion = false;

        // Check PIR sensor again after 500ms
        check_interval2 = 500;
      }
    }

    // IF motion has been detected
    if (lighting_motion == true)
    {
      // Remap the potentiometer values between 0 and 255 (as that is
      // what the pwm signal needs) & sends the pwm signal to each LED for zone lighting
      int zone1_pwm_value = map(zone1_pot_value, 38, 1019, 0, 255);
      int zone2_pwm_value = map(zone2_pot_value, 38, 1019, 0, 255);
      int zone3_pwm_value = map(zone3_pot_value, 38, 1019, 0, 255);
      analogWrite(zone1_led_pin, zone1_pwm_value);
      analogWrite(zone2_led_pin, zone2_pwm_value);
      analogWrite(zone3_led_pin, zone3_pwm_value);
    }
    else
    {
      // Turn off all LEDs
      analogWrite(zone1_led_pin, LOW);
      analogWrite(zone2_led_pin, LOW);
      analogWrite(zone3_led_pin, LOW);
    }
  }
  else
  {
    // If the eco mode is not active, ignore the sensor
    // and cary on without smart monitoring.
    // Remap the potentiometer values between 0 and 255 (as that is
    // what the pwm signal needs) & sends the pwm signal to each LED for zone lighting
    int zone1_pwm_value = map(zone1_pot_value, 38, 1019, 0, 255);
    int zone2_pwm_value = map(zone2_pot_value, 38, 1019, 0, 255);
    int zone3_pwm_value = map(zone3_pot_value, 38, 1019, 0, 255);
    analogWrite(zone1_led_pin, zone1_pwm_value);
    analogWrite(zone2_led_pin, zone2_pwm_value);
    analogWrite(zone3_led_pin, zone3_pwm_value);
  }


  // Passcode Screen
  if (currentPage == 8)
  {
    // If the 4th button has been pressed
    if (slot == 4)
    {
      // Set the flag to true and check that all entrys in
      // the array, match the correct passcode array
      bool passcode_flag = true;
      for (i = 0; i < 4; i++)
      {
        if (input_passcode[i] != passcode[i])
        {
          // If passcode does not match, set flag to false
          passcode_flag = false;
          break;
        }
      }

      // If the passcode is correct, call the clear function
      // to reset the arrays, print a message on the serial monitor
      // and draw the security screen on the tft display
      if (passcode_flag == true)
      {
        clear_func();
        Serial.println("Correct Passcode");
        tft.fillScreen(ILI9341_BLACK);
        currentPage = 9;
        drawSecurityScreen();
      }
      else
      {
        // If passcode is incorrect, call the function to
        // restart the arrays in order to try again
        Serial.println("Incorrect Passcode");
        clear_func();
      }
    }

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gets the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Checks if buttons have been pressed
      if (home_button.contains(x, y))
      {
        home_button.press(true);
      }

      if (zero_button.contains(x, y))
      {
        zero_button.press(true);
      }

      if (one_button.contains(x, y))
      {
        one_button.press(true);
      }

      if (two_button.contains(x, y))
      {
        two_button.press(true);
      }

      if (three_button.contains(x, y))
      {
        three_button.press(true);
      }

      if (four_button.contains(x, y))
      {
        four_button.press(true);
      }

      if (five_button.contains(x, y))
      {
        five_button.press(true);
      }

      if (six_button.contains(x, y))
      {
        six_button.press(true);
      }

      if (seven_button.contains(x, y))
      {
        seven_button.press(true);
      }

      if (eight_button.contains(x, y))
      {
        eight_button.press(true);
      }

      if (nine_button.contains(x, y))
      {
        nine_button.press(true);
      }
    }
    else
    {
      home_button.press(false);
      zero_button.press(false);
      one_button.press(false);
      two_button.press(false);
      three_button.press(false);
      four_button.press(false);
      five_button.press(false);
      six_button.press(false);
      seven_button.press(false);
      eight_button.press(false);
      nine_button.press(false);
    }

    // If the home button is pressed, set current page to 0,
    // clear the screen and draw the home screen
    if (home_button.justPressed())
    {
      clear_func();
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 0;
      drawHomeScreen();
    }

    // If any of the number buttons have been pressed, store
    // the digit in a variable and call the passcode function
    if (zero_button.justPressed())
    {
      input_num = 0;
      passcode_func();
    }

    if (one_button.justPressed())
    {
      input_num = 1;
      passcode_func();
    }

    if (two_button.justPressed())
    {
      input_num = 2;
      passcode_func();
    }

    if (three_button.justPressed())
    {
      input_num = 3;
      passcode_func();
    }

    if (four_button.justPressed())
    {
      input_num = 4;
      passcode_func();
    }

    if (five_button.justPressed())
    {
      input_num = 5;
      passcode_func();
    }

    if (six_button.justPressed())
    {
      input_num = 6;
      passcode_func();
    }

    if (seven_button.justPressed())
    {
      input_num = 7;
      passcode_func();
    }

    if (eight_button.justPressed())
    {
      input_num = 8;
      passcode_func();
    }

    if (nine_button.justPressed())
    {
      input_num = 9;
      passcode_func();
    }
  }


  // Security Screen
  if (currentPage == 9)
  {
    // Prints correct colour led and remembers the last state
    if (alarm_led_new_state == LOW)
    {
      tft.fillCircle(alarm_led_x, alarm_led_y, led_r, ILI9341_RED);
    }

    if (alarm_led_new_state == HIGH)
    {
      tft.fillCircle(alarm_led_x, alarm_led_y, led_r, ILI9341_GREEN);
    }

    // If the touch screen has been pressed ...
    if (ts.touched())
    {
      // Gets the touch coordinate and flip it to match the orientation
      // of the screen
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, lcd_height, lcd_height, 0);
      p.y = map(p.y, 0, lcd_width, lcd_width, 0);
      int y = tft.height() - p.x;
      int x = p.y;

      // Checks if the home button has been pressed
      if (home_button.contains(x, y))
      {
        home_button.press(true);
      }

      // Checks if the alarm ON/OFF button has been pressed
      if (alarm_onoff_button.contains(x, y))
      {
        alarm_onoff_button.press(true);
      }

      // Checks if the alarm reset button has been pressed
      if (alarm_reset_button.contains(x, y))
      {
        alarm_reset_button.press(true);
      }
    }
    else
    {
      home_button.press(false);
      alarm_onoff_button.press(false);
      alarm_reset_button.press(false);
    }

    // If the home button is pressed, set current page to 0,
    // clear the screen and draw the home screen
    if (home_button.justPressed())
    {
      tft.fillScreen(ILI9341_BLACK);
      currentPage = 0;
      drawHomeScreen();
    }

    // If the ON/OFF button was pressed
    if (alarm_onoff_button.justPressed())
    {
      if (alarm_led_state == LOW)
      {
        // Turn virtual LED ON, remember state and display message
        // to the serial monitor
        alarm_led_state = HIGH;
        alarm_led_new_state = alarm_led_state;
        Serial.println("Starting Security Detection System ...");
        client.publish((USER_ID "/feeds/alarm-system"), "ON");

        // Start time count when ON button is pressed
        previous_time = millis();
      }
      else
      {
        // Turn virtual LED OFF, remember state and display message
        // to the serial monitor
        alarm_led_state = LOW;
        alarm_led_new_state = alarm_led_state;
        Serial.println("Security Detection System Disabled.");
        client.publish((USER_ID "/feeds/alarm-system"), "OFF");
      }
    }

    // If the ON/OFF button was pressed
    if (alarm_reset_button.justPressed())
    {
      client.publish((USER_ID "/feeds/status-messages"), "Alarm Has Been Reset");
    }
  }


  // SECURITY SYSTEM
  // (Code is placed here to makes sure code runs continuously)
  // Enables alarm system only if the ON button is pressed.
  if (alarm_led_new_state == HIGH)
  {
    if (alarm_motion == false)
    {
      // Checks the pir sensor after 10s
      unsigned long current_time = millis();
      if (current_time - previous_time1 > check_interval)
      {
        previous_time1 = current_time;

        // If the pir sensor reads high
        if (digitalRead(alarm_pir_pin) == HIGH)
        {
          // Print an alert message to the serial monitor & turn the LED ON
          // Publish an announcement on the MQTT dashboard
          // Set alarm motion to true
          Serial.println("Motion detected");
          client.publish((USER_ID "/feeds/status-messages"), "Intruder Alert!");
          digitalWrite(alarm_led_pin, HIGH);
          alarm_motion = true;
        }
        else
        {
          // If not, print correct message and keep LED OFF
          Serial.println("No Motion Detected");
          digitalWrite(alarm_led_pin, LOW);
          alarm_motion = false;

          // If nothing detected, check again every 500ms
          check_interval = 500;
        }
      }
    }

    // If alarm activates
    if (alarm_reset_button.justPressed())
    {
      // Set interval time back to 15s
      check_interval = 15000;

      // Print a message to the serial monitor & and turn LED OFF
      Serial.println("System Has Been Reset...");
      digitalWrite(alarm_led_pin, LOW);
      alarm_motion = false;
    }
  }
  else
  {
    // If not, turn LED OFF and reset the flag to false in order
    // to check the sensor again
    digitalWrite(alarm_led_pin, LOW);
    alarm_motion = false;

    // Make sure the interval time is reset to 15s
    check_interval = 15000;
  }
}
