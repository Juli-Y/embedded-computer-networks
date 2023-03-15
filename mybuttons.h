/**
  mybuttons.h

  This file contrains all the button declaration,
  size & x,y coordinates of each button.
  This header file also contains the coordinates for
  the virtual LEDs.

  Author: Julian Yuste
  Date:   15/03/2023
*/


// GRAPHICS OBJECTS
// GLOBAL
Adafruit_GFX_Button home_button;
Adafruit_GFX_Button back_button;
Adafruit_GFX_Button plus_button;
Adafruit_GFX_Button minus_button;

// Set the height and width of the buttons
const int button_h = 40;
const int button_w = 100;
const int button2_h = 30;
const int button2_w = 60;
const int button3_h = 30;
const int button3_w = 80;

// Set the x and y positions of buttons
const int home_button_x = 45;
const int home_button_y = 220;
const int back_button_x = 45;
const int back_button_y = 220;
const int plus_button_x = 265;
const int plus_button_y = 200;
const int minus_button_x = 180;
const int minus_button_y = 200;


// HOME SCREEN BUTTONS
Adafruit_GFX_Button heating_button;
Adafruit_GFX_Button lighting_button;
Adafruit_GFX_Button security_button;
Adafruit_GFX_Button eco_button;

// Set the x and y positions of buttons
const int eco_button_x = 210;
const int eco_button_y = 170;
const int heating_button_x = 100;
const int heating_button_y = 120;
const int lighting_button_x = 210;
const int lighting_button_y = 120;
const int security_button_x = 100;
const int security_button_y = 170;


// HEATING SCREEN BUTTON
Adafruit_GFX_Button heater_onoff_button;

// Set the x and y positions of buttons
const int heater_onoff_button_x = 185;
const int heater_onoff_button_y = 95;
const int heater_led_x = 270;
const int heater_led_y = 95;


// MAIN LIGHTING SCREEN BUTTONS
Adafruit_GFX_Button outdoor_button;
Adafruit_GFX_Button zone1_button;
Adafruit_GFX_Button zone2_button;
Adafruit_GFX_Button zone3_button;

// Set the x and y positions of buttons
const int outdoor_button_x = 260;
const int outdoor_button_y = 60;
const int zone1_button_x = 260;
const int zone1_button_y = 110;
const int zone2_button_x = 260;
const int zone2_button_y = 160;
const int zone3_button_x = 260;
const int zone3_button_y = 210;


// ACCESS CONTROL & SECURITY BUTTONS
Adafruit_GFX_Button zero_button;
Adafruit_GFX_Button one_button;
Adafruit_GFX_Button two_button;
Adafruit_GFX_Button three_button;
Adafruit_GFX_Button four_button;
Adafruit_GFX_Button five_button;
Adafruit_GFX_Button six_button;
Adafruit_GFX_Button seven_button;
Adafruit_GFX_Button eight_button;
Adafruit_GFX_Button nine_button;
Adafruit_GFX_Button alarm_onoff_button;
Adafruit_GFX_Button alarm_reset_button;

// Set the x and y positions of buttons
const int alarm_onoff_button_x = 125;
const int alarm_onoff_button_y = 95;
const int alarm_reset_button_x = 125;
const int alarm_reset_button_y = 150;
const int zero_button_x = 160;
const int zero_button_y = 190;
const int one_button_x = 90;
const int one_button_y = 70;
const int two_button_x = 160;
const int two_button_y = 70;
const int three_button_x = 230;
const int three_button_y = 70;
const int four_button_x = 90;
const int four_button_y = 110;
const int five_button_x = 160;
const int five_button_y = 110;
const int six_button_x = 230;
const int six_button_y = 110;
const int seven_button_x = 90;
const int seven_button_y = 150;
const int eight_button_x = 160;
const int eight_button_y = 150;
const int nine_button_x = 230;
const int nine_button_y = 150;
const int alarm_led_x = 205;
const int alarm_led_y = 120;


// ECO SCREEN BUTTONS
Adafruit_GFX_Button heating_pir_button;
Adafruit_GFX_Button lighting_pir_button;
Adafruit_GFX_Button timer_plus_button;
Adafruit_GFX_Button timer_minus_button;
Adafruit_GFX_Button lighting_timer_plus_button;
Adafruit_GFX_Button lighting_timer_minus_button;
Adafruit_GFX_Button heating_timer_plus_button;
Adafruit_GFX_Button heating_timer_minus_button;

// Set the x and y positions of buttons
const int heating_pir_button_x = 40;
const int heating_pir_button_y = 95;
const int lighting_pir_button_x = 210;
const int lighting_pir_button_y = 95;
const int lighting_timer_plus_x = 285;
const int lighting_timer_plus_y = 180;
const int lighting_timer_minus_x = 200;
const int lighting_timer_minus_y = 180;
const int heating_timer_plus_x = 120;
const int heating_timer_plus_y = 180;
const int heating_timer_minus_x = 35;
const int heating_timer_minus_y = 180;
const int timer_plus_button_x = 100;
const int timer_plus_button_y = 110;
const int timer_minus_button_x = 210;
const int timer_minus_button_y = 110;
const int heating_pir_led_x = 120;
const int heating_pir_led_y = 95;
const int lighting_pir_led_x = 290;
const int lighting_pir_led_y = 95;
